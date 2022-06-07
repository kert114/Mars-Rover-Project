import React, {useEffect, useState} from 'react';// before the tweet was hard coded, now it is pulling an 
// displaying to the front end. 
import {Link} from 'react-router-dom'



import './Control.css';
function Control() {
  useEffect( () => {
      fetchItems();
  }, []);

  const [items, setItems] = useState([]);

  const fetchItems = async () => {
    const data = await fetch('/control'); // that's the control URL port 4000 from our handler. 
    const items = await data.json();
    setItems(items); 
    console.log(items);// sets the item that you're gonna use in the return funtion. 
    // It extracts it from the tweet then it embeds it in an item. This function is always gonna 
    // be the same except for the control section. 
    var array = JSON.parse(items);
    
  };

  return(
    <section>
        {
        items.map(item => (
            
            <h6 class="card-title">{item.id}</h6>
                    
        ))
        }
    </section>
);
}

export default Control;