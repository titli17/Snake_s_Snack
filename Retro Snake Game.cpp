#include <iostream>
#include <deque>
#include <raylib.h>
#include <raymath.h>

using namespace std;

Color green = { 173,204,96,255 };            //4 arguments, color = {red,green,blue,alpha}....alpha represents the opacity of color, 255 means opaque and 0 means transparent
Color darkgreen = { 43,51,24,255 };

int cellsize = 30;
int cellcount = 25;				//30x25=750 so window size of 750 is covered...25 times 30 pixels...30 pixels make 1 cell. representing the entire window in a grid

float offset = 75;

double lastUpdateTime = 0;				//keep tracks of the time at which the last update of the snake occured. it is initialised to zero when the game starts

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval)		//if the difference is >= 300ms this means 300 ms has passed since last update time...so set lastUpdateTime to currentTime
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

bool elementInDeque(Vector2 element, deque<Vector2> deque)			//the food position and the snake body are passed as arguments
{
	for (unsigned int i = 0; i < deque.size(); i++)					//deque.size() returns the number of cells present in the snake body
	{
		if (Vector2Equals(deque[i], element))
			return true;
	}
	return false;
}


class Snake {

public:
	deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };        //head of our snake is (6,9)         deque is double ended queue...enqueue and dequeue is possible on both ends 
	// a deque of Vector2 type is created named body...

	Vector2 direction = { 1,0 };

	bool addSegment = false;     // for adding one cell to the snake if it has eaten food

	void Draw()
	{
		float x = body[0].x;
		float y = body[0].y;
		if (body[0].y == body[1].y)
		{
			if (body[0].x > body[1].x)
			{
				DrawEllipse(offset + x * cellsize + 15, offset + y * cellsize + 15, (float)cellsize / 1.5, (float)cellsize / 2, darkgreen);
				DrawCircle(offset + x * cellsize + 22, offset + y * cellsize + 8, 2, green);
				DrawCircle(offset + x * cellsize + 22, offset + y * cellsize + 22, 2, green);
			}
			else
			{
				DrawEllipse(offset + x * cellsize + 15, offset + y * cellsize + 15, (float)cellsize / 1.5, (float)cellsize / 2, darkgreen);
				DrawCircle(offset + x * cellsize + 8, offset + y * cellsize + 22, 2, green);
				DrawCircle(offset + x * cellsize + 8, offset + y * cellsize + 8, 2, green);
			}
		}
		if (body[0].x == body[1].x)
		{
			if (body[0].y > body[1].y)
			{
				DrawEllipse(offset + x * cellsize + 15, offset + y * cellsize + 15, (float)cellsize / 2, (float)cellsize / 1.5, darkgreen);
				DrawCircle(offset + x * cellsize + 22, offset + y * cellsize + 22, 2, green);
				DrawCircle(offset + x * cellsize + 8, offset + y * cellsize + 22, 2, green);
			}
			else
			{
				DrawEllipse(offset + x * cellsize + 15, offset + y * cellsize + 15, (float)cellsize / 2, (float)cellsize / 1.5, darkgreen);
				DrawCircle(offset + x * cellsize + 8, offset + y * cellsize + 8, 2, green);
				DrawCircle(offset + x * cellsize + 22, offset + y * cellsize + 8, 2, green);
			}
		}

		for (int i = 1; i < body.size(); i++)
		{
			x = body[i].x;
			y = body[i].y;
			Rectangle segment = Rectangle{ offset + x * cellsize, offset + y * cellsize, (float)cellsize, (float)cellsize };
			DrawRectangleRounded(segment, 0.5, 6, darkgreen);			//0.5 is the radius of the corners...6 is the number of line segments used to draw the corners for smoother corners         
		}
	}

	void Update()
	{
		body.push_front(Vector2Add(body[0], direction));      //adding one cell to the head of the snake to make it move
		if (addSegment == true)
			addSegment = false;				//if snake has eaten then no popping of last cell of snake
		else
			body.pop_back();				//if snake has not eaten then popping of last cell to make it look like its moving
	}

	void Reset()
	{
		body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
		direction = { 1,0 };
	}

	void Vanish()
	{
		body = { Vector2{31,31}, Vector2{31,31}, Vector2{31,31} };
	}
};

class Food {

public:
	Vector2 position;									//position is variable of Vector2. It will contain x and y values, accessible by position.x and position.y

	Texture2D texture;			//Texture2D is a variable type that represents an image loaded into the graphics memory. Optimised datatype for GPU processing.

	Food(deque<Vector2> snakeBody)
	{
		Image img = LoadImage("fd.png");		//this function returns an image. In raylib Image is a DS that contains the pixel data of a graphical image.
		texture = LoadTextureFromImage(img);
		UnloadImage(img);      //free up some memory        

		position = GenRandomPos(snakeBody);
	}

	/*~Food()
	{
		UnloadTexture(texture);
	}*/

	void Draw()
	{
		//DrawRectangle(offset + position.x * cellsize, offset + position.y * cellsize, cellsize, cellsize, darkgreen);   //5 arguments (x and y coordinates, width and height, color)

		DrawTexture(texture, offset + position.x * cellsize, offset + position.y * cellsize, WHITE);    // WHITE means no filter on the image
	}

	Vector2 GenRandomCell()
	{
		int x = GetRandomValue(0, cellcount - 1);
		int y = GetRandomValue(0, cellcount - 1);
		Vector2 pos = { x,y };
		return pos;
	}

	Vector2 GenRandomPos(deque<Vector2> snakeBody)				//this function ensures that food doesnt generate in the snake's body cell
	{
		Vector2 position = GenRandomCell();
		while (elementInDeque(position, snakeBody))				//checking if position generated == snake's body cell or not
		{
			position = GenRandomCell();							//if yes then GenRandomCell() will be called continuously
		}
		return position;
	}
};


class Game {

public:
	Snake snake = Snake();
	Food food = Food(snake.body);

	bool running = true;

	int score = 0;

	void Draw()
	{
		food.Draw();
		snake.Draw();
	}

	void Update()
	{
		if (running)
		{
			snake.Update();
			checkEatFood();
			checkCollision();
		}

	}

	void checkEatFood()
	{
		if (Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.GenRandomPos(snake.body);		//full snake body is passed here
			snake.addSegment = true;							//this variable is made true so that no popping of last segment occurs
			score++;
		}
	}

	void checkCollision()
	{
		if (snake.body[0].x == cellcount || snake.body[0].x == -1)
		{
			GameOver();
		}
		if (snake.body[0].y == cellcount || snake.body[0].y == -1)
		{
			GameOver();
		}
		for (int i = 1; i < snake.body.size(); i++)
		{
			if (Vector2Equals(snake.body[0], snake.body[i]))
			{
				GameOver();
			}
		}
	}


	void GameRestart()
	{
		snake.Reset();
		food.position = food.GenRandomPos(snake.body);
		running = true;
		score = 0;
	}


	void GameOver()
	{
		running = false;
		snake.Vanish();
	}
};


int main()
{
	InitWindow(2 * offset + cellsize * cellcount, 2 * offset + cellsize * cellcount, "Snake Game");        //initialise the game window (pixelsize and the title of the game

	SetTargetFPS(60);              //it sets the speed of the game...here 60 frames per second, run the while loop and update the display 60 times a second

	Game game = Game();

	while (WindowShouldClose() == false)
	{
		BeginDrawing();
		if (game.running)
		{
			if (eventTriggered(0.2))			//this ensures that snake doesnt move 60 cells every second. Every 300 ms the update function will be called and this snake will move
			{
				game.Update();
			}

			if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
			{
				game.snake.direction = { 0,-1 };
				game.running = true;
			}

			if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
			{
				game.snake.direction = { 0,1 };
				game.running = true;
			}

			if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
			{
				game.snake.direction = { -1,0 };
				game.running = true;
			}

			if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
			{
				game.snake.direction = { 1,0 };
				game.running = true;
			}

			ClearBackground(green);        //making the gamescreen green
			game.Draw();

			DrawRectangleLinesEx(Rectangle{ offset - 5 , offset - 5 , (float)cellsize * cellcount + 10, (float)cellsize * cellcount + 10 }, 5, darkgreen);

			DrawText("Retro Snake Game ", offset - 5, offset - 50, 40, darkgreen);
			DrawText("Score : ", offset - 5, offset + cellsize * cellcount + 10, 30, darkgreen);
			DrawText(TextFormat("%i", game.score), offset + 120, offset + cellsize * cellcount + 10, 30, darkgreen);


		}
		else
		{
			DrawText("GAME OVER ! ", offset + 175, offset + 300, 60, darkgreen);
			DrawText("Press -> to restart the game ", offset + 75, offset + 400, 40, darkgreen);
			if (IsKeyPressed(KEY_RIGHT))
				game.GameRestart();
		}

		EndDrawing();

	}

	CloseWindow();
	return 0;
}