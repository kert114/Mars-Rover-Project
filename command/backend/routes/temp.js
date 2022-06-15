const express = require('express');
const db = require('./mysql')

const router = express.Router();// the mpules you need to use.
let json = require('../data.json');
const fs = require('fs');
var x;

console.log(json);
console.log(json[0].xVal);
console.log(json[0].yVal);


router.get('/control', (req, res) => { //takes the request and the response. 
  var array = [json[0].xVal, json[0].yVal];

  res.send(array);

});

   
var array=[];
router.post('/directions', (req, res) =>{
   // Route for creating the post
  const username = req.body.direction;
  console.log(username);
  db.query("INSERT INTO flying (direction) VALUES (?)",[username], (err,result)=>{
     if(err) {
     console.log(err)
     res.status(400).send(e)

     } 
     console.log(result)
     res.status(201).send()


  });   })
    

router.post('/Newcontrol', (req, res) => {
    res.end('NA'); // not implemented yet.  
});

module.exports = router;