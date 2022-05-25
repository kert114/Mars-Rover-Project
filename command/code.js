
var dps = [

];   //dataPoints. 

window.onload = function () {

    
    var chart = new CanvasJS.Chart("chartContainer",{
      zoomEnabled: true,
      panEnabled: true,
      
        title :{
            text: "Mapping of Arena"
        },
        axisX: {						
            title: "X domain"
        },
        axisY: {						
            title: "Y Domain"
        },
        data: [{
            type: "scatter",
            dataPoints : dps
        }]
    });

    chart.render();	
    var updateInterval = 1000;

    var updateChart = function () {
        
        xVal = Math.floor((Math.random() * 100));
        yVal =Math.floor((Math.random() * 100));
        dps.push({x: xVal,y: yVal});
        
        chart.render();		

  // update chart after specified time. 

};

setInterval(function(){updateChart()}, updateInterval); 
}


x = msg.split(":")
		tmp2 = (x[1]).split("; ") #x[0] = o # print(int(tmp2[0])) = 1 # tmp2[1] = 2 for txt = "o:1; 2"
		if x[0] == "o":
			msg_to_send = "o" + tmp2[0] + tmp2[1] # send as o12
		elif x[0] == "t":
			msg_to_send = "t" + tmp2[0] + tmp2[1] # send as o12
		elif x[0] == "f":
			msg_to_send = "f" + tmp2[0] + tmp2[1] # send as o12
		elif x[0] == "r":
			msg_to_send = "r" + tmp2[0] + tmp2[1] # send as o12
		if x[0] == "p":
			msg_to_send = "p" + tmp2[0] + tmp2[1] # send as p12 -> power supply underground
		