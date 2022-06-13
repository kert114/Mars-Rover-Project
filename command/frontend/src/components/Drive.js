import React, {useState, Component, useEffect} from 'react';
import axios from 'axios';
import './Drive.css';
import {Link} from 'react-router-dom';
import {Button} from './button';
import ToggleSwitch from './toggleswitch';
import Switch from '@material-ui/core/Switch';
import { withStyles } from '@material-ui/core/styles';
import { makeStyles } from '@material-ui/core/styles';




const AntSwitch = withStyles((theme) => ({
    root: {
      width: 28,
      height: 16,
      padding: 0,
      display: 'flex',
    },
    switchBase: {
      padding: 2,
      color: theme.palette.grey[500],
      '&$checked': {
        transform: 'translateX(12px)',
        color: theme.palette.common.white,
        '& + $track': {
          opacity: 1,
          backgroundColor: theme.palette.primary.main,
          borderColor: theme.palette.primary.main,
        },
      },
    },
    thumb: {
      width: 12,
      height: 12,
      boxShadow: 'none',
    },
    track: {
      border: `1px solid ${theme.palette.grey[500]}`,
      borderRadius: 16 / 2,
      opacity: 1,
      backgroundColor: theme.palette.common.white,
    },
    checked:{},
  }))(Switch);




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
        ManualMethod(true);
        axios.post('http://localhost:4000/directions', { 'direction': "automate"}) // if automate - then pathfind
            .then(response =>{
                console.log("received " + JSON.stringify(response));
            })
            .catch(err => {
                console.log("Received error: " + err);
            })
            console.log("sent");
        alert("The rover will start investigating!");
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
    function GetkeyPress(){
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
            if(e.key === " "){
                whatkey = "spacebar";
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
const [control, setControl] = useState(true);
    const SetVar=(event)=>{
       // event.preventDefault();// the default action will not happen
            // if(control===true){
            // setControl(!control);
            // event.preventDefault();
            GetkeyPress();
           // }
            // else{
            //  setControl(!control);
            // }
    }
    return (
        <div class="background">
            <div class="layer"></div>
        
        <div className='command-container'>
            <h1>Drive Commands</h1>
            <h4 className="automated header">Automated Driving</h4>
            <h4 className="manual header">Manual Driving</h4>
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
                        <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium' >
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
                <div>
                <AntSwitch onChange={SetVar()} />
                </div>
            </div>
        </div>
        </div>
    );
};

export default Drive;