import {CanvasJSChart} from 'canvasjs-react-charts'
import React, {useEffect, useState} from 'react';//bootstrap
import 'bootstrap/dist/css/bootstrap.min.css';
import axios from 'axios';
import Control from './Control';
import path from './images/path.png';
import './map.css';

var Component = React.Component;
//2337 x 3555
var dps = [];   //dataPoints.
var variable;
var dps_line = [{ x: 0, y: 0 },{ x: 0, y: 2337 }, { x: 3555, y: 2337 },{ x: 3555, y: 0 }, { x: 0, y: 0 }]
 
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
		dps.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal), markerColor: data[0].Object});
		variable = data[0].Object;
		console.log("dps ", dps);
		this.chart.render();
		console.log(variable);

	}
	
	render() {
		const options = {
			backgroundColor: "white",
			////2337 x 3555
			zoomEnabled: true, 
			visible :true,

			zoomType: "xy",
			width: 1260,
			height: 600,
			position: "center",
			title :{
				text: "Mapping of the Arena", 
			},
			data: [{
				type: "scatter",
				dataPoints : dps,
				markerType : "circle",
				markerSize: 20,
				
			}],

			// axisX:{
			// 	minimum: -200,
			// 	maximum: 3555,
			// 	interval:10,
			// 	gridThickness: 0,
			// 	tickLength: 0,
			// 	lineThickness: 0,
			// 	labelFormatter: function(){
			// 	  return " ";
			// 	}
			//   },
			//   axisY:{
			// 	minimum: -200,
			// 	maximum:2337,
			// 	interval: 10,
			// 	gridThickness: 0,
			// 	tickLength: 0,
			// 	lineThickness: 0,
			// 	labelFormatter: function(){
			// 	  return " ";
			// 	}
			//   },
			axisX:{
				minimum: -200,
				maximum: 3555,
				//interval: 10,
				lineThickness: 0,
				tickThickness: 0
			},
			axisY:{
				minimum: -200,
				maximum:2337,
				//interval: 10,
				lineThickness: 0,
				gridThickness: 0,
				tickLength: 0
			},

			// data: [{
			// 	type: "line",
			// 	dataPoints : dps_line,
			// 	markerType : null
			// }],

            
		}
		return (
		<section>
			<div className='image'></div>
			<div>
				<CanvasJSChart options = {options}
					onRef={ref => this.chart = ref}
				/>
				  </div>
				{/*You can get reference to the chart instance as shown above using onRef. This allows you to access all chart properties and methods*/}
		</section>
		);
	}
}