import useKeypress from 'react-use-keypress';

const Example = (props) => {
  // ...
  useKeypress(['ArrowLeft', 'ArrowRight'], (event) => {
    if (event.key === 'ArrowLeft') {
      moveLeft();
    } else {
      moveRight();
    }
  });
  // ...
};