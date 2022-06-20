import React, { useState} from 'react';
import './Map2.css';
import RotateLeftIcon from '@material-ui/icons/RotateLeft';
import RotateRightIcon from '@material-ui/icons/RotateRight';
import axios from 'axios';
import Switch from '@material-ui/core/Switch';
import Grid from '@material-ui/core/Grid';
import Typography from '@material-ui/core/Typography';
import {makeStyles } from '@material-ui/core/styles';
import { withStyles } from '@material-ui/core/styles';
import { useLocation } from 'react-router-dom';
import MousePosition from './mousePos';


function Map2(){

    if(useLocation().pathname==="/controller"){
        document.getElementById('pinkball').style.display="block";
        document.getElementById('blueball').style.display="block";
        document.getElementById('greenball').style.display="block";
        document.getElementById('orangeball').style.display="block";
        document.getElementById('blackball').style.display="block";
        document.getElementById('start').style.display="block";
        document.getElementById('reset').style.display="block";
        document.getElementById('i1').style.display="block";
        document.getElementById('msg').style.display="block";
        document.getElementById('info').style.display="block";
        document.getElementById('pinkballcoord').style.display="block";
        document.getElementById('greenballcoord').style.display="block";
        document.getElementById('orangeballcoord').style.display="block";
        document.getElementById('blueballcoord').style.display="block";
        document.getElementById('blackballcoord').style.display="block";
        
      }
    const position=MousePosition();
    //SetTimeout : calls a function or runs some code after a period of time, specified using the second argument (in milliseconds)

    const handleClick=(event)=>{ 
        if (event.type === "mousedown") {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'left' }));
        axios.post('http://localhost:4000/direction', { 'direction':'left' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
        } else {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'' }));
        axios.post('http://localhost:4000/direction', { 'direction':'' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            });
        }
    }
    const handleClick2=(event)=>{
        if (event.type === "mousedown") {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'forwards' }));
        axios.post('http://localhost:4000/direction', { 'direction':'forwards' })
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
        } else {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'' }));
        axios.post('http://localhost:4000/direction', { 'direction':'' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            });
        }
    }
    const handleClick3=(event)=>{
        if (event.type === "mousedown") {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'backwards' }));
        axios.post('http://localhost:4000/direction', { 'direction':'backwards' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
        } else {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'' }));
        axios.post('http://localhost:4000/direction', { 'direction':'' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            });
        }
    }
    const handleClick4=(event)=>{
        if (event.type === "mousedown") {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'right' }));
        axios.post('http://localhost:4000/direction', { 'direction':'right' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
        } else {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'' }));
        axios.post('http://localhost:4000/direction', { 'direction':'' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            });
        }
    }


    //hold down the button 

    const sendDest=(event)=>{
        event.preventDefault();
        let params={
            coordinateX: x_real,
            coordinateY: y_real
        }
        console.log("Message sent: " + JSON.stringify(params));
        axios.post('http://localhost:4000/direction', params)
            .then(response=>{
                console.log("Received message: " + JSON.stringify(response.data));
                //points is array of coordinates x and y for optimal path
                for( var i in response.data.points){
                    var x_disp= Math.floor(825+(response.data.points[i].x)*300/1000);
                    var y_disp=Math.floor(420-(response.data.points[i].y)*300/1000);
                    
                    document.getElementById("PAF").innerHTML= "Point "+ i + ": [" + response.data.points[i].x + ";" + response.data.points[i].y + "] " +"to get to destination <br/>";  
                    document.getElementById(i).style.left= x_disp + "px"; //x axis update
                    document.getElementById(i).style.top= y_disp + "px";  
                    document.getElementById(i).style.display="block"; 
                    
               }   
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
           }
    
     var x_real = Math.round((position.x-850)/300*1000);
     var y_real = -Math.round((position.y-445)/300*1000);
    
    return(
        <nav className="div">
            <img src={grid} alt="map" className="grid" onClick={sendDest}/>
            <img src={ground} alt="ground" className="ground"/>
            <h1 className="header"> Remote Control </h1> 
            <Typography component="div" className={classes.typography1}>
            <Grid component="label" container alignItems="center" spacing={1}>
            
            <Grid item>On</Grid>
            </Grid>
            </Typography>
            <div className="div">
            <button className="leftrotate" onMouseDown={handleClick} onMouseUp={handleClick} >
           <RotateLeftIcon/>
            </button> 
            <button className="uparrow"  onMouseDown={handleClick2} onMouseUp={handleClick2}  >
           <i class="fas fa-angle-up"></i>
            </button> 
            <button className="downarrow"  onMouseDown={handleClick3} onMouseUp={handleClick3}   >
           <i class="fas fa-angle-down"></i>
            </button> 
            <button className="rightrotate" onMouseDown={handleClick4} onMouseUp={handleClick4} >
           <RotateRightIcon/>
            </button>
            <img src={circle} alt="circle" style={{marginLeft:"163px", marginTop:"31px", width:"175px", height:"175px"}}/>
            <h4 style={{marginLeft: "10px", marginTop: "100px", position:"absolute"}}>
                Click on position
            </h4>
            <div style={{marginLeft: "10px", marginTop: "140px", position: "absolute"}} > 
            [{x_real}:{y_real}]
            </div>
            <div id="PAF" style={{marginLeft: "10px", marginTop: "170px", position: "absolute"}}></div>
            </div>
         </nav>
    );
}a

export default Map2;