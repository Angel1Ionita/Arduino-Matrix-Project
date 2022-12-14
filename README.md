# Matrix Project

The scope of this project is to make a game with Arduino using an 8x8 matrix display.

Full requirements can be found in the attached pdf.

### Image
![IMG_20221209_110054](https://user-images.githubusercontent.com/79210109/206665893-a1722fa1-df89-4ce3-b0ad-10fbeb3deff0.jpg)

### Video
[Video Link](https://youtu.be/fMRTq0O3CAk)

## Components used:
-   Arduino UNO
-   wires
-   8x8 LED matrix
-   16x2 LCD
-   1 joystick
-   2 breadboards 
-   2 resistors (1 for matrix , 1 for LCD)
-   1 potentiometer (for LCD contrast control)

## Game description

The idea of the game is to avoid incoming obstacles which become harder as the game goes on faster.
Additionally , an "ability" can appear at a random interval between 15-25 seconds , which will double your current speed for a certain amount of time, which depends on the chosen difficulty. When this happens , a double arrow will appear on the matrix and start blinking.

The game ends upon hitting an obstacle.
To achieve a high score, the game must be played in hard difficulty.


![Screenshot 2022-12-23 105624](https://user-images.githubusercontent.com/79210109/209305341-4f6e384b-1adf-4de2-8d1c-cefb3ae75d1b.png)
Ability activation indicator
## Menu
~~~
Menu
	-> Start
	-> Settings
		-> Name
		-> Difficulty
			-> Easy/Normal/Hard
		-> LCD Brightness
		-> LED Brightness
		-> Sounds
	-> High Score
	-> How to play
	-> About
	-> Reset
~~~


## Difficulty

Over time, the game will start moving faster. The chosen starting difficulty will also affect the following :

- Increased obstacle generation chance
- Increased ability duration

### More detailed:
- Easy: Chance of obstacle generation on a line: 33%, ability duration: 7 seconds
- Normal: Chance of obstacle generation on a line: 50%, ability duration: 10 seconds
- Hard: Chance of obstacle generation on a line: 100%, ability duration: 13 seconds 