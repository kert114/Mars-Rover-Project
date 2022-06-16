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
            document.getElementById("listings").innerHTML = "←";
        } 
        if (e.key === 'ArrowRight'){
        whatkey = "right";
        document.getElementById("listings").innerHTML = "→";
        }
        if (e.key === 'ArrowDown'){
            whatkey = "backwards";
            document.getElementById("listings").innerHTML = "↓";
        }
        if (e.key === 'ArrowUp'){
        whatkey = "forwards";
        document.getElementById("listings").innerHTML = "↑";
        }
        if(e.key === " "){
            whatkey = "stop";
            document.getElementById("listings").innerHTML = "Space Bar";
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
    return(
        <section>
        <div>Arrow Keys</div>
        <p>Presses are: </p>
        <p id="listings">  </p>
        
        </section>
    );
}

export default GetkeyPress;   
