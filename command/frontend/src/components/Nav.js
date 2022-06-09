import React from 'react'; // ES6 js
import {Link} from 'react-router-dom';

function Nav() {
    return(
        <nav class="navbar navbar-expand-lg navbar-dark  top">
            <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navMainMenu" aria-controls="navMainMenu" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div id="navMainMenu" class="navbar-collapse collapse">
                <div class="navbar-nav ml-auto">
                    <Link to='/' className="nav-item nav-link active">Home</Link>
                    <Link to="/map" className='nav-item nav-link'>Map</Link>
                    <Link to="/drive" className='nav-item nav-link'>Drive</Link>
                    <Link to="/Control" className='nav-item nav-link'>Control</Link>
                    <Link to="/mouseposition" className='nav-item nav-link'>mouseposition</Link>
                </div>
            </div>
        </nav>
    );
}

export default Nav;