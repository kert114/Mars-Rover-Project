//import logo from './logo.svg';
import { useState } from 'react';
import $ from "jquery";
import './App.css';
import Nav from './components/Nav';
import Home from './components/Home';
import Control from './components/Control';
import Map from './components/map';
import {BrowserRouter as Router, Routes, Route} from 'react-router-dom';


function App() {
  return (
    <Router>
      <div className="App">
          <Nav />
          <Routes> 
            <Route path="/" exact element={<Home/>} />
            <Route path="/control" exact element={<Control/>} />
            <Route path="/map" exact element={<Map/>}/>
            
          </Routes>
          <img src="./rover.png" alt="" class="out-of-screen"></img>
      </div>
    </Router>

  );
}
export default App;