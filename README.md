# Monkey Type Videogame on MBED LPC1768
 Georgia Institute of Technology

ECE 4180 - Spring 2023

Creators: Rony Stephan, Andrew Wang

## Table of Contents
1. [Motivation](#Project-Motivation)
2. [Final Showcase](#Final-Showcase)
3. [Electronic Components](#Electronic-Components)
4. [System Schematic](#System-Schematic)
5. [Future Improvements](#Future-Improvements)



## Project Motivation

## Final Showcase

## Electronic Components
 - uLCD-144-g2: https://www.sparkfun.com/products/11377
 - Class D Audio Amp: https://www.sparkfun.com/products/11044
 - Speaker: https://www.sparkfun.com/products/11089
 - PS2 Keyboard: https://www.amazon.com/MCSaite-Computer-Keyboard-Stands-Waterproof/dp/B097ZLWS7S/ref=sr_1_3?keywords=Ps%2F2+Keyboard&qid=1682912778&sr=8-3

**MBED and uLCD display**
|uLCD|Mbed|
|--- | ---|
| 5V | 5V |
| GND| GND|
| TX | p9 |
| RX | p10|
| RST| p11|



**MBED and PS2 Keyboard**

|uLCD|Mbed|
|--- | ---|
| 5V | 5V |
| GND| GND|
| TX | p9 |
| RX | p10|
| RST| p11|

**MBED, TPA2005D1 Amp, and Speaker **
|MBED|TPA|  Speaker |
|--- | ---| ---     |
| 5V |PWR+|         |
| GND|PWR-|         |
| p26| in+|         |
| GND| in-|         |
|    |out+|    +    |
|    |out-|    -   |



## System Schematic

## Important Components

## Game Operation
Upon booting up the game, our user is presented with a list of words to type, alongside a 30-second timer. These words are generated from a list of the 500 most commonly used words in the English Language. 

The countdown begins, and the user is tasked to type as many words correctly as they can in 30 seconds. Each incorrect letter typed shows up as red on the LCD, while correct letters are green. Once a word is completed, the user can press the space bar or "enter" key to move on to the next word. 

This process continues for 30 seconds, alongside real-time accuracy and WPM calculations. Upon completion of the game, the MBED moves to a "Game Over" state where it shows important statistics, like words and characters typed, along with accuracy and WPM calculation. A short tune accompanies this screen, played using PWM audio output.

## Future Improvements
In the future, we would like to make a start/reset screen for the device, to allow the user to play multiple games. Additionally, we would like to find a way to increase the size of the input buffer, as some fast typers experienced lag when typing too quickly.
