//import logo from './logo.svg';
import { useState } from 'react';
import $ from "jquery";
import './App.css';
import Nav from './components/Nav';
import Home from './components/Home';
import Map from './components/map';
import Drive from './components/Drive'
import Control from './components/Control';
import {BrowserRouter as Router, Routes, Route} from 'react-router-dom';
import React from 'react';


function App() {
  //setInterval(function(){"http://localhost:3000/drive".reload(true);}, 1000);
  //window.location.reload(true);
  return (
    <Router>
      
      <div className="App">
          <Nav />
          <Routes> 
            <Route path="/" exact element={<Home/>} />
            <Route path="/map" exact element={<Map/>}/>
            <Route path='/drive' exact element={<Drive/>}/>
            <Route path='/control' exact element={<Control/>}/>
           
          </Routes>
      </div>
    </Router>

  );
}
export default App;