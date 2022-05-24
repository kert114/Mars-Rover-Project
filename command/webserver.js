var http = require('http');
let htmlContent = `
<!DOCTYPE html>
<html>
 <head>
 <title>Page Title</title>
 </head>
 <body style="background: linear-gradient(to bottom, #ffff99 0%, #00ffff 100%)">
 <h1 style="font-family:verdana;">This is a heading</h1>
 <p style="font-family:courier;">This is a paragraph.</p>
 </body>
</html>



<! scatter plots>
<html>
<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js"></script>
<body>
<canvas id="myChart" style="width:100%;max-width:700px"></canvas>

<script>
var xyValues = [
  {x:50, y:7},
  {x:60, y:8},
  {x:70, y:8},
  {x:80, y:9},
  {x:90, y:9},
  {x:100, y:9},
  {x:110, y:10},
  {x:120, y:11},
  {x:130, y:14},
  {x:140, y:14},
  {x:150, y:15}
];

new Chart("myChart", {
  type: "scatter",
  data: {
    datasets: [{
      pointRadius: 4,
      pointBackgroundColor: "rgb(0,0,255)",
      data: xyValues
    }]
  },
  options: {
    legend: {display: false},
    scales: {
      xAxes: [{ticks: {min: 40, max:160}}],
      yAxes: [{ticks: {min: 6, max:16}}],
    }
  }
});
</script>

</body>
</html>

`;


var server = http.createServer(function(req, res){
 res.writeHead(200, {'Content-Type':'text/html'})
 res.end(htmlContent);

});
console.log('Server is running on port 3000');
server.listen(3000,'127.0.0.1');
