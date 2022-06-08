import React, { useState, useEffect } from 'react';
import axios from 'axios';
//import useKeypress from 'react-use-keypress';

var whatkey;
function GetkeyPress (){
    useEffect (()=>{
        document.addEventListener('keydown', detectKeydown, true)
    },[])
    const detectKeydown = (e) => {
        console.log("clicked key " , e.key);
        if (e.key === 'ArrowLeft') {
            whatkey = "left";
        } 
        if (e.key === 'ArrowRight'){
        whatkey = "right";
        }
        if (e.key === 'ArrowDown'){
            whatkey = "backwards";
        }
        if (e.key === 'ArrowUp'){
        whatkey = "forwards";
        }
        console.log("whatkey value in keypress is before ", whatkey);
        axios.post('http://localhost:4000/directions', { 'direction': whatkey});

    }
}

export default GetkeyPress;   