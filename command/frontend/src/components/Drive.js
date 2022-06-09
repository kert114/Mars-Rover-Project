import React, {useState, Component, useEffect} from 'react';
import axios from 'axios';
import './Drive.css';
import {Link} from 'react-router-dom';
import {Button} from './button';
import Socket from './socket';



function Drive() {
    const [automated, ManualMethod] = useState(false);

    function sendAngle() {
        if(automated) {
            alert("Automated mode activated");
        } else {
            if(document.getElementById('angle').value === '') {
            } else {
                var enteredAngle = document.getElementById('angle').value;
                Socket.emit("Angle", enteredAngle);
            }
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
                Socket.emit("Distance", enteredDistance);
            }
        }
    }

    function automationMode() {
        ManualMethod(true);
        Socket.emit("Command", "automation");
        alert("The rover will start investigating!");
    }
    function goback() {
        Socket.emit("Command", "back");
        alert("The rover will head back to the base.");
    }
    function stop() {
        Socket.emit("Command", "stop");
        ManualMethod(false);
        alert("Automated mode exited");
    }
    var whatkey;
    function GetkeyPress() {
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

    return (
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
                        <Button name="button" className='btns' buttonStyle='btn-outline' buttonSize='btn--medium'>
                            View the map
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
            </div>
        </div>
    );
};

export default Drive;