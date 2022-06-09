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
        if (whatkey == "forwards" || whatkey == "backwards" || whatkey == "right" || whatkey == "left"){
            axios.post('http://localhost:4000/directions', { 'direction': whatkey})
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
        }
        var whatkey;

    }
}

export default GetkeyPress;   
