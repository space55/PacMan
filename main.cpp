#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_ttf.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "..\..\..\..\Desktop\EamonnC++\AStarPacManV2\AStar.h"

using namespace std;

class AStar;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
SDL_Window* window;
SDL_Renderer* renderer;

TTF_Font* font;

char space = ' '; //Variable used to define the spaces
int score = 0; //Score integer
char player = 'e'; //PacMan variable
int boardx = 20; //X size of the board
int boardy = 23; //Y size of the board
char pos[20][23]; //Variable that stores the board as a 2D array
char posO[20][23]; //Unused
char posP[20][23]; //Array that stores the board, but is not edited
int x = 9; //PacMan's X Position
int y = 17; //PacMan's Y Position
AStar as;

bool AIMove = true; //Cheat determining if the AI should freeze or not

bool dotRegen = true; //Boolean determining if the dots should regen or not

int Bx = 8; //Blinky's X Position
int By = 10; //Blinky's Y Position

int BFx; //Blinky's Last X Position
int BFy; //Blinky's Last Y Position

int BHx = 8; //Blinky's Home X Position
int BHy = 10; //Blinky's Home Y Position

int Px = 10; //Pinky's X Position
int Py = 10; //Pinky's Y Position

int PFx; //Pinky's Last X Position
int PFy; //Pinky's Last Y Position

int PHx = 10; //Pinky's Home X Position
int PHy = 10; //Pinky's Home Y Position

int PTx = 10; //Pinky's Target X Position
int PTy = 10; //Pinky's Target Y Position

int Ix = 9; //Inky's X Position
int Iy = 10; //Inky's Y Position

int IFx; //Inky's Last X Position
int IFy; //Inky's Last Y Position

int IHx = 9; //Iinky's Home X Position
int IHy = 10; //Iinky's Home Y Position

int Cx = 11; //Clyde's X Position
int Cy = 10; //Clyde's Y Position

int CAx; //X Variable used to determine Clyde's distance from PacMan
int CAy; //Y Variable used to determine Clyde's distance from PacMan

int CFx; //Clyde's Last X Position
int CFy; //Clyde's Last Y Position

int CHx = 11; //Clyde's Home X Position
int CHy = 10; //Clyde's Home Y Position

int CTx = 11; //Clyde's Target X Position
int CTy = 10; // Clyde's Target Y Position

int nextMoveB[2]; //Array determining Blinky's next move using an A Star Pathfinding algorithm made by Tibbs

int nextMoveC[2]; //Array determining Clyde's next move using an A Star Pathfinding algorithm made by Tibbs

int nextMoveI[2]; //Array determining Inky's next move using an A Star Pathfinding algorithm made by Tibbs

int nextMoveP[2]; //Array determining Pinky's next move using an A Star Pathfinding algorithm made by Tibbs

int rndmPX; //Random X Position for Pinky
int rndmPY; //Random Y Position for Pinky

int rndmCX; //Random X Position for Clyde
int rndmCY; //Random Y Position for Clyde

bool goHome;
int timeHome = 0; //Times for which AI should move home

bool savehighscore; //Bool for saving high score if cheatcodes are not used

void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_RENDERER_ACCELERATED, &window, &renderer);

	font = TTF_OpenFont("consola.ttf", 50);

	SDL_StartTextInput();
}

void setup() {
	//This method writes the board from a text file to a 2D array for later use
	ifstream fs("PacManBoard.txt");
	for (int countery = 0; countery < boardy; countery++) {
		for (int counterx = 0; counterx < boardx; counterx++) {
			fs >> pos[counterx][countery]; //Main array
		}
	}
	for (int countery = 0; countery < boardy; countery++) {
		for (int counterx = 0; counterx < boardx; counterx++) {
			fs >> posO[counterx][countery]; //Unused
		}
	}
	for (int countery = 0; countery < boardy; countery++) {
		for (int counterx = 0; counterx < boardx; counterx++) {
			fs >> posP[counterx][countery]; //Point array
		}
	}
}
void calc() {
	pos[Cx][Cy] = 'C';
	pos[Ix][Iy] = 'I';
	pos[Px][Py] = 'P';
	pos[Bx][By] = 'B';

	srand(time(NULL));
	if (timeHome == 0) {
		goHome = false;
	}
	if (goHome == false && timeHome == 0 && AIMove) {

		//The next 4 lines create a random number to determine if the AI should move or not
		int goB = (rand()+1) % 3;
		int goP = (rand()-1) % 3;
		int goI = rand() % 3;
		int goC = rand() % 3;

		if (goB != 1) {
			if (Bx != BFx && By != BFy) {
				pos[Bx][By] = '.'; //Draws a dot in the last point they were at
			}

			//The next few lines determine where Blinky should move next
			as.nextMove(Bx, By, x, y, pos, boardx, boardy, nextMoveB);
			Bx = nextMoveB[0];
			By = nextMoveB[1];
			pos[Bx][By] = 'B';
			//These two lines set where blinky is to the place he last was for later use.
			BFx = Bx;
			BFy = By;
		}

		//The next 4 if statements determine if Clyde is less than 8 blocks away
		if (Cx > x) {
			CAx = (Cx - x);
		}
		else if (Cx < x) {
			CAx = (x - Cx);
		}
		if (Cy > y) {
			CAy = (Cy - y);
		}
		else if (Cy < y) {
			CAy = (y - Cy);
		}

		//This is to find where he goes if he is less than 8 blocks away
		if (CAx <= 8 && CAy <= 8) {
			rndmCX = rand() % boardx;
			rndmCY = rand() % boardy;
			while (rndmCX == '#' || rndmPY == '#') {
				rndmCX = rand() % boardx;
				rndmCY = rand() % boardy;
			}
			//Sets target position
			CTx = rndmCX;
			CTy = rndmCY;

			//If the random number determining if he goes is not two, he moves
			if (goC != 2) {
				//Same AI code as Blinky, only with Clyde's variables (Tells Clyde to move to a random spot)
				if (Cx != CFx && Cy != CFy) {
					pos[Cx][Cy] = '.';
				}
				as.nextMove(Cx, Cy, CTx, CTy, pos, boardx, boardy, nextMoveC);
				Cx = nextMoveC[0];
				Cy = nextMoveC[1];
				pos[Cx][Cy] = 'C';
				CFx = Cx;
				CFy = Cy;
			}
		}
		//If he is more than 8  blocks away
		else {
			if (goC != 2) {
				if (Cx != CFx && Cy != CFy) {
					pos[Cx][Cy] = '.';
				}
				as.nextMove(Cx, Cy, x, y, pos, boardx, boardy, nextMoveC);
				Cx = nextMoveC[0];
				Cy = nextMoveC[1];
				pos[Cx][Cy] = 'C';
				CFx = Cx;
				CFy = Cy;
			}
		}

		//If Inky's random number determing if he moves or not is not two, he moves
		if (goI != 2) {
			//Same AI code with Inky's variables
			if (Ix != IFx && Iy != IFy) {
				pos[Ix][Iy] = '.';
			}
			as.nextMove(Ix, Iy, x, y, pos, boardx, boardy, nextMoveI);
			Ix = nextMoveI[0];
			Iy = nextMoveI[1];
			pos[Ix][Iy] = 'I';
			IFx = Ix;
			IFy = Iy;
		}

		//If your score is less than thirty, Pinky moves to a random spot
		if (score < 30) {
			PTx = rndmPX;
			PTy = rndmPY;

			//If Pinky's random number determining if he moves or not is not 0, he moves
			if (goP != 0) {
				//Same AI code but with Pinky's variables
				if (Px != PFx && Py != PFy) {
					pos[Px][Py] = '.';
				}
				as.nextMove(Px, Py, PTx, PTy, pos, boardx, boardy, nextMoveP);
				Px = nextMoveP[0];
				Py = nextMoveP[1];
				pos[Px][Py] = 'P';
				PFx = Px;
				PFy = Py;
			}
		}
		//If your score is greater than zero, he moves toward you
		else {
			//If Pinky's random number determining if he moves or not is not 0, he moves
			if (goP != 0) {
				//Same AI code but with Pinky's variables
				if (Px != PFx && Py != PFy) {
					pos[Px][Py] = '.';
				}
				as.nextMove(Px, Py, x, y, pos, boardx, boardy, nextMoveP);
				Px = nextMoveP[0];
				Py = nextMoveP[1];
				pos[Px][Py] = 'P';
				PFx = Px;
				PFy = Py;
			}
		}
	}
	//If you picked up a big dot, they immidiately try to go to their home spot for ten moves
	else if (AIMove) {
		//Same AI code as everywhere else, but with their home position as the target
		if (Px != PFx && Py != PFy) {
			pos[Px][Py] = '.';
		}
		as.nextMove(Px, Py, PHx, PHy, pos, boardx, boardy, nextMoveP);
		Px = nextMoveP[0];
		Py = nextMoveP[1];
		pos[Px][Py] = 'P';
		PFx = Px;
		PFy = Py;

		if (Ix != IFx && Iy != IFy) {
			pos[Ix][Iy] = '.';
		}
		as.nextMove(Ix, Iy, IHx, IHy, pos, boardx, boardy, nextMoveI);
		Ix = nextMoveI[0];
		Iy = nextMoveI[1];
		pos[Ix][Iy] = 'I';
		IFx = Ix;
		IFy = Iy;

		if (Cx != CFx && Cy != CFy) {
			pos[Cx][Cy] = '.';
		}
		as.nextMove(Cx, Cy, CHx, CHy, pos, boardx, boardy, nextMoveC);
		Cx = nextMoveC[0];
		Cy = nextMoveC[1];
		pos[Cx][Cy] = 'C';
		CFx = Cx;
		CFy = Cy;

		if (Bx != BFx && By != BFy) {
			pos[Bx][By] = '.';
		}
		as.nextMove(Bx, By, BHx, BHy, pos, boardx, boardy, nextMoveB);
		Bx = nextMoveB[0];
		By = nextMoveB[1];
		pos[Bx][By] = 'B';
		BFx = Bx;
		BFy = By;

		timeHome--;
	}
	//Brute force removing all instances of any AI except for the one where their X and Y position stored in their variable is. Snakes were made before this was here. I saw Indiana Jones run through the wall trying to get out when those snakes were made...
	for (int counterx = 0; counterx < boardx; counterx++) {
		for (int countery = 0; countery < boardy; countery++) {
			if ((counterx != Bx || countery != By) && pos[counterx][countery] == 'B') {
				pos[counterx][countery] = '.';
			}
			if ((counterx != Cx || countery != Cy) && pos[counterx][countery] == 'C') {
				pos[counterx][countery] = '.';
			}
			if ((counterx != Ix || countery != Iy) && pos[counterx][countery] == 'I') {
				pos[counterx][countery] = '.';
			}
			if ((counterx != Px || countery != Py) && pos[counterx][countery] == 'P') {
				pos[counterx][countery] = '.';
			}
		}
	}
}

void square(int x, int y, int w, int h, Uint8 r = 0, Uint8 g = 200, Uint8 b = 0, Uint8 a = 0);
void text(int x, int y, string score);

SDL_Event event;
bool quit;
void find() {
	//All of this code is to determine which key was pressed and the action
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_TEXTINPUT:
				//event.text.text
				break;
			//Cheatcode for moving the AI to their home positions
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_HOME:
					if (event.key.keysym.mod & KMOD_SHIFT) {
						savehighscore = false;
						pos[Bx][By] = '.';
						pos[BHx][BHy] = 'B';
						Bx = BHx;
						By = BHy;

						pos[Cx][Cy] = '.';
						pos[CHx][CHy] = 'C';
						Cx = CHx;
						Cy = CHy;

						pos[Ix][Iy] = '.';
						pos[IHx][IHy] = 'I';
						Ix = IHx;
						Iy = IHy;

						pos[Px][Py] = '.';
						pos[PHx][PHy] = 'P';
						Px = PHx;
						Py = PHy;
					}
					break;
				case SDL_SCANCODE_END:
					if (event.key.keysym.mod & KMOD_SHIFT) {
						if (AIMove == true) {
							AIMove = false;
						}
						else if (AIMove == false) {
							AIMove = true;
						}
						savehighscore = false;
					}
					break;
				case SDL_SCANCODE_INSERT:
					if (event.key.keysym.mod & KMOD_SHIFT) {
						savehighscore = false;
						score = MAXINT-1      ;
					}
					break;
					//Key A
					case SDL_SCANCODE_A:
						//If PacMan's position was a dot, he gains a point.
						if (pos[x - 1][y] == '.') {
							score++;
						}
						//If it was a big dot, he gains ten points and the AI start to move to their home position for ten moves
						else if (pos[x - 1][y] == 'O') {
							score += 10;
							goHome = true;
							timeHome = 10;
						}
						//If it was a portal, PacMan is teleported
						else if (pos[x - 1][y] == '|') {
							pos[0][10] = '|';
							pos[x][y] = ' ';
							x = 18;
							y = 10;
							pos[x][y] = player;
							pos[19][10] = '|';
						}
						//This generates a space in any area PacMan just was
						if (pos[x - 1][y] == ' ' || pos[x - 1][y] == '.' || pos[x - 1][y] == 'O') {
							pos[x][y] = space;
							posP[x][y] = space;
							pos[x - 1][y] = player;
							x -= 1;
						}
						//Calls AI calculations
						calc();
						break;
					//Key W
					case SDL_SCANCODE_W:
						//If PacMan's position was a dot, he gains a point.
						if (pos[x][y - 1] == '.') {
							score++;
						}
						//If it was a big dot, he gains ten points and the AI start to move to their home position for ten moves
						else if (pos[x][y - 1] == 'O') {
							score += 10;
							goHome = true;
							timeHome = 10;
						}
						//No portal this time

						//This generates a space in any area PacMan just was
						if (pos[x][y - 1] == ' ' || pos[x][y - 1] == '.' || pos[x][y - 1] == 'O') {
							pos[x][y] = space;
							posP[x][y] = space;
							pos[x][y - 1] = player;
							y -= 1;
						}
						//Calls AI calculations
						calc();
						break;
					//Key S
					case SDL_SCANCODE_S:
						//If PacMan's position was a dot, he gains a point.
						if (pos[x][y + 1] == '.') {
							score++;
						}
						//If it was a big dot, he gains ten points and the AI start to move to their home position for ten moves
						else if (pos[x][y + 1] == 'O') {
							score += 10;
							goHome = true;
							timeHome = 10;
						}
						//No portal this time

						//This generates a space in any area PacMan just was
						if (pos[x][y + 1] == ' ' || pos[x][y + 1] == '.' || pos[x][y + 1] == 'O') {
							pos[x][y] = space;
							posP[x][y] = space;
							pos[x][y + 1] = player;
							y += 1;
						}
						//Calls AI calculations
						calc();
						break;
					//Key D
					case SDL_SCANCODE_D:
						//If PacMan's position was a dot, he gains a point.
						if (pos[x + 1][y] == '.') {
							score++;
						}
						//If it was a big dot, he gains ten points and the AI start to move to their home position for ten moves
						else if (pos[x + 1][y] == 'O') {
							score += 10;
							goHome = true;
							timeHome = 10;
						}
						//If it was a portal, PacMan is teleported
						else if (pos[x + 1][y] == '|') {
							pos[boardx][11] = '|';
							pos[x][y] = ' ';
							x = 1;
							y = 10;
							pos[x][y] = player;
						}
						//This generates a space in any area PacMan just was
						if (pos[x + 1][y] == ' ' || pos[x + 1][y] == '.' || pos[x + 1][y] == 'O') {
							pos[x][y] = space;
							posP[x][y] = space;
							pos[x + 1][y] = player;
							x += 1;
						}
						//Calls AI calculations
						calc();
						break;
					case SDL_SCANCODE_ESCAPE:
						quit = true;
						break;
					default:
						break;
				}
				break;
		}
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	//Renders everything on the board stored in pos[][]
	for (int countery = 0; countery < boardy; countery++) {
		for (int counterx = 0; counterx < boardx; counterx++) {
			switch (pos[counterx][countery]) {
				//Wall
				case '#':
					square(counterx * 20, countery * 20, 20, 20);
					break;
				//Dot
				case '.':
					square(counterx * 20 + 9, countery * 20 + 9, 2, 2);
					break;
				//Big dot
				case 'O':
					square(counterx * 20 + 6, countery * 20 + 6, 8, 8, 0, 0, 0);
					break;
				//PacMan
				case 'e':
					square(counterx * 20 + 6, countery * 20 + 6, 8, 8, 240, 230, 110);
					break;
				//Clyde
				case 'C':
					square(counterx * 20 + 6, countery * 20 + 6, 8, 8, 255, 180, 0);
					break;
				//Blinky
				case 'B':
					square(counterx * 20 + 6, countery * 20 + 6, 8, 8, 255, 0, 0);
					break;
				//Inky
				case 'I':
					square(counterx * 20 + 6, countery * 20 + 6, 8, 8, 135, 255, 247);
					break;
				//Pinky
				case 'P':
					square(counterx * 20 + 6, countery * 20 + 6, 8, 8, 255, 150, 255);
					break;
				//Portal
				case '|':
					square(counterx * 20 + 8, countery * 20, 2, 20);
					break;
				//Unused space code
				case ' ':
					/*if (pos[counterx][countery] != 'B' && pos[counterx][countery] != 'e' && pos[counterx][countery] != 'P' && pos[counterx][countery] != 'C' && pos[counterx][countery] != 'I' && pos[counterx][countery] != '.' && pos[counterx][countery] != 'O') {
						square(counterx * 20 + 6, countery * 20 + 6, 8, 8, 255, 255, 255);
					}*/
					break;
				//Alex's error code
				default:
					cerr << "ERROR Line 508" << endl;
			}
			//Places a point in every area that is supposed to have one
			switch (posP[counterx][countery]) {
			case '#':
				square(counterx * 20, countery * 20, 20, 20);
				break;
			case '.':
				square(counterx * 20 + 9, countery * 20 + 9, 2, 2);
				break;
			case 'O':
				square(counterx * 20 + 6, countery * 20 + 6, 8, 8, 0, 0, 0);
				break;
			case ' ':
				/*if (pos[counterx][countery] != 'B' && pos[counterx][countery] != 'e' && pos[counterx][countery] != 'P' && pos[counterx][countery] != 'C' && pos[counterx][countery] != 'I' && pos[counterx][countery] != '.' && pos[counterx][countery] != 'O') {
					square(counterx * 20 + 6, countery * 20 + 6, 8, 8, 255, 255, 255);
				}*/
				break;
			}
		}

	}

	//Prints score on screen
	stringstream scoreStream;
	scoreStream << "Score: " << score;

	text(50, 500, scoreStream.str().c_str());

	SDL_RenderPresent(renderer);
}

//Sets up squares
void square(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_Rect rectangleulares = {x, y, w, h};
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &rectangleulares);
}

//Sets up text
void text(int x , int y, string score) {
	SDL_Color color = {0, 0, 0, 0};

	SDL_Surface* tempSurface = TTF_RenderText_Solid(font, score.c_str(), color);
	SDL_Rect displayRect = { x, y, tempSurface->w, tempSurface->h };

	SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	
	SDL_RenderCopy(renderer, tempTexture, NULL, &displayRect);
	SDL_DestroyTexture(tempTexture);
}

//Organizes score
void org() {
	string dud;
	int number;
	int num = 0;
	ifstream sfs("PacManScores.txt");
	ofstream osfs;
	vector<int> scores;
	//Reads scores
	for (number = 0; !sfs.eof(); number++) {
		int temp;
		sfs >> temp;
		scores.push_back(temp);
	}

	//Sorts scores
	sort(scores.begin(), scores.end(), greater<int>());
	osfs.open("PacManScores.txt", ios_base::in);
	//Clears scores list
	for (int counter = 0; counter < num; counter++) {
		osfs << "" << endl;
	}
	//Writes scores
	for (int counter = 0; counter < scores.size(); counter++) {
		osfs << scores[counter] << endl;
	}
}

int main(int argc, char* args[])
{
	//Calls methods
	init();
	setup();
	find();
	while (!quit) {
		dotRegen = true;
		if ((score % 100) == 0) {
			for (int counterx = 0; counterx <= boardx; counterx++) {
				for (int countery = 0; countery <= boardy; countery++) {
					if (pos[1][2] != 'O' && dotRegen) {
						pos[1][2] = 'O';
						dotRegen = false;
					}
					else if (pos[18][2] != 'O' && dotRegen) {
						pos[18][2] = 'O';
						dotRegen = false;
					}
					else if (pos[1][16] != 'O' && dotRegen) {
						pos[1][16] = 'O';
						dotRegen = false;
					}
					else if (pos[18][16] != 'O' && dotRegen) {
						pos[18][16] = 'O';
						dotRegen = false;
					}
				}
			}
		}
		//Makes sure you don't die
		if (pos[x][y] == 'B' || pos[x][y] == 'P' || pos[x][y] == 'I' || pos[x][y] == 'C') {
			ofstream ofs;
			ofs.open("PacManScores.txt", ios_base::app);
			if (score > 0 && !savehighscore) {
				//Writes scores
				ofs << score << endl;
			}
			//Organizes
			org();
			return 0;
		}
		//Calls find
		find();
	}
	return 0;
}