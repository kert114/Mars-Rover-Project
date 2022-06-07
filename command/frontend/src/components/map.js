import {CanvasJSChart} from 'canvasjs-react-charts'
import React, { useEffect } from 'react'; // ES6 js
//bootstrap
import 'bootstrap/dist/css/bootstrap.min.css';
import axios from 'axios';
import { data } from 'jquery';
var Component = React.Component;

//Axios for get request
let json = require('../data.json');
console.log(json, 'data.json');


var xValue;
var yValue;
var length;
length = Object.keys(json).length;

//var x_val;
//va/r y_val;
//var length;
var dps = [];   //dataPoints.



export default class Map extends Component {
	
	constructor() {
		super();
		this.updateChart = this.updateChart.bind(this);
	}

	componentDidMount() {
		setInterval(this.updateChart, 1000);
	}
	// method 1
	/*const [database, datatoprocess] = useState([])
	useEffect(()=> {
		const fetchData = async () =>{
			const result = await fetch("http:localhost:3000/query.php")
			const jsonResult = await result.json();
			datatoprocess(jsonResult)
		}
		fetchData()
	}, [])
	*/     // method 2 
	updateChart() { // going to change later on. 
		//var yVal = Math.floor((Math.random()*100) + 1)
        //var xVal = Math.floor((Math.random()*100) + 1)
		for (let i=0;i<length; i++){
			xValue = parseFloat(json[i].xVal);
			yValue = parseFloat(json[i].yVal);
		/*	const getObstaclesx=()=>{
				axios.get('http://localhost:8000')
				.then(response=>{
					x_val = response.data[i].xValue;
				})
				.catch(err => {
					console.log(err);
				})
				
			}
			const getObstaclesy=()=>{
				axios.get('http://localhost:8000')
				.then(response=>{
					y_val = response.data.yValue;
				})
				.catch(err => {
					console.log(err);
				})
				
			}
			const send_len_dps=()=>{
				axios.get('http://localhost:8000')
				.then(response=>{
					length = response.data.length;
				})
				.catch(err => {
					console.log(err);
				})
				
			}
*/

			if(dps.length !== length && dps.length<length){
				dps.push({x: xValue ,y: yValue});
				//console.log(x_val);
				//console.log(y_val);
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