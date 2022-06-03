import {CanvasJSChart} from 'canvasjs-react-charts'
import React from 'react'; // ES6 js
var Component = React.Component;

//var CanvasJSReact = require('./canvasjs.react');
//var CanvasJSChart = CanvasJSReact.CanvasJSChart;

var dps = [{x: 1, y: 10}, 
            {x: 2, y: 1113}, 
            {x: -3, y: 18}, 
            {x: -4, y: 20}, 
            {x: -5, y: 17},
            {x: -6, y: -10},
            {x: 7, y: -13},
            {x: 8, y: 18},
            {x: 9, y: -20},
            {x: -10, y: -17}];   //dataPoints.

export default class Map extends Component {

	constructor() {
		super();
		this.updateChart = this.updateChart.bind(this);
	}

	componentDidMount() {
		setInterval(this.updateChart, 1000);
	}


	updateChart() { // going to change later on. 
		var yVal = Math.floor((Math.random()*100) + 1)
        var xVal = Math.floor((Math.random()*100) + 1)
		dps.push({x: xVal,y: yVal});
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