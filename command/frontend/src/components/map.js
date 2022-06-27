import {CanvasJSChart} from 'canvasjs-react-charts'
import React, {useEffect, useState} from 'react';//bootstrap
import 'bootstrap/dist/css/bootstrap.min.css';
import axios from 'axios';
import Control from './Control';
import './map.css';

var Component = React.Component;
//2337 x 3555
var dps = [];   //dataPoints.
var variable;
//var dps_line = [{ x: 0, y: 0 },{ x: 0, y: 2337 }, { x: 3555, y: 2337 },{ x: 3555, y: 0 }, { x: 0, y: 0 }]
var rover_position = [];
var radar =[];
var building = [];
export default class Map extends Component {
	constructor() {
		super();
		this.updateChart = this.updateChart.bind(this);
		//this.state = dps;
	}

	componentDidMount() {
		setInterval(this.updateChart, 1000);
	}
	
	async updateChart() { // going to change later on. 
		//var new_data = [];
		const data = await Control();
		console.log("data ", data);
		console.log(data[0].xVal);
		if(data[0].Object === "fuchsia" ){
				dps.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal), markerColor: "#ff00ff" });
		}
		else if(data[0].Object === "teal"){
			dps.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal), markerColor: "#008080" });

		}
		else if(data[0].Object === "pink"){
			dps.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal), markerColor: "#FF1493" });

		}
		else if(data[0].Object === "dark"){
			dps.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal), markerColor: "green" });

		}
		else if(data[0].Object === "building"){
			building.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal), markerColor: "black"});

		}
		else if(data[0].Object === "rover"){
			rover_position.shift();
			rover_position.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal), markerColor: "black" })
		}
		else if(data[0].Object === "radar"){
			radar.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal), markerColor: "grey" });
		}
		else{
			dps.push({x: parseFloat(data[0].xVal) ,y: parseFloat(data[0].yVal), markerColor: data[0].Object});
		}
	
		variable = data[0].Object;
		console.log("dps ", dps);
		console.log("rover pos ", rover_position);
		this.chart.render()	;
		console.log(variable);

	}
	render() {
		const options = {
			backgroundColor: null,
			visible :true,
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
				showInLegend: true,
				legendText: "aliens",
				legendMarkerColor: "black",
				
			}, 
			{
				type: "scatter",
				dataPoints: rover_position,
				markerType:"cross",
				markerSize: 10,
				markerColor: "black",
				showInLegend: true,
				legendText: "rover pos",
				legendMarkerColor: "black"
			}, 
			{
				type: "scatter",
				dataPoints: building,
				markerType: "square",
				markerSize: 20,
				markerColor: "black",
				showInLegend: true,
				legendText: "building",
				legendMarkerColor: "black"
			},
			{
				type: "scatter", 
				dataPoints: radar, 
				markerType:"triangle",
				showInLegend: true,
				legendText: "underground power station",
				markerSize: 20, 
				fontColor: "black",
				legendMarkerColor: "black"

			}
		],
			
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
				minimum: -10.0,
				maximum: 355.5,
				//interval: 10,
				lineThickness: 0,
				tickThickness: 0
			},
			axisY:{
				minimum: -10.0,
				maximum:233.7,
				//interval: 10,
				lineThickness: 0,
				gridThickness: 0,
				tickLength: 0
			},

			// data: [{
			// 	type: "scatter",
			// 	dataPoints : rover_position,
			// }],
			
            
		}
		return (
		<section>

			<div className='image3'></div>
			<div className='image2'></div>
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