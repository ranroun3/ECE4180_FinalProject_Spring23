# Monkey Type Videogame on MBED LPC1768
 Georgia Institute of Technology

ECE 4180 - Spring 2023

Creators: Rony Stephan, Andrew Wang

## Table of Contents
1. [Project Motivation](#Project-Motivation)
2. [Final Showcase](#Final-Showcase)
3. [Electronic Components](#Electronic-Components)
4. [Software Flowchart](#Software-Flowchart)
5. [Pin Connections](#Pin-Connections)
6. [System Schematic](#System-Schematic)
7. [Game Operation](#Game-Operation)
8. [Future Improvements](#Future-Improvements)



## Project Motivation
For this project, we really enjoyed learning about the ability of the MBED to emulate and recieve user input. We wanted to focus on something that put MBED's ability to parse user input to the test, by creating a type racing game. Inspired by the popular [Monkeytype](https://monkeytype.com/), we wanted to create an MBED type racing game. Our game models Monkeytype closely, but includes many Mbed-specific implementation details to bring the game to life on such a low-power device.


## Final Showcase
[![MonkeyType MBED](https://img.youtube.com/vi/41Kbnx_lNgI/hqdefault.jpg)](https://youtu.be/41Kbnx_lNgI)

## Electronic Components
 - uLCD-144-g2: https://www.sparkfun.com/products/11377
 - Class D Audio Amp: https://www.sparkfun.com/products/11044
 - Speaker: https://www.sparkfun.com/products/11089
 - PS2 Keyboard: https://www.amazon.com/MCSaite-Computer-Keyboard-Stands-Waterproof/dp/B097ZLWS7S/ref=sr_1_3?keywords=Ps%2F2+Keyboard&qid=1682912778&sr=8-3


## Software Flowchart
<img src="https://github.com/ranroun3/ECE4180_FinalProject_Spring23/blob/main/ECE%204180%20Final%20Flowchart.png" width="860" height="585">

## Pin Connections
**MBED and uLCD display**
|uLCD|Mbed|
|--- | ---|
| 5V | 5V |
| GND| GND|
| TX | p9 |
| RX | p10|
| RST| p11|



**MBED and PS2 Keyboard**

|Mbed|PS2|
|--- | ---|
| 5V | 5V |
| GND| GND|
| TX | p9 |
| RX | p10|
| RST| p11|

**MBED, TPA2005D1 Amp, and Speaker**
|MBED|TPA|  Speaker |
|--- | ---| ---     |
| 5V |PWR+|         |
| GND|PWR-|         |
| p26| in+|         |
| GND| in-|         |
|    |out+|    +    |
|    |out-|    -   |
//todo


## System Schematic
<img src="https://github.com/ranroun3/ECE4180_FinalProject_Spring23/blob/main/ECE%204180%20Final%20Schematic.png" width="476" height="685">

## Game Operation
Upon booting up the game, our user is presented with a list of words to type, alongside a 30-second timer. These words are generated from a list of the 500 most commonly used words in the English Language. 

The countdown begins, and the user is tasked to type as many words correctly as they can in 30 seconds. Each incorrect letter typed shows up as red on the LCD, while correct letters are green. Once a word is completed, the user can press the space bar or "enter" key to move on to the next word. 

This process continues for 30 seconds, alongside real-time accuracy and WPM calculations. Upon completion of the game, the MBED moves to a "Game Over" state where it shows important statistics, like words and characters typed, along with accuracy and WPM calculation. A short tune accompanies this screen, played using PWM audio output.

## Future Improvements
We have a couple of changes we wish to implement.
1. **Backspace Feature:** Due to our specific game logic, implementing the backspace feature was not possible within our project's time frame. In the future, we would like to find a way to implement this feature while maintaining the core functionality of the game.
2. **Start/Reset Screen:** We would like to give the user the ability to play multiple games, instead of resetting the Mbed in between each trial.
3. **Input Buffer/Reduce Input Lag:** Some users experienced input lag when typing too quickly. We hope to remedy this input lag by either increasing the size of the input buffer or reducing the processing time needed to process an input. Both of these changes may aid the user at the expense of lots of processing power.
4. **Increase Word Library/Randomness of Selection:** Currently, we have about 420 words in our library. While this is quite expansive, it means users could still find repeat words. Moreover, our random number generation technology, which uses the noise level on an unconnected pin, can sometimes experience duplicate values. In future implementations, we would like to replace this with a more truly random strategy.
5. **Upgraded Statistics:** we wish to add a more robust statistics page. This could include things like a rolling WPM graph, error scatter plot, or accuracy graph. These features would enhance the user's experience and allow them to gain usable information from our game. 

