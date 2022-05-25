
var express = require('express');
var server = express();//server will now contain additional methods

var http = require('http');
let htmlContent = `
<!DOCTYPE HTML>
<html>
<head>

<script type="text/javascript" src="https://canvasjs.com/assets/script/canvasjs.min.js"></script>
<script type="text/javascript" src="code.js"></script>
</head>

<body>
	<div id="chartContainer" style="height: 300px; width: 100%;">
	</div>
</body>


</html>
`;


server.get('/', function(req, res) {
  res.sendFile('\\wsl$\Ubuntu-20.04\root\GSB2\command/code.js');
 });
 
var server = http.createServer(function(req, res){
 res.writeHead(200, {'Content-Type':'text/html'})
 res.end(htmlContent);

});
console.log('Server is running on port 3000');
server.listen(3000,'127.0.0.1');