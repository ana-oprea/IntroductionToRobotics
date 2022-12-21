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

## Matrix Game
### Components:
* LCD
* 8x8 LED matrix
* joystick
* resistors
* wires
* buzzer
* 100k ohm resistor
* 100 ohm resistor

### Technical Task
Create a menu for your game, emphasis on the game. You should scroll on the LCD with the joystick. Remember you have quite a lot of flexibility here, but do not confuse that with a free ticket to slack off. The menu should include the following functionality:

1. When powering up a game, a greeting message should be shown for a few moments.
2. Should contain roughly the following categories:
  a) Start game, starts the initial level of your game
  b) Highscore:
    * Initially, we have 0.
    * Update it when the game is done. Highest possible score should be achieved by starting at a higher level.
    * Save the top 5+ values in EEPROM with name and score.
  c) Settings:
    * Enter name. The name should be shown in highscore. Maybe somewhere else, as well?
    * Difficulty
    * LCD contrast control
    * LCD brightness control (mandatory, must change LED wire that’s directly connected to 5v)
    * Matrix brightness control (see function setIntesnity from the ledControl library)
    * Sounds on or off
    * Extra stuff can include items specific to the game mechanics, or other settings such as chosen theme song etc. Again, save it to eeprom.
  d) About: should include details about the creator(s) of the game. At least game name, author and github link or user (use scrolling text?)
  e) How to play: short and informative description
3. While playing the game: display all relevant info
  * Lives
  * Score
4. Upon game ending:
  * Screen 1: a message such as ”Congratulations on reaching level/score X”. ”You did better than y people.”. etc. Switches to screen 2 upon interaction (button press) or after a few moments.
  * Screen 2: display relevant game info: score, time, lives left etc. Must inform player if he/she beat the highscore. This menu should only be closed by the player, pressing a button.

### Implementation Details
When you power the project, you're prompted to enter your name (3 letters), move the joystick up and down to change letters, once you have all 3 letters, press the joystick to confirm. After that, a short message with "Welcome to snake" is gonna show up, then the main menu opens.

The main menu has 5 options:
* Start game: play the game
* Settings: make changes to LCD contrast/brightness, matrix brightness, difficulty, sounds (ON/OFF)
* High scores: see the top 5 players and their score
* How to play: a brief description of the game and how to play it
* About: information about the developer

## Navigation
* Moving through the menu is done by moving the joystick up and down to cycle the menu, to view an option, press the joystick. 
* If you are in a sub-menu (Start game, Settings, High scores, How to play, About) and want to exit, press the joystick. 
* If you are in Settings and want to change a value, move to that sub-section (LCD contrast/brightness, matrix brightness, difficulty, sounds) and an arrow should be pointing to the option you want to change, from there just move the joystick left and right to change the value, any change you make is going to be saved. If you want to exit, press the joystick and you are in the main manu again.
* If you are in game and want to exit, press the button and that will take you to the main menu.
* If you are at the end of the game (you lost) a brief "Game over" and your score will be shown on the lcd, after a short period you are met with the message "Try again" if you didn't make top 5, or with "You made top 5" if you managed your way on to the high score board. Press the button to return to the main menu.

### Picture of the setup (updated)
![46282895-7cbe-4830-bdf6-6f28d0b9ae2c](https://user-images.githubusercontent.com/80201759/208938267-9c9d5e91-2539-4902-8b23-efe7afa9806d.jpeg)

### Video
https://youtu.be/aGQtVZvUgOo
### Updated Video
https://www.youtube.com/watch?v=FRJVdvrxxxs
