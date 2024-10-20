**GRID MAKING:**
To make the game first we will make grid 
The grid will be formed by making a node structure with up down left right x_axis y_axis and character
Making function to insert nodes 
**RANDOM PLACEMENTS:**
A function to place characters at random positions and making Key and door disappear by ponting key and door pointer to their positions
**MOVE:**
A function for the player to move by using arrowkeys (done by exploring ncurses different functions)
It contains key found and door found conditions
Condition when new and old coins functions are called          
Pushing moves into the undo stack
If undo move called then moving the player to its previous position
**UNDO FUNCTION:**
Added a undo functionality to the same function 
Undo did by using stack with nodes containing data of only x and y axis and next node
**COINS CHANGING:**
A function to remove the old coins and a function to add new coins to the random places
**DISTANCE FUNCTIONS:**
Calculating distances of player to key, key to door and player to door and working on them to find total moves by using city block distance formula
Moving function used to display the player to key distance and player to door distance statuses respectively.
**DISPLAY:**
Calling respective functions to display top bar and grid according to the conditions matched.
Game over function resets the grid to its initial state whenever the game is over
Playgame function calls move function in loop and getting arrowkey to move the player and if game is over it will display the initial grid without calling moving function and waiting for users exit input to exit the display.
**How game works:**
Select the difficulty level
Move the player by using arrow keys
To undo pres U
To exit the game press q
To exits the game over screen press e
**screenshots of game**
![Screenshot from 2024-10-20 22-56-52](https://github.com/user-attachments/assets/aa614914-3ae2-4e12-9048-bda00d354d22)
![Screenshot from 2024-10-20 22-57-14](https://github.com/user-attachments/assets/449cddcc-251e-4801-8ac0-425928ca634b)
![Screenshot from 2024-10-20 22-57-31](https://github.com/user-attachments/assets/659df92c-9e66-4ad4-9753-67a1719cf26d)
![Screenshot from 2024-10-20 22-58-20](https://github.com/user-attachments/assets/538705e1-93a1-48e8-9f35-1c3f2d405d5f)
![Screenshot from 2024-10-20 23-04-12](https://github.com/user-attachments/assets/4beac031-8fe3-4972-9986-cebff2eecd54)
![Screenshot from 2024-10-20 23-04-12](https://github.com/user-attachments/assets/55bdd02f-8d98-41da-83da-a2c2746fa813)



