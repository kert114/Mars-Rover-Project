import React, { useState, useEffect } from 'react';

const PositionMouse=()=>{

    const [position, setPosition]=  useState([
        {x: null, y: null}
    ])
    const onMouseMove=(event)=>{
        setPosition({
        x:event.clientX, 
        y:event.clientY
        });
    };
    
    useEffect(()=>{
        window.addEventListener("mousemove", onMouseMove);
    return()=> window.removeEventListener("mousemove",onMouseMove);},[]);
    return position;
};

export default PositionMouse;