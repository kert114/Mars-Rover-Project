import "./styles.css";
import React, { useState } from "react";
import { Canvas } from "@bucky24/react-canvas";
import { Map } from "@bucky24/react-canvas-map";
import TerrainLayer from "./TerrainLayer";

function App() {
  const width = 500;
  const height = 500;
  var dps;
  const [terrainCells, setCells] = useState(
    (dps = [
      { x: 1, y: 1 },
      { x: 1, y: 2 },
      { x: 3, y: 2 },
      { x: 3, y: 4 },
      { x: 2, y: 3 }
    ])
  );
  //  setTheArray([dps, { x: 2, y: 3 }]);
  return (
    <div>
      <Canvas width={width} height={height}>
        <Map
          x={50}
          y={50}
          width={width - 100}
          height={height - 100}
          cellSize={60}
          xOff={0}
          yOff={0}
          mapBackground={{
            color: "#00f"
          }}
          layers={[TerrainLayer.getLayer(terrainCells)]}
        />
      </Canvas>
    </div>
  );
}

export default App;
