import React from 'react';
import './Home.css';
import {BrowserRouter as Router, Routes, Route} from 'react-router-dom';


function Home() {
    return(
       <section>
       <section class="parent">
         
        <div class="flier"><img src={require("./rock-removebg-preview.png")} width="200" height="200"/></div>

          <div class="parent">
                    <div class="image_animation"><img src="https://www.freepnglogos.com/uploads/moon-png/moon-transparent-png-and-clip-art-images-icons-and-14.png" width="100" height="100" align="left"/></div>
         </div>
       
              <div class="container-fluid">
                <h1 class="mt-5"> Mars Rover GSB2 </h1>
                <p1><center>Created by Mahanoor, Angus, Kert, Floro, Anais, Maxime, Yusuf.</center></p1>
            </div>
           <div className='white'>
            <p2> This aim of this project is to create an autonomous Mars Rover that explores the terrain of Mars. Whilst exploring, it is to avoid aliens and their buildings and plot a map of their loations. This Mars Rover can also detect and map underground power stations.The mars rover interacts with the web server running in the background, providing information such as location of the encountered obstacles and its own location for the user. There are 3 pages to this website: Home (current loation), Map (to plot the locations of the obstacle) and Drive (to send driving instructions to the control unit). </p2>
            </div>
            
      
         
        

        </section>
         <div class="rover"><img src={require("./rover2.png")} width="200"/></div>
         </section>
    );
}

export default Home;