#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#define MAX_X 42
#define MAX_Y 20

#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

POINT base_cursor;

int y = 11, /*ypre = 0,*/ y2 = 11 /*y2pre = 0*/; // coordinate sbarre
int xB = 20, /*xBpre = 0*/ yB = 11/* yBpre = 0*/; // coordinate palla
int speedX = -1, speedY = 0; // velocit� palla
int p1 = 0,p2 = 0; // punteggio giocatori
char quit = '0'; // se 1 esce dal gioco
char prev_c[MAX_Y][MAX_X]; //campo da gioco
char current_c[MAX_Y][MAX_X]; //campo da gioco

void setCursorVisibility(BOOL b) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = b;
    SetConsoleCursorInfo(hOut, &info);
}

void setCursorPosition(int x, int y)
{
   	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    fflush(stdout);
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

BOOL getCursorPosition(POINT *p){
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    fflush(stdout);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    BOOL ok = GetConsoleScreenBufferInfo(hOut, &csbi);
	if (ok) {
        p->x = csbi.dwCursorPosition.X;
        p->y = csbi.dwCursorPosition.Y;
    }
	return ok;
}

int scan() {
	char input = '0', input2 = '0', temp = '0';

	if (_kbhit() != 0) { // se viene premuto qualche tasto
		input = _getch();

		if(_kbhit() != 0)
			input2 = _getch();

		//se input contiene codici che dovrebbe contenere input 2 allora scambia e viceversa
		if (input ==  'i' || input == 'k') {
			if (input2 != 'w' && input2 != 's')
				input2 = input;
			else {
				temp = input;
				input = input2;
				input2 = temp;
			}
		}

		switch (input) {
			case 'w': /*ypre = y,*/ y = (y >= 1) ? --y : y; break; // se w allora ] sale
			case 's': /*ypre = y,*/ y = (y <= MAX_Y - 2) ? ++y : y; break; // se s allora ] scende
			case '1': quit = 1; break;
		}

		switch (input2) {
			case 'i': /*y2pre = y2,*/ y2 = (y2 >= 1) ? --y2 : y2; break;
			case 'k': /*y2pre = y2,*/ y2 = (y2 <= MAX_Y - 2) ? ++y2 : y2; break;
		}

		fflush(stdin);
	}

	return 0;
}

int reset() {
	/*ypre = y, y2pre = y2;*/
	y = MAX_Y / 2, y2 = MAX_Y / 2;
	/*xBpre = xB, yBpre = yB;*/
	xB = MAX_X / 2, yB = MAX_Y / 2;

	speedX = rand() % 2;
	if (speedX == 0) speedX = -1;
	speedY = rand() % 3 - 2;
	if (speedY == -2) speedY = 1;
	return 0;
}

int ballMove() {
	if ((xB == 2 || xB == 1) && yB == y) { // palla presa
		speedX = -speedX;
		speedY = rand() % 3 - 2;
		if (speedY == -2) speedY = 1;
	}
	else if(xB == 0){
		p2++; // aumenta punteggio giocatore 2
		reset();
		return 0;
	}

	if ((xB == MAX_X - 4 || xB == MAX_X - 3) && yB == y2) { // palla presa
		speedX = -speedX;
		speedY = rand() % 3 - 2;
		if (speedY == -2) speedY = 1;
	}
	else if(xB == MAX_X - 2) {
		p1++; // aumenta punteggio giocatore 1
		reset();
		return 0;
	}

	if (yB == 1 || yB == MAX_Y - 2) {
		speedY = -speedY;
	}

	/*xBpre = xB;
	yBpre = yB;*/

	xB += speedX;
	yB += speedY;

	return 0;
}

void setCurrent_c(){
	int k,m = 0;

	for(k = 0; k < MAX_Y; k++){
		for(m = 0; m < MAX_X; m++){

			if(m == MAX_X - 1){
				current_c[k][m] = '\n';
				continue;
			}

			if(k == 0 || k == MAX_Y - 1 || m == 0 || m == MAX_X - 2){
				current_c[k][m] = '#';
				continue;
			}
			/*else if(m == MAX_X - 1)
				current_c[k][m] = 'n';*/
			
			current_c[k][m] = ' ';
		}
	}

	current_c[y][1] = ']';
	current_c[y2][MAX_X - 3] = '[';
	current_c[yB][xB] = 'o';
}

int draw() {

	//int i = 0, j = -10; // contatori cicli for
	int i = 0, j = 0;
	//int righe_campo = 22, colonne_campo = 42; // dimensioni campo da gioco

	setCursorPosition(base_cursor.x, base_cursor.y - 2);
	printf("P1: %d                 P2: %d\n\n", p1, p2);

	for (i = 0; i < MAX_Y; i++)
	{
		for (j = 0; j < MAX_X; j++)
		{
			if (current_c[i][j] == prev_c[i][j]) {
				continue;
			}
			if(prev_c[i][j] == '#') continue;

			if(prev_c[i][j] == '[' && current_c[i][j] == 'o') continue;
			if(prev_c[i][j] == ']' && current_c[i][j] == 'o') continue;

			setCursorPosition(j + (int)base_cursor.x, i + (int)base_cursor.y);
			printf("%c", current_c[i][j]);
		}
	}

	fflush(stdout);

	/*c[ypre][0] = ' ';
	c[y][0] = ']';

	c[y2pre][39] = ' ';
	c[y2][39] = '[';

	c[yBpre][xBpre] = ' ';
	c[yB][xB] = 'o';

	
	printf("##########################################\n");
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[0][0], c[0][1], c[0][2], c[0][3], c[0][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[0][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[0][38], c[0][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[1][0], c[1][1], c[1][2], c[1][3], c[1][4], c[1][5], c[1][6], c[1][7], c[1][8], c[1][9], c[1][10], c[1][11], c[1][12], c[1][13], c[1][14], c[1][15], c[1][16], c[1][17], c[1][18], c[1][19], c[1][20], c[1][21], c[1][22], c[1][23], c[1][24], c[1][25], c[1][26], c[1][27], c[1][28], c[1][29], c[1][30], c[1][31], c[1][32], c[1][33], c[1][34], c[1][35], c[1][36], c[1][37], c[1][38], c[1][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[2][0], c[2][1], c[2][2], c[2][3], c[2][4], c[2][5], c[2][6], c[2][7], c[2][8], c[2][9], c[2][10], c[2][11], c[2][12], c[2][13], c[2][14], c[2][15], c[2][16], c[2][17], c[2][18], c[2][19], c[2][20], c[2][21], c[2][22], c[2][23], c[2][24], c[2][25], c[2][26], c[2][27], c[2][28], c[2][29], c[2][30], c[2][31], c[2][32], c[2][33], c[2][34], c[2][35], c[2][36], c[2][37], c[2][38], c[2][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[3][0], c[3][1], c[3][2], c[3][3], c[3][4], c[3][5], c[3][6], c[3][7], c[3][8], c[3][9], c[3][10], c[3][11], c[3][12], c[3][13], c[3][14], c[3][15], c[3][16], c[3][17], c[3][18], c[3][19], c[3][20], c[3][21], c[3][22], c[3][23], c[3][24], c[3][25], c[3][26], c[3][27], c[3][28], c[3][29], c[3][30], c[3][31], c[3][32], c[3][33], c[3][34], c[3][35], c[3][36], c[3][37], c[3][38], c[3][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[4][0], c[4][1], c[4][2], c[4][3], c[4][4], c[4][5], c[4][6], c[4][7], c[4][8], c[4][9], c[4][10], c[4][11], c[4][12], c[4][13], c[4][14], c[4][15], c[4][16], c[4][17], c[4][18], c[4][19], c[4][20], c[4][21], c[4][22], c[4][23], c[4][24], c[4][25], c[4][26], c[4][27], c[4][28], c[4][29], c[4][30], c[4][31], c[4][32], c[4][33], c[4][34], c[4][35], c[4][36], c[4][37], c[4][38], c[4][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[5][0], c[5][1], c[5][2], c[5][3], c[5][4], c[5][5], c[5][6], c[5][7], c[5][8], c[5][9], c[5][10], c[5][11], c[5][12], c[5][13], c[5][14], c[5][15], c[5][16], c[5][17], c[5][18], c[5][19], c[5][20], c[5][21], c[5][22], c[5][23], c[5][24], c[5][25], c[5][26], c[5][27], c[5][28], c[5][29], c[5][30], c[5][31], c[5][32], c[5][33], c[5][34], c[5][35], c[5][36], c[5][37], c[5][38], c[5][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[6][0], c[6][1], c[6][2], c[6][3], c[6][4], c[6][5], c[6][6], c[6][7], c[6][8], c[6][9], c[6][10], c[6][11], c[6][12], c[6][13], c[6][14], c[6][15], c[6][16], c[6][17], c[6][18], c[6][19], c[6][20], c[6][21], c[6][22], c[6][23], c[6][24], c[6][25], c[6][26], c[6][27], c[6][28], c[6][29], c[6][30], c[6][31], c[6][32], c[6][33], c[6][34], c[6][35], c[6][36], c[6][37], c[6][38], c[6][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[7][0], c[7][1], c[7][2], c[7][3], c[7][4], c[7][5], c[7][6], c[7][7], c[7][8], c[7][9], c[7][10], c[7][11], c[7][12], c[7][13], c[7][14], c[7][15], c[7][16], c[7][17], c[7][18], c[7][19], c[7][20], c[7][21], c[7][22], c[7][23], c[7][24], c[7][25], c[7][26], c[7][27], c[7][28], c[7][29], c[7][30], c[7][31], c[7][32], c[7][33], c[7][34], c[7][35], c[7][36], c[7][37], c[7][38], c[7][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[8][0], c[8][1], c[8][2], c[8][3], c[8][4], c[8][5], c[8][6], c[8][7], c[8][8], c[8][9], c[8][10], c[8][11], c[8][12], c[8][13], c[8][14], c[8][15], c[8][16], c[8][17], c[8][18], c[8][19], c[8][20], c[8][21], c[8][22], c[8][23], c[8][24], c[8][25], c[8][26], c[8][27], c[8][28], c[8][29], c[8][30], c[8][31], c[8][32], c[8][33], c[8][34], c[8][35], c[8][36], c[8][37], c[8][38], c[8][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[9][0], c[9][1], c[9][2], c[9][3], c[9][4], c[9][5], c[9][6], c[9][7], c[9][8], c[9][9], c[9][10], c[9][11], c[9][12], c[9][13], c[9][14], c[9][15], c[9][16], c[9][17], c[9][18], c[9][19], c[9][20], c[9][21], c[9][22], c[9][23], c[9][24], c[9][25], c[9][26], c[9][27], c[9][28], c[9][29], c[9][30], c[9][31], c[9][32], c[9][33], c[9][34], c[9][35], c[9][36], c[9][37], c[9][38], c[9][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[10][0], c[10][1], c[10][2], c[10][3], c[10][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[10][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[10][38], c[10][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[11][0], c[11][1], c[11][2], c[11][3], c[11][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[11][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[11][38], c[11][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[12][0], c[12][1], c[12][2], c[12][3], c[12][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[12][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[12][38], c[12][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[13][0], c[13][1], c[13][2], c[13][3], c[13][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[13][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[13][38], c[13][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[14][0], c[14][1], c[14][2], c[14][3], c[14][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[14][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[14][38], c[14][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[15][0], c[15][1], c[15][2], c[15][3], c[15][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[15][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[15][38], c[15][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[16][0], c[16][1], c[16][2], c[16][3], c[16][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[16][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[16][38], c[16][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[17][0], c[17][1], c[17][2], c[17][3], c[17][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[17][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[17][38], c[17][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[18][0], c[18][1], c[18][2], c[18][3], c[18][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[18][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[18][38], c[18][39]);
	printf("#%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c#\n", c[19][0], c[19][1], c[19][2], c[19][3], c[19][4], c[0][5], c[0][6], c[0][7], c[0][8], c[0][9], c[0][10], c[0][11], c[0][12], c[0][13], c[0][14], c[0][15], c[0][16], c[0][17], c[0][18], c[0][19], c[19][20], c[0][21], c[0][22], c[0][23], c[0][24], c[0][25], c[0][26], c[0][27], c[0][28], c[0][29], c[0][30], c[0][31], c[0][32], c[0][33], c[0][34], c[0][35], c[0][36], c[0][37], c[19][38], c[19][39]);
	printf("##########################################\n");*/

	/*printf("                               ");
	printf("############################################\n");
	for (i = 0; i < righe_campo; i++)
	{
		printf("                               ");
		printf("#");
		for (j = 0; j < colonne_campo; j++)
		{
			if (i == y && j == 0)
				printf("]");
			else if (i == y2 && j == 41)
				printf("[");
			else if (i == yB && j == xB)
				printf("o");
			else
				printf(" ");
		}
		printf("#\n");
	}
	printf("                               ");
	printf("############################################\n");*/

	return 0;
}

int start() {
	int i = 0, j = 0;
	srand(time(NULL));
	reset(); // inizializza variabili

	setCurrent_c();
	memcpy(prev_c, current_c, MAX_X * MAX_Y);

	printf("Premi 0 per avviare il gioco e 1 per uscire\n\n");
	scanf("%c%*c", &quit);
	fflush(stdin);
	system("cls");
	printf("Premi 1 per uscire\n");
	printf("P1: %d                 P2: %d\n\n", p1, p2);
	getCursorPosition(&base_cursor);
	setCursorVisibility(FALSE);

	for (i = 0; i < MAX_Y; i++)
	{
		for (j = 0; j < MAX_X; j++)
		{
			printf("%c", current_c[i][j]);
		}
	}


	/*for (i = 0; i < 20; i++) // campo vuoto
	{
		for (j = 0; j < 40; j++)
		{
			c[i][j] = ' ';
		}
	}*/

	return 0;
}

int update() {
	//system("cls"); // pulisci terminale
	//cls();

	scan(); // leggi input
	ballMove(); // muovi pallina
	setCurrent_c();

	draw(); // aggiorna grafica
	memcpy(prev_c, current_c, MAX_X * MAX_Y);

	Sleep(100); // aggiorna schermo ogni 100 millisecondi
	return 0;
}

int main(void) {
	start();
	while (quit == '0') {
		update();
	}
}