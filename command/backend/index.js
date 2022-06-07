const express = require('express');
const app = express();
const bodyParser = require('body-parser');
const routesHandler = require('../backend/routes/handler');

app.use(bodyParser.urlencoded({extended:false}));
app.use(bodyParser.json());
app.use('/', routesHandler); 



app.get('/', function(req,res){
    res.send("express here");
})

const PORT = 4000; // backend routing port need to be adifferent port. 

app.listen(PORT, function() {
    console.log(`Server is running on port ${PORT}.`);
})