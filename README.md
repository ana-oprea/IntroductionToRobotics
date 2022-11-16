# Introduction To Robotics (2022-2023)
Introduction to Robotics laboratory homeworks, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code and image files.

## Homework 1
### Components:
* RGB led
* 3 potentiometers
* 3 resistors
* wires

### Technical Task
Use a separate potentiometer in controlling each of the color of the RGB led (Red, Green and Blue). The control must be done with digital electronics.

### Implementation details
The values read from each potentiometer range from 0 to 1023, and each color of the led has values between between 0 and 255. Therefore we need to map the values from the potentiometer to those of the led.

### Picture of the setup
![WhatsApp Image 2022-10-22 at 18 49 44](https://user-images.githubusercontent.com/80201759/197349035-d32b38ad-daaa-48ca-9365-a6fc53777228.jpeg)

### Video
https://youtu.be/A-1eMlE3CVA

## Homework 2
### Components:
* 5 leds
* 6 resistors
* 1 pushbutton
* 1 buzzer
* wires

### Technical Task
Building  the  traffic  lights  for  a  crosswalk. You will use 2 LEDs to represent the traffic lights for people (red and green) and 3 LEDs to represent the traffic lights for cars (red, yellow and green).

### Implementation details
There are 4 states:
* State 1. (default state) green light for cars, red for people, no sounds, the state is changed by pressing the button.
* State 2. (initiated 8 seconds after pushing the button) yellow for cars, red for people, no sounds, duration: 3 seconds.
* State 3. (initiated at the end of state 2) red for cars, green for people, a beeping sound from buzzer at a constant interval, duraton: 8 seconds.
* State 4. (initiated at the end of state 3) red for cars, blinking green for people, a beeping sound from the buzzer at a constant interval, duration: 4 seconds.

### Picture of the setup
![WhatsApp Image 2022-11-02 at 3 14 58 AM](https://user-images.githubusercontent.com/80201759/199372824-b2537a50-7b86-4cc4-9e14-a9baa9e2c331.jpeg)

### Video
https://youtu.be/X8QO-E7MXTM

## Homework 3
### Components:
* 1 7-segment display
* 1 joystick
* resistors
* wires

### Technical Task
You will use the joystick to control the position of the segment and ”draw” on the display. The movement between segments should be natural (meaning they should jump from the current position only to neighbors, but without passing through ”walls”.

### Implementation details
There are 2 states:
* State 1. (default state) use joystick to move from one position to neighbors, short pressing the button toggles state 2. Long Pressign the button resets the entire display and moves the current position to the decimal point.
* State 2. (initiated after pushing the button) the current segment stops blinking, adopting the state of the segment before selection. Toggling the X axis should change the value of the segment state from ON to OFF and from OFF to ON. Clicking the joystick should save the segment state and exit back to state 1.

### Picture of the setup
![aef1db09-c35b-4083-ab9d-6db4906d367b](https://user-images.githubusercontent.com/80201759/200861220-3523a69a-fea9-4b48-85a5-e2f498ee8b4d.jpeg)

### Video
https://www.youtube.com/watch?v=S7BMqWEZUIY

## Homework 4
### Components:
* 4 digit 7-segment display
* 1 joystick
* 74hc595 shift register
* wires

### Technical Task
Use the joystick to move through the 4 digit 7 segment displays digits, press the button to lock in on the current digit and use the other axis to increment or decrement the number. Keep the button pressed to reset all the digit values and the current position to the first digit in the first state.

### Implementation details
There are 2 states:
* State 1. you can use a joystick axis to cycle through the 4 digits; using the other axis does nothing. A blinking decimal point showsthe current digit position. When pressing the button, you lock in onthe selected digit and enter the second state.
* State 2. in this state, the decimal point stays always on, no longer blinking and you can no longer use the axis to cycle throughthe 4 digits.   Instead, using the other axis, you can increment or decrement the number on the current digit IN HEX (aka  from  0 to F, as in the lab).  Pressing the button again returns you to the previous state.  Also, keep in mind that when changing the number, you must increment it for each joystick  movement - it  should  notwork continuosly increment if you keep the joystick in one position
* Reset:  toggled by long pressing the button only in the first state. When resetting, all the digits go back to 0 and the current position is set to the first (rightmost) digit, in the first state.

### Picture of the setup
![c0f1ac42-7955-4233-b171-1af1167ac920](https://user-images.githubusercontent.com/80201759/202220590-bddf4685-1fa2-4042-b037-eb1829689c28.jpeg)


### Video
https://www.youtube.com/watch?v=eMhBunDrKtI
