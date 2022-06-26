const express = require('express');
const db = require('./mysql');
const db2 = require('./mysql2');
const db3 = require('./mysql3');

const router = express.Router();// the mpules you need to use.

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

router.post('/mode', (req, res) =>{
   // Route for creating the post
   const username = req.body.mode;
   console.log(username);
   db3.query("INSERT INTO mode (mode) VALUES (?)",[username], (err,result)=>{
      if(err) {
      console.log(err)
      res.status(400).send(e)

      } 
      console.log(result)
      res.status(201).send()
   });   })

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
module.exports = router;