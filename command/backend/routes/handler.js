const express = require('express');
const db = require('./mysql')
const db2 = require('./mysql2')
const router = express.Router();// the mpules you need to use.
let json = require('../data.json');
const fs = require('fs');
var x;


router.get('/control', (req, res) => { //takes the request and the response. 
   const id = req.params.id;
   db2.query("SELECT * FROM esp32data ORDER BY id DESC LIMIT 1", id, 
   (err,result)=>{
      if(err) {
      console.log(err)
      } 
      console.log(result);
      res.send(result)
      });   });


   
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