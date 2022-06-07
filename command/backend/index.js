const express = require('express');


const bodyParser = require('body-parser');
const routesHandler = require('./routes/handler.js');
//app.use(bodyParser.urlencoded({extended:false}));
//app.use(bodyParser.json());
app.use('/', routesHandler); 


const app = express();

app.get("/", function(req, res){
    res.send("express app here");
})
//.callback fns 
// get request = frontend to the backend. 
// send to backend = post request. 
const PORT = 8000; // backend routing port need to be a different port. 
app.listen(PORT, function() {
    console.log(`Server is running on port ${PORT}.`);
})
/*
// To read the json data at the backend instead of the frontend
let json = require('./data.json');
console.log(json, 'data.json');
var xValue;
var yValue;
var length;

function getObstaclesx(res) {
    xValue = parseFloat(json[i].xVal);
    res.send(xValue);
}

function getObstaclesy(res) {
    yValue = parseFloat(json[i].yVal);
    res.send(yValue);
}
function send_len_dps(res){
    length = Object.keys(json).length;
    res.send(length);
}



*/