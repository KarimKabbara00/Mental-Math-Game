# Mental-Math-Game
## A Player vs. Player Mental Math Game built with Arduino.

### Description: 

The game starts by first picking a difficulty. The levels are:
1. Easy
2. Challenging
3. Impossible

The possible operations are addition, subtraction, multiplication, and division.
The numbers are randomized for each player. The player must input the correct answer to gain a point.

When the game starts, the yellow LED is activated indicating that the player may enter their answer. 
If a player answers a question correctly, the green LED is activated, and a the buzzer plays a sound.
Similarly, if a player answers a question incorrectly, the red LED and the buzzer are activated. 

First player to 3 points wins the game! 

### Input tutorial:

Press a number, then press the * button to submit. Pressing C erases the last input.



Diagram: 
![Diagram](https://user-images.githubusercontent.com/44332803/207449036-07c5c159-a609-4117-ab8e-ed2d2ccf770a.png)
NOTE: The diagram includes a transistor (and its resistor) and a poteniometer. In this build, neither of those parts where needed. The keypad in the diagram is a 3x4 membrane whereas the code uses a 4x4 membrane.

Parts Used:

* 1x Arduino MEGA 2560

* 1x 4x4 Membrane Switch Module

* 1x LCD 1602 Module

* 1x Passive Buzzer

* 1x Green LED

* 1x Yellow LED

* 1x Red LED

* 4x 220 Ohm Resistor 

* A bunch of wires
