const express = require('express');
const db = require('./mysql')
const dp2 = require('./mysql2')

const router = express.Router();// the mpules you need to use.
let json = require('../data.json');
var object = JSON.stringify(json);
const fs = require('fs');

var xValue;
var yValue;
// router.get('/control', (req, res) => { //takes the request and the response. 
//     //if anyone goes to the tweets page, they wll get back a json response. 
//     const id = req.params.id;
//     db2.query("SELECT LAST(Object, xVal, yVal) FROM esp32data WHERE id = ?", id, 
//     (err,result)=>{
//         if(err) {
//         console.log(err)
//         } 
//         res.send(result)
//         });   });
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