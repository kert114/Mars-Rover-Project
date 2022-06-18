#include <Wire.h>
#include <INA219_WE.h>
#include <SPI.h>
#include <SD.h>
 
INA219_WE ina219; // this is the instantiation of the library for the current sensor
 
//SD
Sd2Card card;
SdVolume volume;
SdFile root;
 
const int chipSelect = 10;
float  closed_loop; // Duty Cycles
float vw, vpd, vb, vref, iL, dutyref, current_mA, deltap, vd, deltav, va, vin, vbt, vpdt, buck_vin; // Measurement Variables
unsigned int sensorValue0, sensorValue1, sensorValue2, sensorValue3, sensorValue4; // ADC sample values declaration
float oc = 0; //internal signals
float Ts = 0.001; //1.25 kHz control frequency. It's better to design the control period as integral multiple of switching period.
unsigned int int_count = 0;
float voltage_limit = 10;
boolean Boost_mode = 0;
boolean CL_mode = 0;
float open_loop = 0.1;
float vbk1 = 0;
float pk1 = 0;
float p;
int state;
float pt = 0;
String dataString, dataString1;
unsigned int loop_trigger;
 
int relaypin ;
float iin;
 
 
 
 
// Timer A CMP1 interrupt. Every 800us the program enters this interrupt.
// This, clears the incoming interrupt flag and triggers the main loop.
 
 
// This subroutine processes all of the analogue samples, creating the required values for the main loop
 
 
 
void sampling() {
 
  // Make the initial sampling operations for the circuit measurements
 
  sensorValue0 = analogRead(A0); //sample Vb
  sensorValue1 = analogRead(A1); //sample Vinofbuck
  sensorValue2 = analogRead(A2); //sample Vref
  sensorValue3 = analogRead(A3); //sample Vpd
  sensorValue4 = analogRead(A7); //sample Vin
  current_mA = ina219.getCurrent_mA(); // sample the inductor current (via the sensor chip)
 
  // Process the values so they are a bit more usable/readable
  // The analogRead process gives a value between 0 and 1023
  // representing a voltage between 0 and the analogue reference which is 4.096V
 
  vb = sensorValue0 * (4.096 / 1023.0); // Convert the Vb sensor reading to volts
  vin = sensorValue4 * (4.096 / 1023.0);
  vref = sensorValue2 * (4.096 / 1023.0); // Convert the Vref sensor reading to volts
  vpd = sensorValue3 * (4.096 / 1023.0); // Convert the Vpd sensor reading to volts
  buck_vin = sensorValue1 * (4.096 / 1023.0);
 
  // The inductor current is in mA from the sensor so we need to convert to amps.
  // We want to treat it as an input current in the Boost, so its also inverted
  // For open loop control the duty cycle reference is calculated from the sensor
  // differently from the Vref, this time scaled between zero and 1.
  // The boost duty cycle needs to be saturated with a 0.33 minimum to prevent high output voltages
 
  iL = -current_mA / 1000.0;
  dutyref = saturation(sensorValue2 * (1.0 / 1023.0), 0.99, 0.33);
 
 
}
 
float saturation( float sat_input, float uplim, float lowlim) { // Saturatio function
  if (sat_input > uplim) sat_input = uplim;
  else if (sat_input < lowlim ) sat_input = lowlim;
  else;
  return sat_input;
}
 
void pwm_modulate(float pwm_input) { // PWM function
  analogWrite(6, (int)(255 - pwm_input * 255));
}
 
 
void setup() {
 
  Wire.begin(); // We need this for the i2c comms for the current sensor
  Wire.setClock(700000); // set the comms speed for i2c
  ina219.init(); // this initiates the current sensor
  Serial.begin(9600); // USB Communications
 
 
  //Check for the SD Card
  Serial.println("\nInitializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("* is a card inserted?");
    while (true) {} //It will stick here FOREVER if no SD is in on boot
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }
 
 
  //Basic pin setups
  //Analogue input, the battery voltage (also port B voltage)
  pinMode(A0, INPUT);
  pinMode (A6, OUTPUT);
  pinMode (A7, INPUT);
  pinMode (A3, INPUT);
 
  noInterrupts(); //disable all interrupts
  pinMode(13, OUTPUT);  //Pin13 is used to time the loops of the controller
  pinMode(3, INPUT_PULLUP); //Pin3 is the input from the Buck/Boost switch
  pinMode(2, INPUT_PULLUP); // Pin 2 is the input from the CL/OL switch
  analogReference(EXTERNAL); // We are using an external analogue reference for the ADC
 
  pinMode(6, OUTPUT); // This is the PWM Pin
 
  //LEDs on pin 7 and 8
  pinMode(7, OUTPUT); //error led
  pinMode(8, OUTPUT); //some other digital out
  // TimerA0 initialization for control-loop interrupt.
 
  TCA0.SINGLE.PER = 999; //
  TCA0.SINGLE.CMP1 = 999; //
  TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | TCA_SINGLE_ENABLE_bm; //16 prescaler, 1M.
  TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP1_bm;
 
  // TimerB0 initialization for PWM output
 
 
  TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm; //62.5kHz
  analogWrite(6, 120);
 
  interrupts();  //enable interrupts.
 
 
}
 
void loop() {
 
  if (loop_trigger == 1) { // FAST LOOP (1kHZ)
    pwm_modulate(open_loop); // write it out (inverting for the Buck here)
    int_count++; //count how many interrupts since this was last reset to zero
 
    loop_trigger = 0; //reset the trigger and move on with life
    Serial.print(int_count);
 
  }
 
  if (int_count == 500) { // SLOW LOOP (1Hz)
    Serial.println(" ");
    sampling();
    vbt = 2 * vin;
 
    iin = iL;
    vpdt = 2 * vpd;
   
   
    va = vpdt * 2.69697;
 
    //input power
    pt = iL * vbt;
 
    //desired output voltage
    vd = (pt + 52.166) / 11.482;
 
    if (vd<4.5){
      vd = 4.6;
    }
    if (vd > 5.2){
      vd = 5.1;
    }
 
    if (va > 5.2) {
      //relay off
      digitalWrite(A6, HIGH);
      open_loop = open_loop - 0.05;
    }
    if (va < 4.5) {
      //relay off
      digitalWrite(A6, HIGH);
      open_loop = open_loop + 0.05;
    }
    if ( 4.5 <= va <= 5.2) {
      //relay on
      digitalWrite(A6, LOW);
      open_loop = 1  + vbt/vd;
      vw = -vbt * (1/(1-open_loop));
     
 
    }
    if (open_loop > 1){
      open_loop = 0.99;
    }
    if (open_loop < 0 ){
      open_loop = 0.01;
    }
    pwm_modulate(open_loop);
    digitalWrite(13, LOW);   // reset pin13.
    int_count = 0;
 
 
 
 
 
 
 
 
 
 
    dataString = "Output v: " + String(vw) + " Input V: " + String(vb) + " input current: " + String(iL) + " input power: " + String(pt);
    dataString1 = String(vw) + " " + String(vbt) + " " + String(iin) + " " + String(open_loop) + " " + String(pt) + " " + String(vd) + " " + String(buck_vin*2);
    Serial.println(dataString1); // send it to serial as well in case a computer is connected
    File dataFile = SD.open("SD_Test.txt", FILE_WRITE); // open our CSV file
    if (dataFile) { //If we succeeded (usually this fails if the SD card is out)
      dataFile.println(dataString1); // print the data
    } else {
      Serial.println("File not open"); //otherwise print an error
    }
    dataFile.close(); // close the file
  }
}
 
ISR(TCA0_CMP1_vect) {
  TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_CMP1_bm; //clear interrupt flag
  loop_trigger = 1;
}
