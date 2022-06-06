import React, { useState} from 'react';
//import RotateLeftIcon from '@material-ui/icons/RotateLeft';
//import RotateRightIcon from '@material-ui/icons/RotateRight';
import axios from 'axios';
import { useLocation } from 'react-router-dom';
import GetkeyPress from './mouseposition';

function Map(){

   
    const position=GetkeyPress();

    const handleClick=(event)=>{ 
        if (event.type === "mousedown") {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'L' }));
        axios.post('http://localhost:3000/drive', { 'direction':'L' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
        } else {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'S' }));
        axios.post('http://localhost:3000/drive', { 'direction':'S' } )
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
        console.log("Message sent: " + JSON.stringify({ 'direction':'F' }));
        axios.post('http://localhost:3000/drive', { 'direction':'F' })
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
        } else {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'S' }));
        axios.post('http://localhost:3000/drive', { 'direction':'S' } )
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
        console.log("Message sent: " + JSON.stringify({ 'direction':'B' }));
        axios.post('http://localhost:3000/drive', { 'direction':'B' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
        } else {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'S' }));
        axios.post('http://localhost:3000/drive', { 'direction':'S' } )
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
        console.log("Message sent: " + JSON.stringify({ 'direction':'R' }));
        axios.post('http://localhost:3000/drive', { 'direction':'R' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
        } else {
            event.preventDefault();
        console.log("Message sent: " + JSON.stringify({ 'direction':'S' }));
        axios.post('http://localhost:3000/drive', { 'direction':'S' } )
            .then(response=>{
                console.log(JSON.stringify(response.data));
            })
            .catch(err => {
                console.log("Received error: " + err);
            });
        }
    }

    const sendDest=(event)=>{
        event.preventDefault();
        let params={
            coordinateX: x_real,
            coordinateY: y_real
        }
        console.log("Message sent: " + JSON.stringify(params));
        axios.post('http://localhost:3000/drive', params) // not sure 
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
                    <LineTo from={i} to={i+1}/>
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
             <Grid item>Off</Grid>
             <Grid item>
            <AntSwitch onChange={RemoteControl} />
            </Grid>
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
}

export default Map;