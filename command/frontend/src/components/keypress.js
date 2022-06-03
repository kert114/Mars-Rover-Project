import useKeypress from 'react-use-keypress';

const Example = (props) => {
  // ...
  useKeypress(['ArrowLeft', 'ArrowRight'], (event) => {
    if (event.key === 'ArrowLeft') {
      moveLeft();
    } 
    else if (event.key === 'ArrowRight'){
      moveRight();
    }
    else if (event.key === 'Arrow'){
        moveRight();
      }
    else if (event.key === 'ArrowRight'){
    moveRight();
    }
  });
  // ...
};