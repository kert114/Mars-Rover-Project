const express = require('express');
const router = express.Router();// the mpules you need to use.
let json = require('../data.json');
var object = JSON.stringify(json);

var xValue;
var yValue;
router.get('/control', (req, res) => { //takes the request and the response. 
    //if anyone goes to the tweets page, they wll get back a json response. 
    
    res.end(JSON.stringify(json));
});

router.post('/Newcontrol', (req, res) => {
    res.end('NA'); // not implemented yet.  
});

module.exports = router;