import useKeypress from 'react-use-keypress';

const Example = (props) => {
  // ...
  useKeypress(['ArrowLeft', 'ArrowRight'], (event) => {
    if (event.key === 'ArrowLeft') {
      moveLeft();
    } 
    if (event.key === 'ArrowRight'){
      send string "L"
    }
    if (event.key === 'Arrow'){
        moveRight();
      }
    if (event.key === 'ArrowRight'){
    moveRight();
    }
  });
  // ...
};