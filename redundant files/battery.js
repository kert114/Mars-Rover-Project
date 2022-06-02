import React from 'react';
import './Battery.css';

export const Battery = ({children, value}) => {
    const battery_number = parseInt(value, 10);
    var batteryLevel = 'unknown';
    switch(Math.ceil(battery_number/10)*10) {
        case 0:
            batteryLevel = '0';
            break
        case 10:
            batteryLevel = '10';
            break
        case 20:
            batteryLevel = '20';
            break
        case 30:
            batteryLevel = '30';
            break
        case 40:
            batteryLevel = '40';
            break
        case 50:
            batteryLevel = '50';
            break
        case 60:
            batteryLevel = '60';
            break
        case 70:
            batteryLevel = '70';
            break
        case 80:
            batteryLevel = '80';
            break
        case 90:
            batteryLevel = '90';
            break
        case 100:
            batteryLevel = '100';
            break
        default:
            batteryLevel = 'unknown';
    }
    return (
        <>
            <nav className='batteryform'>
                <div className='battery-container'>
                    <div className='status'>
                        <div className='statusinfo'>
                            <div className={`battery ${batteryLevel}`}></div>
                            <div className='batteryValue'>
                                Battery: {children}%
                            </div>
                        </div>
                    </div>
                </div>
            </nav>
        </>
    )
}