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
`;


var server = http.createServer(function(req, res){
 res.writeHead(200, {'Content-Type':'text/html'})
 res.end(htmlContent);
});
console.log('Server is running on port 3000');
server.listen(3000,'127.0.0.1');
