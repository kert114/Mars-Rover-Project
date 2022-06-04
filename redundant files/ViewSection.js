import React from 'react';
import '../App.css';
import dynamicMap from './dynamicMap';


function ViewSection({path, pos, obstacle, stage}) {
    return (
        <div className='view-container'>
            <h1>Welcome to the View page</h1>
            <div className='mapping'>
                <dynamicMap roverPath={path} pos={pos} obstacle={obstacle} stage={stage}/>
            </div>
        </div>
    );
};

export default ViewSection;