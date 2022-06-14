import React from 'react';
import './Home.css';
import {BrowserRouter as Router, Routes, Route} from 'react-router-dom';


function Home() {
    return(
       
       <section class="parent">
          <div class="parent">
                    <div class="image_animation"><img src="https://www.freepnglogos.com/uploads/moon-png/moon-transparent-png-and-clip-art-images-icons-and-14.png" width="100" height="100" align="left"/></div>
         </div>
       
              <div class="container-fluid">
                <h1 class="mt-5"> Mars Rover GSB2 </h1>
                <p1><center>Created by Mahanoor, Angus, Kert, Floro, Anais, Maxime, Yusuf.</center></p1>
            </div>
            <p2> This aim of this project is to create an autonomous Mars Rover that explores the terrain </p2>
            <p3>of Mars. Whilst exploring, it is to avoid aliens and their buildings and plot a map of their</p3>
            <p4>loations. This Mars Rover can also detect and map underground power stations. </p4>
            <div2 class="parent2">
         <div class="hello"><img src={require("./rover2.png")} width="200"/></div>
         </div2>
        

        </section>
    );
}

export default Home;