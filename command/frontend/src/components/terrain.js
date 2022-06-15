import path from "./images/path.png";
import blank from "./images/blank.png";
class TerrainLayer {
  static getLayer(terrainCells) {
    const keys = terrainCells.map(({ x, y }) => {
      return `${x}_${y}`;
    });

    const images = [];
    var colour = "green";
    terrainCells.forEach(({ x, y }) => {
      // const upperLeft = keys.includes(`${x}_${y}`);

      if (colour === "white") {
        images.push({
          src: blank,
          cellWidth: 1,
          cellHeight: 1,
          cellX: 1,
          cellY: 0
        });
      }
      if (colour === "green") {
        images.push({
          src: path,
          cellWidth: 1,
          cellHeight: 1,
          cellX: 2,
          cellY: 3
        });
      }
      // if (upperLeft) {
      //   images.push({
      //     src: fill,
      //     cellWidth: 1,
      //     cellHeight: 1,
      //     cellX: x,
      //     cellY: y
      //   });
      //   return;
      // }
    });

    return {
      images,
      text: []
    };
  }
}

export default TerrainLayer;
