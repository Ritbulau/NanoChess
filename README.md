# NANO CHESS
![logo](/Images/NanoChessLogoR.png?raw=true)
## A fully playable chess game for Arduino Nano/Uno
____

# WARNING! RUN NANO CHESS IN WOKWI ONLY USING THE DEBUG MODE!

[Wokwi Nano Chess emulator link](https://wokwi.com/projects/371236223885557761)

[Wokwi Nano Chess emulator with no sound, but with better performance link](https://wokwi.com/projects/371330181778109441)

Also, all projects based on Nano Engine can be launched on Wokwi!

____

## About

This is a Chess game, based on Nano Engine (currently not for public), that was made for Arduino Nano\Uno.

If you don't know, how to install, here is the instruction:
1. Wire whole thing up.
2. Download Arduino IDE and connect your Arduino Nano/Uno to it.
3. Download mod files and unpack NanoChess folder.
4. Open NanoChess.ino file.
5. Press "Upload".
6. Done!

### Wiring

![wiring](/Images/wiring.png?raw=true)
(Nano have all pins, that Uno does, but in different places).
___

### Features:
* Sounds
* 3 customizable gamemodes
* Informative interface
* Playable in Wokwi

___

### Controls and game info:

Please note that the game itself is designed for two players and does not contain a chess bot.

The first thing you will see after the start screen is the settings menu. Here you can choose one of three game modes: classic, timed and incremental.

![classic](/Images/Classic.png?raw=true)
![timed](/Images/Timed.png?raw=true)
![incremental](/Images/Incremental.png?raw=true)

Please note that you cannot start the game in one of the two time modes if the clock indicators are at zero!

![UI hint 1-10](/Images/UI.png?raw=true)

Then, running into the game itself, you will see a frightening picture at first, but everything is quite simple.
In the picture, the numbers from 1 to 10 indicate different game elements.
1) example of a white piece (here it is a pawn)
2) an example of a black piece (also a pawn), black pieces from white can be distinguished not by color (because the color of the figure is inverted so that it can be seen on both white and black squares), but by 3 pixels, to the right of the figure itself.
3) the cursor. It consists of 4 pixels and blinks constantly, so it will be easy to find on the screen.
4) a square showing whose turn it is now. If it is painted white, it means that the white pieces are moving now, and if it is black, then black.
5) turns counter.
6) the white clock (there are 4 black pixels on the sides of the white clock, which means that it is white time now, if they are on the sides of the clock of black figures, then it is black time)
7) the clock of black figures.
8) text showing which cell the cursor is on, and what is on this cell (it is necessary that it would be easier to determine where and on which figure the cursor is)
9) text showing which chess piece is selected and where it is located.
10) the selected chess piece (here it is a pawn), there are always 4 pixels around the selected piece, like the cursor, but unlike it, they do not blink.
    
![UI hint 11](/Images/UI2.png?raw=true)

The second picture shows an example of the action of the object 11) a window for displaying information about a victory, a check and a draw.



To move a chess piece, hover the cursor over the desired piece of the color that the move is now and press the fire button to select this piece. After that, move the cursor to the cell where you want to move, or to the opponent's chess piece that you want to capture, and press fire again. If your move matches the rules, your chosen chess piece will execute the move. After that, transfer control to the second player. There is a possibility of castling and En passant. To win, you need to eat the enemy king (which is inherently similar to checkmate, but I haven't written the code responsible for calculating it yet (that's why the game is now in beta). Otherwise, all the rules are the same as in ordinary chess.

So you are trained in the rules, have a good game!
___

#### **TRAILER**
[![see it in actrion](/Images/prev2.png?raw=true)](https://youtu.be/4uxslUcWOHw)

___

## To do:
+ Make Checkmate code
___

### And please, don't try to research how does it work, my coding skills can hurt you!

