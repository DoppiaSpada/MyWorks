#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

/*#define MAX_X 142
#define MAX_Y 36*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>

POINT base_cursor;

int MAX_X, MAX_Y;
int y = 11, /*ypre = 0,*/ y2 = 11 /*y2pre = 0*/; // coordinate sbarre
int xB = 20, /*xBpre = 0*/ yB = 11/* yBpre = 0*/; // coordinate palla
int speedX = -1, speedY = 0; // velocit� palla
int p1 = 0,p2 = 0; // punteggio giocatori
char quit = '0'; // se 1 esce dal gioco
char **prev_c; //campo da gioco
char **current_c; //campo da gioco

int m = 0;

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
	if ((xB == 2 || xB == 1) && yB <= y + 1 && yB >= y - 1) { // palla presa
		speedX = -speedX;
		speedY = rand() % 3 - 2;
		if (speedY == -2) speedY = 1;
	}
	else if(xB == 0){
		p2++; // aumenta punteggio giocatore 2
		reset();
		return 0;
	}

	if ((xB == MAX_X - 4 || xB == MAX_X - 3) && yB <= y2 + 1 && yB >= y2 - 1) { // palla presa
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
	return 0;
}

int start() {
	int i = 0, j = 0;

	//Attenzione ad usare la malloc (dimensionare bene le sizeof)
	//dimensiona il campo in base allo schermo
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	MAX_X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	MAX_Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	MAX_X -= ceil((float)MAX_X/6);
	MAX_Y -= ceil((float)MAX_Y/6);
	prev_c = (char**) malloc(MAX_Y * sizeof(char*));
	current_c = (char**) malloc(MAX_Y * sizeof(char*));
	for(m = 0; m < MAX_Y; m++){
		prev_c[m] = (char*) malloc(MAX_X * sizeof(char));
		current_c[m] = (char*) malloc(MAX_X * sizeof(char));
	}

	srand(time(NULL));
	reset(); // inizializza variabili

	setCurrent_c();

	for(int m = 0; m < MAX_Y; m++){
		memcpy(prev_c[m], current_c[m], MAX_X * sizeof(char));
	}

	char title[] = "PONG Z";
	char dedication[] = "Un piccolo dono ... che ricorda tempi lontani ... per Leonardo, Giacomo e Marco!";
	setCursorPosition((MAX_X + (int)ceil((float)MAX_X/6)) / 2 - 3, 2);
	for(m = 0; m < strlen(title); m++){
		printf("%c", title[m]);
		Sleep(500);
	}

	printf("\n");
	Sleep(500);
	printf("\n");
	Sleep(500);

	setCursorPosition((MAX_X + (int)ceil((float)MAX_X/6)) / 2 - 40, 3);
	for(m = 0; m < strlen(dedication); m++){
		printf("%c", dedication[m]);
		Sleep(200);
	}

	printf("\n\n");
	system("cls");

	printf("Premi 0 per avviare Pong\nPremi 1 per uscire da Pong\n\n");
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

	return 0;
}

int update() {

	scan(); // leggi input
	ballMove(); // muovi pallina
	setCurrent_c();
	draw(); // aggiorna grafica
	for(m = 0; m < MAX_Y; m++){
		memcpy(prev_c[m], current_c[m], MAX_X * sizeof(char));
	}

	Sleep(60); // aggiorna schermo ogni 60 millisecondi
	return 0;
}

int main(void) {
	start();
	while (quit == '0') {
		update();
	}

	exit(0);
}