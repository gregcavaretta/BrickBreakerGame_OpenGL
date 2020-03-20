//By Greg Cavaretta
//Version 1.2
//Dec 10, 2019


#include <stdio.h>
//This may need to be changed
#include <include/GL/freeglut.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <Windows.h> //Used for sleep


using namespace std;

//clipping window size
float cwx = 800, cwy = 600;
void init();
void display();
int SPEED = 2500; //Lower = faster, Higher slower 


//Brick class
class Brick {
private:

	//x is length (Left/right) ⇔
	//y is height (Up/down) ∧∨
	//z is width (The thing that makes stuff 3d)
	//Score keep tracks how much the brick is worth
	double x, y, z, score_value;

	//Used when moving Y pos
	int offset;


public:

	//default constructor 
	Brick() {
		x = 0;
		y = 0;
		z = 0;
		score_value = 0;
		offset = 5;
	}

	//-> are pointers
	//Overload constructor
	Brick(double x = 0, double y = 0, double z = 0, int score_value = 0, double offset = 5) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->score_value = score_value;
		this->offset = offset;
	}


	//Overload destructor
	~Brick() {

	}


	//Getters
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }
	double getScoreVal() { return score_value; }
	double getOffset() { return offset; }

	//Setters
	void setX(double x) { this->x = x; };
	void setY(double y) { this->y = y; };
	void setZ(double z) { this->z = z; };
	void setScoreVal(int score_value) { this->score_value = score_value; };
	void setOffset(double offset) { this->offset = offset; };


};

class Ball {
private:
	//Size of ball
	double ballSize;
	//Current X, Y pos
	double curX, curY;

	//Ball destroyed
	bool ballDestroyed = false;

	//Ball color
	int ballColor;

	//Which way the ball is moving
	int dir_x, dir_y;


	int layoutBoundsX, layoutBoundsY;

	//Used to store the 4 points of the ball, needed for hit detection
	int col_x, col_x1, col_y, col_y1;

public:
	//default constructor 
	Ball() {
		ballSize = 0;
		curX = 0;
		curY = 0;
		ballColor = 1;
		dir_x = 0;
		dir_y = 0;
		layoutBoundsX = cwx;
		layoutBoundsY = cwy;
		col_x = col_x1 = col_y = col_y1 = 0;

	}

	//Overload constructor


	/// <summary> 
	/// Overloaded Ball constructor - GC
	/// </summary>
	/// <param name="x_pos"> Current ball's x position </param>
	/// <param name="y_pos"> Current ball's y position </param>
	/// <param name="ballSize"> Scale the ball size by (x, y) </param>
	/// <param name="xBound"> Where the ball can bounce in x direction. If 0 then default = cwx </param>
	/// <param name="yBound"> Where the ball can bounce in y direction. If 0 then default = cwy </param>
	Ball(double x_pos, double y_pos, double ballSize, int xBound, int yBound) {

		//Ballsize cannot be < 1
		if (ballSize <= 0) {
			ballSize = 1;
		}

		//Set ball size
		this->ballSize = ballSize;

		//Set current x,y pos
		curX = x_pos + ballSize;
		curY = y_pos + ballSize;

		//Set layout bounds
		layoutBoundsX = cwx; // glutGet(GLUT_WINDOW_WIDTH);
		layoutBoundsY = cwy; //glutGet(GLUT_WINDOW_HEIGHT);

		dir_x = 0;
		dir_y = 0;
		col_x = col_x1 = col_y = col_y1 = 0;

	}

	//dir_x is the direction in x the ball is moving
	//    -1 is left, 1 is right, 0 is same position
	//dir_y is the direction in y the ball is moving
	//    -1 is down, 1 is up, 0 is same position
	void moveBall() {

		// 0, 0 is lower left side	

		//Wall collider
		//If ball hits right of screen -> bounce left
		if (layoutBoundsX <= getcurX() + ballSize) {
			dir_x = -1;
		}

		//If ball hits left side of screen -> bounce right
		else if (getcurX() <= 0 + ballSize) {
			dir_x = 1;
		}

		//If ball hits top of screen -> should go down
		else if (layoutBoundsY <= getcurY() + ballSize) {
			dir_y = -1;
		}

		//If ball hits bottom od screen -> should remove ball
		else if (getcurY() <= 0 + ballSize) {
			//dir_y = 1;
			//Render off screen. And not moving
			/*curX = -200;
			curY = -200;
			dir_x = 0;
			dir_y = 0;*/
			ballDestroyed = true;
		}


		if (dir_x == -1) {
			curX--;
		}
		else if (dir_x == 1) {
			curX++;
		}

		if (dir_y == -1) {
			curY--;
		}
		else if (dir_y == 1) {
			curY++;
		}
	}

	//This function will keep tell if the ball went out of bounds
	bool getBallDestroyed() { return ballDestroyed; }

	//This function sets the ball to move up and to the left
	void moveBallLeftUp() {
		dir_x = -1;
		dir_y = 1;
	}

	//This function sets the ball to move up and to the right
	void moveBallRightUp() {
		dir_x = 1;
		dir_y = 1;
	}

	//This function sets the ball to move down and to the right
	void moveBallRightDown() {
		dir_x = 1;
		dir_y = -1;
	}

	//This function sets the ball to move down and to the left
	void moveBallLeftDown() {
		dir_x = -1;
		dir_y = -1;
	}

	//This function added a random value to the current x cords
	void randomOffset() {
		int randNum = rand() & 9 - 4; //Generate a number -4 <-> 4

		curX += randNum;


	}

	//This function will moving the ball in a random direction. This needs to called when game first starts.
	void moveBallInRandomDir() {
		int randNum = rand() & 2; //Generate a number 0 or 1

		//Move right
		if (randNum == 1) {
			dir_x = 1;
		}
		//move left
		else {
			dir_x = -1;
		}

		//Always moving up
		dir_y = 1;
		randomOffset();

	}

	//This function will move the ball based on how its been hit
	void decisionMakerTopOrBottom() {

		if (dir_x == -1 && dir_y == -1) {
			dir_x = 1;
			dir_y = 1;
		}
		else if (dir_x == 1 && dir_y == -1) {
			dir_x = -1;
			dir_y = 1;
		}
		else if (dir_x == -1 && dir_y == 1) {
			dir_x = 1;
			dir_y = -1;
		}
		else if (dir_x == 1 && dir_y == 1) {
			dir_x = -1;
			dir_y = -1;
		}

		randomOffset();
	}

	//This function will move the ball based on how its been hit
	void decisionMakerLeftOrRight() {

		if (dir_x == -1 && dir_y == -1) {
			moveBallRightDown();
		}
		else if (dir_x == 1 && dir_y == -1) {
			moveBallLeftUp();
		}
		else if (dir_x == -1 && dir_y == 1) {
			moveBallRightDown();
		}
		else if (dir_x == 1 && dir_y == 1) {
			moveBallLeftDown();
		}

		randomOffset();
	}

	//This function will draw a ball on the screen
	void drawBall() {

		GLfloat ambient1[] = { 1,1,1 };
		GLfloat diffuse1[] = { 0.4,0.4,0.4 };
		GLfloat specular1[] = { 1,1,1 };

		GLfloat position[] = { 0,0,-50,1 };
		GLfloat ambient2[] = { 0,0,0 };
		GLfloat diffuse2[] = { 1,1,1 };
		GLfloat specular2[] = { 0,1,1 };

		float materialColours[][3] = { {1,0,0},{0,0,1},{0,1,0},{1,0,1},{1,1,0},{0,1,1} };
		GLfloat matAmbient1[] = { 1,1,1 };
		GLfloat matDiffuse1[] = { 1,1,1 };
		GLfloat matSpecular1[] = { 1,1,1 };
		GLfloat shininess[] = { 1000 };

		glLightfv(GL_LIGHT0, GL_SPECULAR, specular1);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);

		glLightfv(GL_LIGHT1, GL_POSITION, position);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular2);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);

		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialColours[1]);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);

		//Push matrix
		glPushMatrix();
	
		glTranslatef(getcurX(), getcurY(), 0);
		glScalef(ballSize, ballSize, .5);
		glutSolidSphere(1.0, 52, 52);
		glPopMatrix(); //Pop matrix 
		glDisable(GL_LIGHTING);

		col_x = curX - ballSize;
		col_x1 = ballSize + curX;
		col_y = curY + ballSize;
		col_y1 = curY - ballSize;

		/* cout << "( " << ballSize - curX << ", " << curY - ballSize << " ) ";
		cout << "( " << ballSize - curX << ", " << ballSize + curY << " ) ";
		cout << "( " << ballSize + curX << ", " << curY - ballSize << " ) ";
		cout << "( " << ballSize + curX << ", " << ballSize + curY << " ) " << endl; */


	}

	//Set the size of the ball
	void setBallSize(double ballSize) {
		this->ballSize = ballSize;
	}

	//This function will check to see if the ball is moving
	bool isBallMoving() {

		if (dir_x == 0 && dir_y == 0) {
			return false;
		}
		else {
			return true;
		}

	}


	double getBallSize() { return ballSize; }
	double getcurX() { return curX; }
	double getcurY() { return curY; }
	void setcurPos(double x, double y) { curX = x; curY = y; } //This function sets the x, y value of ball pos
	int getCol_x() { return col_x; }
	int getCol_x1() { return col_x1; }
	int getCol_y() { return col_y; }
	int getCol_y1() { return col_y1; }


};

//This class keeps track of all ojects in the game.
//class Game: public Ball, public Brick {
class Game {
	//Vector to keep track of blocks and balls 
	vector<Ball> ballVect;
	vector<Brick> brickVect;
	
	int BALL_SIZE = 8;


	int score = 0;
	int lives = 0;
	bool firstClick = false;

	//Some consts
	const int MAX_BRICKS = 30;
	const int MIN_BRICKS = 01;

	//Paddle positions
	float xp = 200.0, yp = 100.0, xp2 = 260, yp2 = 90;
	float xPos = 0.0, yPos = 0.0;

public:
	Game(int bricks, int balls, int lives = 3) {

		//Lets create the game

		//Set the lives
		this->lives = lives;


		//Check if there are too many bricks
		if (bricks > MAX_BRICKS) {
			bricks = MAX_BRICKS;
		}

		//CHeck if there are too few bricks
		if (bricks < MIN_BRICKS) {
			bricks = MIN_BRICKS;
		}

		//Lets generate bircks 
		//Y modifier - Keeps track of y blocks
		int yModifier = 30;
		double xModifier = .005;
		int yOffset = 0;

		for (int i = 1; i <= bricks; i++) {
			brickVect.push_back(Brick(xModifier, yModifier, 1, 10, yOffset));
			xModifier += .21;


			//If we need to draw bricks on a new row 
			switch (i) {
			case 05: yModifier += 30;  xModifier = .005; yOffset = 30; break;
			case 10: yModifier += 30;  xModifier = .005; yOffset += 30; break;
			case 15: yModifier += 30;  xModifier = .005; yOffset += 30; break;
			case 20: yModifier += 30;  xModifier = .005; yOffset += 30; break;
			case 25: yModifier += 30;  xModifier = .005; yOffset += 30; break;
			case 30: yModifier += 30;  xModifier = .005; yOffset += 30; break;
			}

		}

		//Add ball
		Ball tBall = Ball(xp, yp, BALL_SIZE, 0, 0);
		ballVect.push_back(tBall);




	}

	//This function draws the game
	void drawGame() {

		//draws the background gradient
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
		glVertex2i(0, cwy);
		glVertex2i(cwx, cwy);
		glColor3f(0.5, 0.0, 0.5);
		glVertex2i(cwx, 0);
		glVertex2i(0, 0);
		glEnd();

		//draws the lives
		string mLives = "Lives: " + to_string(lives);

		//parse the score
		string mScore = "Score: " + to_string(score);

		//DrawUI
		writeText(cwx * .87, cwy * .9, mScore, mScore.length());
		writeText(10, cwy * .9, mLives, mLives.length());

		//Draw the bricks
		if (brickVect.size() == 0) { //Game won
			writeText(15, 50, "Congratulations, you won the game...", 36);
			writeText(15, 20, "PRESS A NUMBER (1 - 6) TO START A NEW GAME", 42);
			
		} else {
			for (int i = 0; i < brickVect.size() - 0; i++) {
				drawBrick(brickVect[i]);
			}
		}



		if (ballVect.size() != 0) {
			//if the game has not been started yet, stick the ball to the paddle.
			if (ballVect[0].isBallMoving() == false) {
				ballVect[0].setcurPos(xp + ballVect[0].getBallSize(), yp + ballVect[0].getBallSize());
				ballVect[0].drawBall();
			}
			else {

				//Draw the ball(s)
				//for (int i = 0; i < ballVect.size() - 0; i++) {
				int i = 0;
				while (i < ballVect.size()) {
					ballVect[i].setBallSize(BALL_SIZE);

					//If the ball is destory, remove from the vector
					if (ballVect[i].getBallDestroyed())
					{
						ballVect.erase(ballVect.begin() + i);
						break;
					}
					//Paddle collider
					//Math is something like this
					// if the ball hits between xp and xp2 @ y pos 


					//Ball size + current x pos
					double mBS_CX = ballVect[i].getcurX() + ballVect[i].getBallSize();
					double mBS_CY = ballVect[i].getcurY() - ballVect[i].getBallSize();

					//If the ball is in the range of paddle pos xp and xp2 AND i 

					if (ballVect[i].getCol_x() >= xp && ballVect[i].getCol_x() <= xp2 && ballVect[i].getCol_y1() == yp ||
						ballVect[i].getCol_x1() >= xp && ballVect[i].getCol_x1() <= xp2 && ballVect[i].getCol_y1() == yp) {

						//Now for the fun part, 
						//The "Left side" is 20% of the paddle from left
						//The "Middle" is 60% after the left side
						//The "Right side" is 20% of the paddle from the middle

						double _20_percent = (xp2 - xp) * .2;
						double _80_percent = (xp2 - xp) * (.6 + .2); //The + 0.2 is so that it is understood that we've already gotten the first 20%
						double _100_percent = (xp2 - xp) * (.2 + .8); // The + 0.8 is so that it is understood that we've already gotten the first 80%

						//Becuase we already know that the ball has hit the paddle we can shrink the numbers we're working with.
						//Basically we're "setting" xp = 0 and computing the offset of xp2
						double ballMath = mBS_CX - xp; //Smaller numbers are nicer to work with


						//The ball hit the left side of the paddle
						if (ballMath <= _20_percent) {
							ballVect[i].moveBallLeftUp();
						}

						//Move the ball somewhere random
						else if (ballMath <= _80_percent) {
							ballVect[i].moveBallInRandomDir();
						}

						//The ball hit the right side of the paddle 
						else if (ballMath <= _100_percent) {
							ballVect[i].moveBallRightUp();
						}

						else {
							exception("SOMETHING IS WRONG, VERY VERY WRONG... BLAME GREG");
						}


					}


					//Check to see if the ball has hit a brick
					//For any given brick j in brickVect
					//--There are 4 points on a brick (bx1, bx2, by1, by2)
					//---First we must get the value for all 4 points
					//---To see if a brick has been hit, We're going to get the range between any 2 points
					//---

					//
					int j = 0;
					//for (int j = 0; j < brickVect.size(); j++) {
					while (j < brickVect.size()) {

						int tScore = 0;
						try { tScore = brickVect[j].getScoreVal(); }
						catch (const std::exception & e) { tScore = 10; }


						Brick mBrick = brickVect[j]; //Temp brick
						double bx1 = cwx * mBrick.getX(); //Left point
						double bx2 = cwx * mBrick.getX() + 120;//Right point
						double by1 = (cwy * .9 - mBrick.getY()) - mBrick.getOffset(); //Top point
						double by2 = (cwy * .9 - mBrick.getY() - 30) - mBrick.getOffset();//Low point


						//If the ball hits the bottom of brick
						if (ballVect[i].getCol_x() >= bx1 && ballVect[i].getCol_x() <= bx2 && ballVect[i].getCol_y() == by2 ||
							ballVect[i].getCol_x1() >= bx1 && ballVect[i].getCol_x1() <= bx2 && ballVect[i].getCol_y() == by2) {
							score += tScore;
							brickVect.erase(brickVect.begin() + j);
							ballVect[i].decisionMakerTopOrBottom();
							j--;
						}

						//If the ball hits the top of brick
						else if (ballVect[i].getCol_x() >= bx1 && ballVect[i].getCol_x() <= bx2 && ballVect[i].getCol_y1() == by1 ||
							ballVect[i].getCol_x1() >= bx1 && ballVect[i].getCol_x1() <= bx2 && ballVect[i].getCol_y1() == by1) {
							score += tScore;
							brickVect.erase(brickVect.begin() + j);
							ballVect[i].decisionMakerTopOrBottom();
							j--;
						}



						//else if the ball has hit the left of brick
						else if (ballVect[i].getCol_y() >= by2 && ballVect[i].getCol_y() <= by1 && ballVect[i].getCol_x() == bx2 ||
							ballVect[i].getCol_y1() >= by2 && ballVect[i].getCol_y1() <= by1 && ballVect[i].getCol_x() == bx2) {
							score += tScore;
							brickVect.erase(brickVect.begin() + j);
							ballVect[i].decisionMakerTopOrBottom();
							j--;
						}

						//else if the ball has hit the right of brick
						else if (ballVect[i].getCol_y() >= by2 && ballVect[i].getCol_y() <= by1 && ballVect[i].getCol_x1() == bx1 ||
							ballVect[i].getCol_y1() >= by2 && ballVect[i].getCol_y1() <= by1 && ballVect[i].getCol_x1() == bx1) {
							score += tScore;
							brickVect.erase(brickVect.begin() + j);
							ballVect[i].decisionMakerTopOrBottom();
							j--;
						}



						//This is needed, or program doesn't work...
						cout << "";
						//Useful for debugging
						//cout << xPos << " " << yPos << " || "
						//	<< xp << " " << xp2 << ", " << yp << " " << yp2 << " || "
						//	<< ballVect[i].getcurX() << ", " << ballVect[i].getcurY() << endl;

						j++;
					}
					ballVect[i].moveBall();
					ballVect[i].drawBall();

					i++;
				}
			}

		}
		else {
			
			if (getLives() > 0) {
				//Remove a live
				removeLive();

				//set the ball back on paddle
				setFirstClick(false);

				//Add ball
				Ball tBall = Ball(xp, yp, BALL_SIZE, 0, 0);
				ballVect.push_back(tBall);
			}
			else {
				writeText(50, 50, "GAME OVER. PRESS 1 TO START A NEW GAME          ", 40);
			}

		}

		draw_rect();





	}


	//This function checks to see if the player has started the game
	bool getFirstClick() {
		return firstClick;
	}

	//This function checks to see if the player has started the game
	void setFirstClick(bool x) {
		 firstClick = x;
	}

	//This function sets first click
	void enableBallMovement() {
		firstClick = true;
		ballVect[0].moveBallInRandomDir(); //Ball sub zero is always the ball on players paddle
	}

	//Paddle 
	void setPoint(float x, float y) {
		cout << "set position: " << x << endl;
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POINTS);
		glVertex2f(x, y);
		glEnd();
	}


	void draw_rect(void)
	{
		//glColor3f(r, g, b);			// Main color
		glColor3f(0.0, 0.0, 1.0);
		glRecti(xp, yp, xp2, yp2);
	}

	//Adds a brick to the brick vector.
	void addBrick(Brick b) {
		brickVect.push_back(b);
	}

	//Adds a ball to the ball vector.
	void addBall(Ball b) {
		ballVect.push_back(b);
	}

	//This function will draw a brick on the screen.
	//mBrick is a Brick object 
	void drawBrick(Brick mBrick) {
	



		glColor3f(0.2, 0.2, 1.0);
		glBegin(GL_POLYGON);
		glVertex2i(cwx * mBrick.getX(), (cwy * .9 - mBrick.getY()) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX() + 120, (cwy * .9 - mBrick.getY()) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX() + 120, (cwy * .9 - mBrick.getY() - 30) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX(), (cwy * .9 - mBrick.getY() - 30) - mBrick.getOffset());
		glEnd();


		glColor3f(0.0, 0.0, 0.8);
		glBegin(GL_POLYGON);
		glVertex2i(cwx * mBrick.getX(), (cwy * .9 - mBrick.getY()) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX() + 120, (cwy * .9 - mBrick.getY()) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX() + 120, (cwy * .9 - mBrick.getY() - 30 + 20) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX() + 0, (cwy * .9 - mBrick.getY() - 30 + 20) - mBrick.getOffset());
		glEnd();

		glColor3f(0.0, 0.0, 0.6);
		glBegin(GL_POLYGON);
		glVertex2i(cwx * mBrick.getX() + 0, (cwy * .9 - mBrick.getY()) - mBrick.getOffset() - 20);
		glVertex2i(cwx * mBrick.getX() + 120 - 0, (cwy * .9 - mBrick.getY()) - mBrick.getOffset() - 20);
		glVertex2i(cwx * mBrick.getX() + 120, (cwy * .9 - mBrick.getY() - 30) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX(), (cwy * .9 - mBrick.getY() - 30) - mBrick.getOffset());
		glEnd();

		glColor3f(0.0, 0.0, 0.4);
		glBegin(GL_POLYGON);
		glVertex2i(cwx * mBrick.getX() + 120 - 10, (cwy * .9 - mBrick.getY()) - mBrick.getOffset() - 10);
		glVertex2i(cwx * mBrick.getX() + 120, (cwy * .9 - mBrick.getY()) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX() + 120, (cwy * .9 - mBrick.getY() - 30) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX() + 120 - 10, (cwy * .9 - mBrick.getY() - 30) - mBrick.getOffset() + 10);
		glEnd();

		glColor3f(0.3, 0.3, 1.0);
		glBegin(GL_POLYGON);
		glVertex2i(cwx * mBrick.getX(), (cwy * .9 - mBrick.getY()) - mBrick.getOffset());
		glVertex2i(cwx * mBrick.getX() + 10, (cwy * .9 - mBrick.getY()) - mBrick.getOffset() - 10);
		glVertex2i(cwx * mBrick.getX() + 10, (cwy * .9 - mBrick.getY() - 30) - mBrick.getOffset() + 10);
		glVertex2i(cwx * mBrick.getX(), (cwy * .9 - mBrick.getY() - 30) - mBrick.getOffset());
		glEnd();


	}


	//This function will write text to the screen
	void writeText(int x, int y, std::string text, int charLen = 0) {
		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2d(x, y);
		for (int i = 0; i < charLen; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
		glEnd();
	}


	//Getters/setters
	void setXp(float xp) { this->xp = xp; }
	float getXp() { return this->xp; }
	void setYp(float yp) { this->yp = yp; }
	float getYp() { return this->yp; }
	void setXp2(float xp2) { this->xp2 = xp2; }
	float getXp2() { return this->xp2; }
	void setYp2(float yp2) { this->yp2 = yp2; }
	float getYp2() { return this->yp2; }
	void setXPos(float xPos) { this->xPos = xPos; }
	float getXPos() { return this->xPos; }
	void setYPos(float yPos) { this->yPos = yPos; }
	float getYPos() { return this->yPos; }
	void removeLive() { lives--; }
	void addLive() { lives++; }
	int getLives() { return lives; }
	void incBall() { BALL_SIZE++; }
	void decBall() { BALL_SIZE--; }


};



//Game var
Game* mGame = new Game(5, 1);



void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 1);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, cwx, 0.0, cwy);

	glutSetCursor(GLUT_CURSOR_NONE);

}


//This function handle when nothing is going on in the game
void idle() {
	Sleep(SPEED * 0.001);
	glutPostRedisplay();
}

void display() {
	Sleep(SPEED * 0.001);

	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPointSize(10.0);


	mGame->drawGame();
	glutSwapBuffers();
}


void keyPress(unsigned char key, int x, int y) {

	//Adds a ball to the screen
	if (key == 'b') {

		Ball tBall = Ball(mGame->getXp() + 10, mGame->getYp() + 10, 10, 0, 0);
		tBall.moveBallInRandomDir();
		mGame->addBall(tBall);
		glutPostRedisplay();
	} 
	
	//If button is hit, start a new game
	else if (key == '1') {
		 mGame = new Game(5, 1, 3);
	}

	else if (key == '2') {
		mGame = new Game(10, 1, 5);
	}

	else if (key == '3') {
		mGame = new Game(15, 1, 7);
	}

	else if (key == '4') {
		mGame = new Game(20, 1, 9);
	}

	else if (key == '5') {
		mGame = new Game(25, 1, 10);
	}

	else if (key == '6') {
		mGame = new Game(30, 1, 999);
	}

	//Adds a life to the player
	else if (key == 'l') {
		mGame->addLive();
	}

	//Adds a life to the player
	else if (key == '=') {
		
		SPEED -= 50;
		
		if (SPEED <= 0) { SPEED = 0; }
		cout << "PAUSE PER FRAME = " << SPEED << endl;

	}
	else if (key == '-') {
		SPEED += 50;
		cout << "PAUSE PER FRAME = " << SPEED << endl;
	}

	else if (key == '[') {
		mGame->decBall();
	}

	else if (key == ']') {
		mGame->incBall();
	}

}


//Mouse movement function
void mouse(int mouse, int state, int x, int y) {
	cout << "x position: " << mGame->getXp() << endl;

	switch (mouse) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			if (!(mGame->getFirstClick())) { //If the ball movement has NOT been enabled then enable it
				mGame->enableBallMovement();
			}

			//mGame->setXp(mGame->getXPos() - 100);
			//mGame->setXp2(mGame->getXPos() - 40);
			//glutPostRedisplay();
		}
		break;
	}

}

//Move paddle
void move(int x1, int y1) {
	POINT mouse;                        // Stores The X And Y Coords For The Current Mouse Position
	GetCursorPos(&mouse);                    // Gets The Current Cursor Coordisnates (Mouse Coordinates)


	//cout << mouse.x << "  " << (mouse.x - 109) << endl;
	//cout << mouse.x << " " << glutGet(GLUT_WINDOW_X) << endl;
	
	mGame->setXp(mouse.x - glutGet(GLUT_WINDOW_X) );
	mGame->setXp2(mouse.x - glutGet(GLUT_WINDOW_X) + 60);


	if (mGame->getXp() <= 0)
	{
		mGame->setXp(0);
		mGame->setXp2(60);
	}

	if (mGame->getXp() >= cwx - 60)
	{
		mGame->setXp(cwx - 60);
		mGame->setXp2(cwx);
	}

	//glutPostRedisplay();

	mGame->setXPos(x1);
	mGame->setYPos(y1);
}


int main(int argc, char** argv) {



	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH );
	glEnable(GL_DEPTH_TEST);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(cwx, cwy);
	glutCreateWindow("Final Project");

	init();

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyPress);
	glutPassiveMotionFunc(move);

	glutMainLoop();

	return 0;
}

