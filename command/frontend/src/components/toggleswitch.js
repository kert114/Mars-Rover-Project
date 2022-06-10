import React, { useState } from "react";
import "./toggleswitch.css";
  //  var input = document.getElementById('toggleswitch');
 //   var outputtext = document.getElementById('status');

  //  input.addEventListener('change',function(){
    //    if(this.checked) {
      //      outputtext.innerHTML = "aktiv";
    //    } else {
     //       outputtext.innerHTML = "inaktiv";
      //  }
 //   });
function ToggleSwitch() {
    const [isToggled, setIsToggled] = useState(false);
    const onToggle = () => setIsToggled(!isToggled);
    return (
      <label className="toggle-switch">
        <input type="checkbox" checked={isToggled} onChange={onToggle} />
        <span className="switch" />
      </label>
    );
}

export default ToggleSwitch;