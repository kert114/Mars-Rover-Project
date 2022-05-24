var http = require('http');
let htmlContent = `
<!DOCTYPE HTML>
<html>
<head>
	<script type="text/javascript">
	window.onload = function () {

      var dps = [
  
      ];   //dataPoints. 

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
</script>
<script type="text/javascript" src="https://canvasjs.com/assets/script/canvasjs.min.js"></script>
</head>
<body>
	<div id="chartContainer" style="height: 300px; width: 100%;">
	</div>
</body>
</html>
`;


var server = http.createServer(function(req, res){
 res.writeHead(200, {'Content-Type':'text/html'})
 res.end(htmlContent);

});
console.log('Server is running on port 3000');
server.listen(3000,'127.0.0.1');