import React, { useState, useEffect } from 'react';

function GetkeyPress (){
    useEffect (()=>{
        document.addEventListener('keydown', detectKeydown, true)
    },[])
    const detectKeydown = (e) => {
        console.log("clicked key " , e.key);
    }
}

export default GetkeyPress;   