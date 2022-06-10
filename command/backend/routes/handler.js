const express = require('express');
const db = require('./mysql')

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
var array=[];
router.post('/directions', (req, res) =>{
   // Route for creating the post
  const username = req.body.direction;
  console.log(username);
  db.query("INSERT INTO flying (direction) VALUES (?)",[username], (err,result)=>{
     if(err) {
     console.log(err)
     } 
     console.log(result)
  });   })
    

router.post('/Newcontrol', (req, res) => {
    res.end('NA'); // not implemented yet.  
});

module.exports = router;