## Homework 3
### Hardware Components:
 - 1 7-segment display
 - 1 joystick
 - resistors and wires (as needed)

### Task Requirements:
 - Use the joystick to naturally change the position of the turned on segment.
 - The 7-segment display will have 2 stages:
     1. (**State 1**) Current  position  blinking.   Can  use  the  joystick  to  move  from one  position  to  neighbors.   Short  pressing  the  button  toggles  state2.  Long pressing the button in state 1 resets the entire display by turning all the segments OFF and moving the current position to the decimal point.
     2. (**State 2**) The  current segment  stops  blinking,  adopting  the  state  of  the  segment  before selection (ON or OFF). Toggling the X (or Y, you chose) axis should change  the  segment  state  from  ON  to  OFF  or  from  OFF  to  ON.Clicking the joystick should save the segment state and exit back to state 1.

### Resources:
 - Mostly, I used the data provided in the laboratories.

### Notes:
 - It was a lot of fun horsing around with Arduino, but I could not finish all the tasks (stage 2).

### Setup:
<img src="https://user-images.githubusercontent.com/63961737/216772499-ebafaf66-50c3-4fab-88d6-6287c2768ac8.jpeg" width="200">

Link video: [https://www.youtube.com/shorts/FyiD2DNzpuM](https://www.youtube.com/shorts/iLVxLvaPUmM)
