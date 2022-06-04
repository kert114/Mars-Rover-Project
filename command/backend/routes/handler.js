const express = require('express');
const router = express.Router();// the mpules you need to use. 

router.get('/control', (req, res) => { //takes the request and the response. 
    //if anyone goes to the tweets page, they wll get back a json response. 
    const str = [
        {
            "name": "Samantha Kai",
            "msg": "React JS is so simple!",
            "username": "samanthakai"
        },
        {
            "name": "John K",
            "msg": "Sweep the leg!",
            "username": "johnk"
        }
    ];
    res.end(JSON.stringify(str));
});

router.post('/control', (req, res) => {
    res.end('NA'); // not implemented yet.  
});

module.exports = router;