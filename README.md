# AERSP_424_Final_Project

Members: Mark Yazemboski, Tyler Brazenec, Sean Magilton

Overview
This game is about a gambling addict (us), who can't get enough of slot machines. So much so, that when he runs out of money, he slithers around the streets of Las Vegas until he gets enough money to play the slots again. The current state of the game is just the "Snake" portion of it, where the player will slither around the streets picking up coins, getting longer for every 25 cents collected.

Game Interface
Once the game starts, the game window pops open, and the game begins. This screen depicts an intersection in Las Vegas, where coins begin dropping onto the board. These coins can be collected, increasing the sum earned and the length of the playable character. Both the street and sidewalk are viable movement areas, with the border of the window depicting the end of the playable area. Colliding with the border results in the game closing and ending. 


Code Explanation
The Slots Are Calling has the following files.

Board
Keeps track of all of the snake positions and coin positions in a 2D array. It also handles the game logic, like telling the snake when to move and knowing when the snake ate a coin and spawns a new one.

Body_Part
This is a class that keeps track of the position and orientation of a specific segment of the snake. It has three subclasses called Head, Body, and Tail, each with its own unique image file.

Coin
This class will keep track of a certain coin's value and position. It also contains subclasses for the different coin types like Penny nickel, etc. The different subclasses have their own image files. The Coins were done in subclasses so the different coins could have different functionalities in the future.

Snake
This class handles all of the logic for the snake. It still keeps track of all of the body parts and their order and direction. When the snake moves, it will add a head object to the front of the queue and will pop the tail from the back. It will then turn the old head into a body and turn the last body into a tail.

Glad
This does something for the OpenGL visual

STB
This does something for the OpenGL visual

Play
This file combines all of the other files to play the game. It creates the Game window and runs the game loop, where it will call the update board function, and display all of the images on the screen. It will also get the user's input.

Gambler
This is the class that would have kept track of the player's total wealth and other aspects of the game as they went between the slot machine and picking up coins from the ground.

Slot Machine
This class has all of the functionality of a slot machine. It keeps track of the different cards in each wheel. And can spin the wheel and pick three random cards. It will calculate if the card combination was a win and will pay the player if it was. It also has the functionality to take players money and not play if they dont have enough.

Card
This is the card class for the slot machine. A card is one of the symbols that the slot machine runs through. The Card class stores the payout for the specific card, as well as the symbol it is for the text display.

Play Slot
This file will combine all of the slot machine-specific things. It will create a gambler, create a list of cards, and then create a slot machine with those cards. The gambler will then play the slot machine 5 times. All of the output is in the command window and is not compiled in the build file and will need to be run on your own

