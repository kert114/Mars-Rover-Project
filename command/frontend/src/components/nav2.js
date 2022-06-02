import React from 'react'; // ES6 js
import {Link} from 'react-router-dom';

function Nav2() {
    return(
        <nav class="navbar">
            <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navMainMenu" aria-controls="navMainMenu" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div id="navMainMenu" class="navbar-collapse collapse">
                <div class="navbar-nav ml-auto">
                    <Link to="/map" className='nav-item nav-link'>Map</Link>
                </div>
            </div>
        </nav>
    );
}

export default Nav2;