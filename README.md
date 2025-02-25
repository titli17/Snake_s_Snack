# **Snake_s_Snack**

## **Introduction:**

In this project we have created a simple Snake game using C++ language and raylib library. Raylib is a simple and easy-to-use library to enjoy videogames programming and it's especially well suited for prototyping, tooling, graphical applications, embedded systems and education.

Every game structure consists of 2 parts : DEFINITIONS and GAME LOOP.

**DEFINITIONS** is the part where we define our variables and game objects such as the snake, the food and the gaming arena. 
<br>The **GAME LOOP** is responsible for event handling, updating the positions of game objects and checking for collisions. The updates happen so fast that it appears like a continuous movement. The GAME LOOP is run repeatedly until the game is closed. 

We will make this game by breaking it down into smaller parts.

## **Step 1: Creating Blank Canvas and Game Loop**

The game window is like a blank canvas where we can draw our game objects.
<br>We need to specify the width and the height of the game window. To create the game window we call the InitWindow() function. This function initialises a window.

Everytime we create a window, at some point we have to destroy it, and we do so with the CloseWindow() function before the return statement. It checks if the Escape key on the keyboard is pressed or if the close icon of the window is pressed, and returns true if that is the case. So, if it returns true, the game loop ends, and the window is destroyed.

**GAME LOOP**
<br>The game loop consists of 3 parts. 
<br>**Event handling:** First, we check for any events that occur in the game, such as
quitting the game, a key pressed on the keyboard, etc. 
<br>**Updating positions:** Next, we update the positions of all game objects, such as the snake and food. 
<br>**Drawing objects:** Finally, we draw all the game objects in their new positions on the screen. 

In the GAME LOOP, we need BeginDrawing function which creates a blank canvas so we can draw our game objects on and the EndDrawing() function which ends the canvas drawing.

Next we define how fast the game should run by calling the SetTargetFPS() function before the game loop. The SetTargetFPS() function takes an integer as an argument and that integer is the number of frames per second that we want. If we don’t define a frame rate for our game, the computer will try to run the game as fast as it can. So the game speed will depend on the speed of the computer. By defining a frame rate we make sure that the game runs at the same speed on every computer.


## **Step 2: Creating the Food**

We create an invisible grid, i.e. dividing the game screen in a large
number of identical cells to help us with the positioning and the movement of the objects on the display.
<br>To display a food object, we are going to display it inside a cell. We use a Vector2 variable to hold the position of the food object. Vector2 is a data structure that contains x and y attribute.

Suppose Vector2 position = {5, 6}.
<br>The two numbers in the parentheses (5, 6) are the x and y coordinates of the food. For example, the food's x-coordinate can be accessed by position.x and its y-coordinate by position.y. These numbers represent cells. The food's position is at the 6th column and 7th row in our grid.

To load an image for the food object, we use the LoadImage() function. This function takes a string argument that represents the path to the image file. The loaded image is used to create a texture using the LoadTextureFromImage() function. 
<br>A destructor is also created to unload the texture when the object is destroyed.

Instead of a fixed position, we create a random position for the food object using the GetRandomValue(). So everytime the snake eats the food, the food object is displayed in a new position.

## **Step 3: Creating the Snake**

To represent the snake we draw some of the cells of the screen dark green. The body of the snake is an array of some cells. For starting, it is 3 cells long, then the size increases as it eats the food.

We use a deque to represent the snake. A deque is a data structure that allows efficient addition and removal of elements from both ends.
<br>The first item in the deque is the “head” of the snake and the remaining ones the "tail".

We draw the snake's body using DrawRoundedRectangle() and DrawEllipse() and filled it with dark green color.

## **Step 4: Moving the Snake**

We move the snake object by removing the last cell in the collection and adding a new cell to the beginning of the collection. 
<br>Suppose we want to move the snake to the right, we remove the last cell from it and add another one at the beginning to the right. If we want to move the snake down, we remove the last cell from it and add another one at the beginning to the bottom.

void Update() method is created to update the position of the snake.

To slow down the snake's movement and make it more visible on the screen we create a function called EventTriggered() that checks if a certain amount of time has passed since the last update of the snake. We call this function 60 times per second so it is inside the game loop.

In order to control the snake's movement, we add keyboard controls to the game in the game loop.

![image](https://github.com/titli17/Snake_s_Snack/assets/96014974/9fd5205b-1fdc-4d60-bdee-34d2171664f8)

## **Step 5: Creating the Game Class**

To improve code organization and make it easier to manage in the future, we create a Game class to hold the snake and food objects, as well as various methods. The Game class serves as a container for all the elements of our game such as the snake, food, and game state. It holds methods that manage the game's logic such as updating the snake's position, checking for collisions, etc. 

## **Step 6: Making the Snake eat the Food**

If the snake’s head cell and the food cell are the same for every update of the game, the snake eats the food, and we create another food object in another random location. 
<br> This checking is done using the void checkEatFood() function in the Game class.

## **Step 7: Making the Snake grow longer**

When the snake eats the food, we add a new segment to the beginning of its body and do not move the snake for that call of the Update() method to create a more natural visual effect.

## **Step 8: Checking for Collisions**

If the position of the head of the snake is out of the grid at the x and y axis, a collision occurrs. So we end the game, i.e. GameOver() method in the Game class.
<br>When the game is over, the player can restart the game by pressing the "->" button and the snake moves to its original position. To move it to the original position, we create the GameRestart() function.

To pause the game until the player restarts the game, we add another attribute to the Game class, i.e. the boolean variable "running". It's value is true if the game runs, and false if the game is stopped. 

Next is the checking if the snake's head touches it's body. To do this we create a copy of the snake body. Then from the deque we remove the head, the first element. If the snake’s head is in this deque, it means that the head has collided with the tail, and the game is over. 

## **Step 9: Adding Score**

In the game class we add an integer variable named "score" initialised to 0. The score increases by one every time the snake eats a food object. So it is included in the checkEatFood() function.

Finally, we display the score in the gaming window and our game is complete.

![image](https://github.com/titli17/Snake_s_Snack/assets/96014974/af3f59c0-452e-44fa-b6e0-e5f43d4e39b3)



