import {CanvasJSChart} from 'canvasjs-react-charts'
import React from 'react'; // ES6 js
//bootstrap
import 'bootstrap/dist/css/bootstrap.min.css';
import Data from '../data.json';
import axios from 'axios';
import { data } from 'jquery';
var Component = React.Component;

//Axios for get request
let json = require('../data.json');
console.log(json, 'data.json');
//setInterval(function(){"http://localhost/query.php".reload(true);}, 100);


var xValue;
var yValue;
var length;
length = Object.keys(json).length;



console.log(length);

var dps = [];   //dataPoints.

export default class Map extends Component {
	
	constructor() {
		super();
		this.updateChart = this.updateChart.bind(this);
	}

	componentDidMount() {
		setInterval(this.updateChart, 1000);
	}
	updateChart() { // going to change later on. 
		//var yVal = Math.floor((Math.random()*100) + 1)
        //var xVal = Math.floor((Math.random()*100) + 1)
		for (let i=0;i<length; i++){
			xValue = parseFloat(json[i].xVal);
			yValue = parseFloat(json[i].yVal);
			if(dps.length !== length && dps.length<length){
				dps.push({x: xValue ,y: yValue});
				console.log(xValue);
				console.log(yValue);
			}
			console.log("length", length);
			console.log("length of dps " , dps.length );
		
		}
		
		console.log(dps);
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