## Homework 3
### Hardware Components:
 - 1 7-segment display
 - 1 joystick
 - resistors and wires (as needed)

### Task Requirements:
 - Use the joystick to naturally change the position of the turned on segment.
 - The 7-segment display will have 2 stages:
     1. Current  position  blinking.   Can  use  the  joystick  to  move  fromone  position  to  neighbors.   Short  pressing  the  button  toggles  state2.  Long pressing the button in state 1 resets the entire display by turning all the segments OFF and moving the current position to the decimal point.
     4. The  currentsegment  stops  blinking,  adopting  the  state  of  the  segment  beforeselection (ON or OFF). Toggling the X (or Y, you chose) axis shouldchange  the  segment  state  from  ON  to  OFF  or  from  OFF  to  ON.Clicking the joystick should save the segment state and exit back tostate 1.

### Implementation:
 - Firstly, I connected one potentiometer to a one-color LED to understand how it works. 
 - Then I connected 3 potentiometeres and tested each one of them using the Serial.Write() function so that I can see that each potentiometer works properly.
 - I removed the one-color LED and placed the RGB one and I used 1 resistor for each color of the RGB LED. 
 - I placed a non-connected one-color LED next to each of the potentiometers to indicate the color the potentiometer refers to.
 - Finally, I successfully read the input from the potentiometers and redirected it to the LED.

### Resources:
 - Mostly, I used the data provided in the laboratories.

### Notes:
 - It was a lot of fun horsing around with Arduino.

### Setup:
<img src="https://user-images.githubusercontent.com/63961737/198081883-00c5223a-eb86-419a-b6d3-baaec2181342.jpeg" width="200">
Link video: https://www.youtube.com/shorts/FyiD2DNzpuM
