import React, {useState, Component, useEffect} from 'react';
import axios from 'axios';
import './Drive.css';
import {Link} from 'react-router-dom';
import {Button} from './button';
import ToggleSwitch from './toggleswitch';
import Switch from '@material-ui/core/Switch';
import { withStyles } from '@material-ui/core/styles';


function Drive() {
    const [automated, ManualMethod] = useState(false);

    function sendAngle() {
        if(automated) {
            alert("Automated mode activated");
        } else {
            if(document.getElementById('angle').value === '') {
            } else {
                var enteredAngle = document.getElementById('angle').value;
                axios.post('http://localhost:4000/directions', { 'angle': enteredAngle})
                .then(response =>{
                    console.log("received " + JSON.stringify(response));
                })
                .catch(err => {
                    console.log("Received error: " + err);
                })
                console.log("sent");            }
        }
    }

    function sendDistance() {
        if(automated) {
            alert("Automated mode activated");
        } else {
            if(document.getElementById('distance').value === '') {
                alert('please enter distance');
            } else {
                var enteredDistance = document.getElementById('distance').value;
                axios.post('http://localhost:4000/directions', { 'direction': enteredDistance})
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
            }
        }
    }

    function automationMode() {
        if(automated){
            alert("already in automation mode");
        }
        else{
        ManualMethod(true);
        axios.post('http://localhost:4000/directions', { 'direction': "automate"}) // if automate - then pathfind
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
        alert("The rover will start investigating!");}
    }
    function goback() {
        axios.post('http://localhost:4000/directions', { 'direction': "0,0"})// base = 0,0
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
        alert("The rover will head back to the base.");
    }
    function stop() {
    axios.post('http://localhost:4000/directions', { 'direction': "stop"})
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
        ManualMethod(false);
        alert("Automated mode exited");
    }
    var whatkey;
    function GetkeyPressLeft(){
        if(automated){
            alert("Automated mode activated")
        }
        else{
            whatkey = "left";
            console.log("whatkey value in keypress is before ", whatkey);
        
            axios.post('http://localhost:4000/directions', { 'direction': whatkey})
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
            var whatkey;
        }
    }
    function GetkeyPressRight(){
        if(automated){
            alert("Automated mode activated")
        }
        else{
            whatkey = "right";
            console.log("whatkey value in keypress is before ", whatkey);
        
            axios.post('http://localhost:4000/directions', { 'direction': whatkey})
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
            var whatkey;
        }
    }
    
    function GetkeyPressTop(){
        if(automated){
            alert("Automated mode activated")
        }
        else{
            whatkey = "forwards";
            console.log("whatkey value in keypress is before ", whatkey);
        
            axios.post('http://localhost:4000/directions', { 'direction': whatkey})
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
            var whatkey;
        }
    }
    function GetkeyPressDown(){
        if(automated){
            alert("Automated mode activated")
        }
        else{
            whatkey = "backwards";
            console.log("whatkey value in keypress is before ", whatkey);
        
            axios.post('http://localhost:4000/directions', { 'direction': whatkey})
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
            var whatkey;
        }
    }

    return (
        <section>
        <div className='image'></div>
        <div class="background">
            
        <div className='command-container'>
            <h1>Drive Commands</h1>
            <h4 className="automated header">Automated Driving</h4>
            <h4 className="manual header">Manual Driving</h4>
            <h4 className="arrow header">Arrow Key Driving</h4>
            <div className='command-btns'>
                <div className='automationMode'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={automationMode}>
                        automationMode!
                    </Button>
                </div>
                <div className='returntobase'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={goback}>
                        Return to base
                    </Button>
                </div>
                <div className='stop'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={stop}>
                        Stop
                    </Button>
                </div>
                <div className='viewmap'>
                    <Link to='/map'>
                        <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium'>
                            View map
                        </Button>
                    </Link>
                </div>
                <label for='angle' name='angle' className='angleLabel'><b> Angle (°): </b></label>
                <input type='number' id='angle' name='angle' className='angleBox'/>
                <label for='distance' name='distance' className='distanceLabel'> <b>Distance (mm): </b></label>
                <input type='number' id='distance' name='distance' className='distanceBox'/>
                <div className='sendAngle'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={sendAngle}>
                        Send
                    </Button>
                </div>
                <div className='sendDistance'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={sendDistance}>
                        Send
                    </Button>
                </div>
                
                <div className='leftarrow'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={GetkeyPressLeft}>
                    ←
                    </Button>
                </div>
                <div className='rightarrow'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={GetkeyPressRight}>
                    →
                    </Button>
                </div>
                <div className='toparrow'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={GetkeyPressTop}>
                    ↑
                    </Button>
                </div>
                <div className='bottomarrow'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={GetkeyPressDown}>
                    ↓
                    </Button>
                </div>   
                <div className='stop_arrowkey'>
                    <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' onClick={stop}>
                    🛑
                    </Button>
                </div>  

            </div>
            
        </div>
        
        </div>
        </section>
    );
};

export default Drive;