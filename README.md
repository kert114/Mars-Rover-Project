# GSB2

This repository contains the code for a fully integrated Mars River that was developed at the end of the 2nd year at Imperial College London. 

The aim of this project was to design, build and control an autonomous mars rover capable of exploring a terrain of a fixed size. 

The terrain contains objects with }}}represent aliens and their buildings, which the rover has to detect and send the coordinates of the objects as well as it's own position. 


This rover has 2 modes:
1. Autonomous driving 
2. Manual control using arrow keys input from user 


There are 5 aspects of this project. 

1. Command
A website was developed using react for frontend and NodeJs for the backend. 
The website contains 3 pages:

a. The home page which contains information about the rover and links all the other pages for the user to see. 

b. The Map is a scatter plot which receives information from the backend about the rovers and the objects coordinates and plots them using a colour code. 

c. The Modes page gives the user flexibility over how they want the rover to be controlled. 
Automation mode executes the path algorithm written in control. 
The manual control controls the rover using arrow key presses. This acts as a fail safe emulating real life conditions on mars

2. Control 

3. Drive 

4. Vision 

5. Energy

