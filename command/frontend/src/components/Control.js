import axios from 'axios';
import React, {useEffect, useState} from 'react';// before the tweet was hard coded, now it is pulling an 
// displaying to the front end. 
import {Link} from 'react-router-dom'
import './Control.css';
var array = [];
async function Control() {
  const response = await axios.get("http://localhost:4000/control")
  return response.data;
}

export default Control;

