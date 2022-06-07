const express = require('express');
const router = express.Router();// the mpules you need to use. 
let json = require('../data.json');
console.log(json, 'data.json'); // just to ensure we have data here. 

var xValue;
var yValue;
var length;
length = Object.keys(json).length;


router.get('/control', (req, res) => { //takes the request and the response. 
    //if anyone goes to the tweets page, they wll get back a json response. 

    res.end(JSON.stringify(json));
    res.send(JSON.stringify(json));
});

router.post('/control', (req, res) => {
    res.end('NA'); // not implemented yet.  
});

module.exports = router;