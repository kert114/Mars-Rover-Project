

import React from 'react';
//bootstrap
import 'bootstrap/dist/css/bootstrap.min.css';
import Data from '../data.json';
import axios from 'axios';
//Axios for get request
let json = require('../data.json');
console.log(json, 'data.json');
//setInterval(function(){"http://localhost/query.php".reload(true);}, 100);

axios.get("http://localhost/query.php")
.then(res=> console.log(res.data))
.catch(err => console.log(err));

export default function Drive(){
    return (
        
     Data.map( data =>{
         return (
             <div className='box'>
                {data.id}
                {data.Object}
                {data.xVal}
                {data.yVal}
             </div>
         )
     })
    );
}