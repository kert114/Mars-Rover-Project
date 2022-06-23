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
unsigned int sensorValue0,sensorValue1,sensorValue2,sensorValue3;
unsigned int loopTrigger;
float vpd, vb, vref, iLk=0, iL, dutyref, current_mA, pk, deltap, deltav, dp, dv,va;
float open_loop = 0.5;
float vbk = 0;
float pk1 = 0;
float p;
float current_limit;
boolean Boost_mode = 0;
boolean CL_mode = 0;
unsigned int int_count = 0;
String dataString1;
unsigned int com_count=0;



void setup() {

  //Basic pin setups
  
  noInterrupts(); //disable all interrupts
  pinMode(13, OUTPUT);  //Pin13 is used to time the loops of the controller
  pinMode(3, INPUT_PULLUP); //Pin3 is the input from the Buck/Boost switch
  pinMode(2, INPUT_PULLUP); // Pin 2 is the input from the CL/OL switch
  analogReference(EXTERNAL); // We are using an external analogue reference for the ADC

  // TimerA0 initialization for control-loop interrupt.
  
  TCA0.SINGLE.PER = 999; //
  TCA0.SINGLE.CMP1 = 999; //
  TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | TCA_SINGLE_ENABLE_bm; //16 prescaler, 1M.
  TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP1_bm; 

  // TimerB0 initialization for PWM output
  
  pinMode(6, OUTPUT);
  TCB0.CTRLA=TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm; //62.5kHz
  analogWrite(6,120); 

  interrupts();  //enable interrupts.
  Wire.begin(); // We need this for the i2c comms for the current sensor
  ina219.init(); // this initiates the current sensor
  Wire.setClock(700000); // set the comms speed for i2c
  Serial.begin(9600);
    //Check for the SD Card
  Serial.println("\nInitializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("* is a card inserted?");
    while (true) {} //It will stick here FOREVER if no SD is in on boot
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }
  
}

void sampling(){

  // Make the initial sampling operations for the circuit measurements
  
  sensorValue0 = analogRead(A1); //sample Vb
  sensorValue2 = analogRead(A2); //sample Vref
  sensorValue3 = analogRead(A3); //sample Vpd
  current_mA = ina219.getCurrent_mA(); // sample the inductor current (via the sensor chip)

  // Process the values so they are a bit more usable/readable
  // The analogRead process gives a value between 0 and 1023 
  // representing a voltage between 0 and the analogue reference which is 4.096V
  
  vb = sensorValue0 * (4.096 / 1023.0)*1.5; // Convert the Vb sensor reading to volts
  vref = sensorValue2 * (4.096 / 1023.0); // Convert the Vref sensor reading to volts
  vpd = sensorValue3 * (4.096 / 1023.0); // Convert the Vpd sensor reading to volts
  va=(vpd*890)/330;
  // The inductor current is in mA from the sensor so we need to convert to amps.
  // We want to treat it as an input current in the Boost, so its also inverted
  // For open loop control the duty cycle reference is calculated from the sensor
  // differently from the Vref, this time scaled between zero and 1.
  // The boost duty cycle needs to be saturated with a 0.33 minimum to prevent high output voltages
  
    iL = -current_mA/1000.0;
    //dutyref = saturation(sensorValue2 * (1.0 / 1023.0),0.99,0.33);
    //Serial.println(iL);

  
}

float saturation( float sat_input, float uplim, float lowlim){ // Saturatio function
  if (sat_input > uplim) sat_input=uplim;
  else if (sat_input < lowlim ) sat_input=lowlim;
  else;
  return sat_input;
}

void pwm_modulate(float pwm_input) { // PWM function

  analogWrite(6, (int)(pwm_input * 255));  // double check this is right
}


void loop() {

  if (loopTrigger == 1) { // FAST LOOP (1kHZ)
    //pwm_modulate(open_loop); // write it out (inverting for the Buck here)
    int_count++; //count how many interrupts since this was last reset to zero
    loopTrigger = 0; //reset the trigger and move on with lif
    //Serial.println("gone through1");

  }

   if (int_count == 1000) { // This loop is triggered, it wont run unless there is an interrupt

      digitalWrite(13, HIGH);   // set pin 13. Pin13 shows the time consumed by each control cycle. It's used for debugging.

      sampling();
 

          p = iL * vb; //input power
          pk = iLk * vbk;
          dp = p - pk;
          dv = vb - vbk;

          if (dp > 0) {
            if (dv > 0) {
              open_loop = open_loop - 0.02;
         
            } 
            else if (dv < 0) {
              open_loop = open_loop + 0.02;
       
            }
          }
          else if (dp < 0) {
            if (dv > 0 ) {
              open_loop = open_loop + 0.02;
            
            } else if (dv < 0){
              open_loop = open_loop - 0.02;
            
            }
          }
          
          if (open_loop > 1) {
            open_loop = 0.98;
          }
          if (open_loop < 0 ) {
            open_loop = 0.10;
          }
    
        pwm_modulate(open_loop);
        
        dataString1 =     " " + String(p)  + " " + String(vb) + " " + String(iL) + " " + String(open_loop);
        Serial.println(dataString1); // send it to serial as well in case a computer is connected
        File dataFile = SD.open("SD_Test.txt", FILE_WRITE); // open our CSV file
        if (dataFile) { //If we succeeded (usually this fails if the SD card is out)
          dataFile.println(dataString1); // print the data
        } else {
          Serial.println("File not open"); //otherwise print an error
        }
        dataFile.close(); // close the file
        vbk = vb;
        iLk = iL;
        int_count=0;
        digitalWrite(13, LOW);   // reset pin13.

  }
}


// Timer A CMP1 interrupt. Every 800us the program enters this interrupt. 
// This, clears the incoming interrupt flag and triggers the main loop.

ISR(TCA0_CMP1_vect){
  TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_CMP1_bm; //clear interrupt flag
  loopTrigger = 1;
}
