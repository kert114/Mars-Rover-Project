const express = require('express');
const router = express.Router();// the mpules you need to use.
let json = require('../data.json');
var object = JSON.stringify(json);
const fs = require('fs');

var xValue;
var yValue;
router.get('/control', (req, res) => { //takes the request and the response. 
    //if anyone goes to the tweets page, they wll get back a json response. 
    console.log(object);    
    res.end(object);
});
router.post('/directions', (req, res) =>{
    // request the rover directions LRFB
    let response = JSON.stringify(req.body);
    let usersjson = fs.readFileSync("user.json","utf-8");
    let users = JSON.parse(usersjson);
    users.push(req.body);
    usersjson = JSON.stringify(users);
    fs.writeFileSync("user.json",usersjson,"utf-8");
    let to_send = {
        "received": "yes"
    };
    res.send(to_send);
  //  fs.appendFileSync('user.json', usersjson, (err) => {
    //    if (err) {
      //      throw err;
       // }
        //console.log("JSON data is saved.");
    //});
  //  console.log(response);
    //fs.appendFileSync('./data.txt', response + ",", 'utf-8');
    
    
});

router.post('/Newcontrol', (req, res) => {
    res.end('NA'); // not implemented yet.  
});

module.exports = router;