/*

  CURHELL2.C
  ==========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  "Hello, world!", ncurses style (now in colour!)

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>

struct Ponto{
	int x, y;
}typedef Ponto;

void doneScreen(){
	WINDOW* done = newwin(3, 59, 11,11);
	box(done, 0,0);
	wrefresh(done);
	mvwprintw(done, 1, 23,"Concluído!");
	wrefresh(done);	
	sleep(3);
	werase(done);
	wrefresh(done);
	delwin(done);
	refresh();
}

/* char* waitCommand(){
	curs_set(1);
	WINDOW* output = newwin(4,79, 5, 1);
	mvwprintw(output, 1, 1, "Insira o comando de entrada");
	refresh();
	box(output, 0, 0);
	wrefresh(output);

	move(7,2);
	char *str = calloc(120, sizeof(char));
	getstr(str);
	mvwprintw(output, 2, 1, "%70s", " ");
	move(7,2);
	wrefresh(output);
	refresh();
	
	delwin(output);
	curs_set(0);
	refresh();
	return str;
} */

int mvwprintw_center(WINDOW* w, int linha, char* str){
	mvwprintw(w, linha, COLS/2 - strlen(str)/2, "%s", str);	
}

int mvwprintw_center_custom(WINDOW* w, int linha, int custom, char* str){
	mvwprintw(w, linha, COLS/2 - strlen(str)/2 - custom, "%s", str);	
}

char* waitCommand(){
	curs_set(0);
	WINDOW* output = newwin(19,79, 5, 1);
	wattron(output, A_BOLD);
	char* sub1 = "Navegação Interativa";
	char* sub2 = "Arvore XSD";
	mvwprintw_center(output, 1, sub1);
	wattroff(output, A_BOLD);
	wattron(output, A_DIM);
	mvwprintw_center_custom(output, 2, 1, sub2);
	refresh();
	box(output, 0, 0);
	wrefresh(output);
	wattroff(output, A_DIM);

	init_pair(4, COLOR_GREEN, -1);
	init_pair(8, COLOR_BLUE, -1);
	init_pair(9, COLOR_RED, -1);
	
	//Left

	for(int i = 0; i < 15; i++){
		mvwaddch(output, 5, i + 19, ACS_HLINE);
	}

	mvwaddch(output, 5, 19, ACS_ULCORNER);

	for(int i = 0; i < 1; i++){
		mvwaddch(output, i + 6, 19, ACS_VLINE);
	}

	for(int i = 0; i < 1; i++){
		mvwaddch(output, i + 10, 19, ACS_VLINE);
	}

	for(int i = 0; i < 15; i++){
		mvwaddch(output, 11, i + 12, ACS_HLINE);
	}

	mvwaddch(output, 11, 19, ACS_BTEE);
	mvwaddch(output, 11, 11, ACS_ULCORNER);
	mvwaddch(output, 11, 27, ACS_URCORNER);


	//Right
	
	for(int i = 0; i < 15; i++){
		mvwaddch(output, 5, i + 44, ACS_HLINE);
	}

	mvwaddch(output, 5,59, ACS_URCORNER);

	for(int i = 0; i < 1; i++){
		mvwaddch(output, i + 6, 59, ACS_VLINE);
	}

	for(int i = 0; i < 1; i++){
		mvwaddch(output, i + 10, 59, ACS_VLINE);
	}

	for(int i = 0; i < 15; i++){
		mvwaddch(output, 11, i + 52, ACS_HLINE);
	}

	mvwaddch(output, 11, 59, ACS_BTEE);
	mvwaddch(output, 11, 51, ACS_ULCORNER);
	mvwaddch(output, 11, 67, ACS_URCORNER);
	
	wrefresh(output);

	WINDOW* nodes[7];

	int altura = 3, largura = 11;
	Ponto coord[7] = {{9,35},  //Pai
                      {12,55},  //Primeira Linha - Direita
					  {12,15},  //Primeira Linha - Esquerda
					  {17,23},  //Linha 2 - Bloco Esquerdo - Direita
					  {17,7},   //Linha 2 - Bloco Esquerdo - Esquerdo
					  {17,63},  //Linha 2 - Bloco Direito - Direita
					  {17, 47}}; //Linha 2 - Bloco Direito - Esquerda

	for(int i = 0; i < 7; i++){
		nodes[i] = newwin(altura, largura, coord[i].x, coord[i].y);
		if(i % 2 == 0){
			wattron(nodes[i], COLOR_PAIR(8));
		}else{
			wattron(nodes[i], COLOR_PAIR(9));
		}

		box(nodes[i], 0, 0);
		wrefresh(nodes[i]);
		if(i % 2 == 0){
			wattroff(nodes[i], COLOR_PAIR(8));
		}else{
			wattroff(nodes[i], COLOR_PAIR(9));
		}	
	}

/* 	{
	WINDOW* treeWindow = newwin(3, 11, 9, 35);
	wattron(treeWindow, COLOR_PAIR(8));
	box(treeWindow, 0, 0);
	wrefresh(treeWindow);
	}


	
	
	{
	WINDOW* treeWindow = newwin(3, 11, 12, 55);
	wattron(treeWindow, COLOR_PAIR(9));
	box(treeWindow, 0, 0);
	wrefresh(treeWindow);
	refresh();
	}
		{
	WINDOW* treeWindow = newwin(3, 11, 12, 15);
	wattron(treeWindow, COLOR_PAIR(9));
	box(treeWindow, 0, 0);
	wrefresh(treeWindow);
	refresh();
	}

		{
	WINDOW* treeWindow = newwin(3, 11, 17, 23);
	wattron(treeWindow, COLOR_PAIR(8));
	box(treeWindow, 0, 0);
	wrefresh(treeWindow);
	refresh();
	}
		{
	WINDOW* treeWindow = newwin(3, 11, 17, 7);
	wattron(treeWindow, COLOR_PAIR(8));
	box(treeWindow, 0, 0);
	wrefresh(treeWindow);
	refresh();
	}

			{
	WINDOW* treeWindow = newwin(3, 11, 17, 63);
	wattron(treeWindow, COLOR_PAIR(8));
	box(treeWindow, 0, 0);
	wrefresh(treeWindow);
	refresh();
	}
		{
	WINDOW* treeWindow = newwin(3, 11, 17, 47);
	wattron(treeWindow, COLOR_PAIR(8));
	box(treeWindow, 0, 0);
	wrefresh(treeWindow);
	refresh();
	} */

	sleep(3000);
	refresh();
	return NULL;
}

void processCommand(char* str){
	WINDOW* processamento = newwin(4, 79, 5, 1);
	wattron(processamento, A_BOLD);
	mvwprintw(processamento, 1, 1, "Executando o comando %.10s", str);
	wattroff(processamento, A_BOLD);
	box(processamento, 0, 0);
	wrefresh(processamento);
	refresh();
	wattron(processamento, COLOR_PAIR(15) | A_BOLD);
	mvwaddch(processamento, 2, 1, '[');
	mvwaddch(processamento, 2, 77, ']');
	wrefresh(processamento);
	wattroff(processamento, COLOR_PAIR(15) | A_BOLD);
	refresh();
	init_pair(5,  COLOR_GREEN, -1);
	wattron(processamento, COLOR_PAIR(5));
	for (int n = 2; n < 77; n++){
		mvwaddch(processamento, 2, n, '#');

        usleep(100000 - (n*1000));
		wrefresh(processamento);
		refresh();
    }
	wattroff(processamento, COLOR_PAIR(5));

}

void loadInitial(){

}

int main(void) {

	#ifndef __NCURSES_H
		printf("A biblioteca não foi incluída");
	#else

    WINDOW * mainwin;


    /*  Initialize ncurses  */

    if ( (mainwin = initscr()) == NULL ) {
	fprintf(stderr, "Error initialising ncurses.\n");
	exit(EXIT_FAILURE);
    }

    start_color();                    

	use_default_colors();
	init_pair(16,  -1,     -1);
	wbkgd(mainwin, COLOR_PAIR(16));
	WINDOW* mainText = newwin(4,79,1,1);	
 	init_pair(15,  COLOR_YELLOW, -1);
	box(mainText, 0, 0);
	wattron(mainwin, A_BOLD);
	wattron(mainText, COLOR_PAIR(15)); 
	refresh();
	wrefresh(mainText);
	mvaddstr(1, 2, "Trabalho de Estrutura de Dados");
	refresh();
	wattroff(mainwin, A_BOLD);
	wattroff(mainwin, A_UNDERLINE);
	wattron(mainText, A_ITALIC);
	wattroff(mainText, COLOR_PAIR(15));
	mvwprintw(mainText, 2, 1, "Modo Interativo");
	wattroff(mainText, A_ITALIC);
	wattron(mainText, COLOR_PAIR(15));
	mvwprintw(mainText, 1, 1, "Copyright 2019 © Matheus Bastos, Melvi Trevisan");
	wrefresh(mainText);
	refresh();
	

	char* str = waitCommand();
	while(strcmp(str, "sai") != 0){
		processCommand(str);
		str = waitCommand();

	}

	delwin(mainwin);
    endwin();
    refresh();

	return 0;



	 
	

    //int h, w;
   	//getmaxyx(stdscr, h, w);
    //printf(" %d %d %d %d\n", h, w, LINES, COLS);
	//80 24


    /*  Make sure we are able to do what we want. If
	has_colors() returns FALSE, we cannot use colours.
	COLOR_PAIRS is the maximum number of colour pairs
	we can use. We use 13 in this program, so we check
	to make sure we have enough available.   
	            */
/* 	sleep(1);
    if ( has_colors() && COLOR_PAIRS >= 13 ) {	

	int n = 1; */


	/*  Initialize a bunch of colour pairs, where:

	        init_pair(pair number, foreground, background);

	    specifies the pair.                                  */

	/* init_pair(1,  COLOR_RED,     COLOR_BLACK);
	init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
	init_pair(3,  COLOR_YELLOW,  COLOR_BLACK);
	init_pair(4,  COLOR_BLUE,    COLOR_BLACK);
	init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6,  COLOR_CYAN,    COLOR_BLACK);
	init_pair(7,  COLOR_BLUE,    COLOR_WHITE);
	init_pair(8,  COLOR_WHITE,   COLOR_RED);
	init_pair(9,  COLOR_BLACK,   COLOR_GREEN);
	init_pair(10, COLOR_BLUE,    COLOR_YELLOW);
	init_pair(11, COLOR_WHITE,   COLOR_BLUE);
	init_pair(12, COLOR_WHITE,   COLOR_MAGENTA);
	init_pair(13, COLOR_BLACK,   COLOR_CYAN); */


	/*  Use them to print of bunch of "Hello, world!"s  */

/* 	while ( n <= 13 ) {
	    color_set(n, mainwin);
	    mvaddstr(6 + n, 32, " Evandro é um imbecil");
        wmove(mainwin, 6 + n, 32);
        refresh();
        usleep(150000);
	    n++;
	} */
/*     }
 */

    /*  Refresh the screen and sleep for a
	while to get the full screen effect  */

    refresh();
    sleep(3000000);
    

    /*  Clean up after ourselves  */

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;

	#endif
}
