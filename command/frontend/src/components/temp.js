import {CanvasJSChart} from 'canvasjs-react-charts'
import React, {useEffect, useState} from 'react';//bootstrap
import 'bootstrap/dist/css/bootstrap.min.css';
import axios from 'axios';
import Control from './Control';

var Component = React.Component;
var x_value;
var y_value;
var dps = [];   //dataPoints.


 
export default class Map extends Component {
	constructor() {
		super();
		this.updateChart = this.updateChart.bind(this);
	}

	componentDidMount() {
		setInterval(this.updateChart, 1000);
	}
	
	async updateChart() { // going to change later on. 
		const data = await Control();
		console.log("data ", data);
		console.log(data[0].xVal);
		dps.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal)});
		console.log("dps ", dps);
		this.chart.render();
	}
	render() {
		const options = {
			title :{
				text: "Mapping of Arena"
			},
			data: [{
				type: "scatter",
				dataPoints : dps
			}],

			axisY:{
				display:false,
				gridThickness :0,
				//lineThickness:0,
// need to display colours instead of random points based on id. 
			},
			axisX:{
				display:false,
				//lineThickness:0,
				gridThickness:0,
			}
            
		}
		return (
		<div>
			<CanvasJSChart options = {options}
				 onRef={ref => this.chart = ref}
			/>
			{/*You can get reference to the chart instance as shown above using onRef. This allows you to access all chart properties and methods*/}
		</div>
		);
	}
}