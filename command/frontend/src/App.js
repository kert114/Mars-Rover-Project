//import logo from './logo.svg';
import { useState } from 'react';
import $ from "jquery";
import './App.css';
import Nav from './components/Nav';
import Home from './components/Home';
import Map from './components/map';
import Drive from './components/Drive'
import {BrowserRouter as Router, Routes, Route} from 'react-router-dom';


function App() {
  return (
    <Router>
      <div className="App">
          <Nav />
          <Routes> 
            <Route path="/" exact element={<Home/>} />
            <Route path="/map" exact element={<Map/>}/>
            <Route path='/drive' exact element={<Drive/>}/>
            
          </Routes>
      </div>
    </Router>

  );
}
export default App;