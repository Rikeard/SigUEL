#include "gui.h"

struct Ponto{
	int x, y;
}typedef Ponto;

enum TREE_TYPE{
	QUADRA, HIDRANTE, SEMAFORO, RADIOBASE, PREDIO, MURO
}typedef TREE_TYPE;

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

void cleanTree(WINDOW* output){
	//19,79, 5, 1)
	for(int y = 3; y < 18; y++){
		for(int x = 1; x < 78; x++){
			mvwaddch(output, y, x, ' ');
		}
	}

	wrefresh(output);
}

void renderTreeLine(WINDOW* output, bool *isActive){

	if(isActive[0]){
		if(isActive[1]){
			for(int i = 0; i < 15; i++){
				mvwaddch(output, 5, i + 44, ACS_HLINE);
			}

			mvwaddch(output, 5,59, ACS_URCORNER);

			for(int i = 0; i < 1; i++){
				mvwaddch(output, i + 6, 59, ACS_VLINE);
			}

			if(isActive[6] || isActive[5]){
				for(int i = 0; i < 1; i++){
					mvwaddch(output, i + 10, 59, ACS_VLINE);
				}

				if(isActive[6]){
					for(int i = 0; i < 7; i++){
						mvwaddch(output, 11, i + 52, ACS_HLINE);
					}
					mvwaddch(output, 11, 51, ACS_ULCORNER);
					mvwaddch(output, 11, 59, ACS_LRCORNER);

				}

				if(isActive[5]){
					for(int i = 0; i < 7; i++){
						mvwaddch(output, 11, i + 60, ACS_HLINE);
					}
					mvwaddch(output, 11, 67, ACS_URCORNER);
					mvwaddch(output, 11, 59, ACS_LLCORNER);
				}

				if(isActive[5] && isActive[6])
					mvwaddch(output, 11, 59, ACS_BTEE);

			}
		}
	}
		
	if(isActive[2]){
		for(int i = 0; i < 15; i++){
			mvwaddch(output, 5, i + 19, ACS_HLINE);
		}

		mvwaddch(output, 5, 19, ACS_ULCORNER);

		for(int i = 0; i < 1; i++){
			mvwaddch(output, i + 6, 19, ACS_VLINE);
		}

		if(isActive[4] || isActive[3]){
			for(int i = 0; i < 1; i++){
				mvwaddch(output, i + 10, 19, ACS_VLINE);
			}

			if(isActive[4]){
				for(int i = 0; i < 7; i++){
					mvwaddch(output, 11, i + 12, ACS_HLINE);
				}

				mvwaddch(output, 11, 11, ACS_ULCORNER);
				mvwaddch(output, 11, 19, ACS_LRCORNER);
			}

			if(isActive[3]){
				for(int i = 0; i < 7; i++){
					mvwaddch(output, 11, i + 20, ACS_HLINE);
				}
				
				mvwaddch(output, 11, 27, ACS_URCORNER);
				mvwaddch(output, 11, 19, ACS_LLCORNER);
			}

			if(isActive[4] && isActive[3])
				mvwaddch(output, 11, 19, ACS_BTEE);

		}

	}
	

	wrefresh(output);
}

char* getStringOf(RBTree tree, Node node, TREE_TYPE tipo){
	Wall wall;
	char* s;
	switch (tipo) {
		case QUADRA:
			return Block_GetCep(RBTreeN_GetValue(tree, node));
			break;
		case HIDRANTE:
			return Equip_GetID(RBTreeN_GetValue(tree, node));
			break;
		case SEMAFORO:
			return Equip_GetID(RBTreeN_GetValue(tree, node));
			break;
		case RADIOBASE:
			return Equip_GetID(RBTreeN_GetValue(tree, node));
			break;
		case PREDIO:
			return Building_GetKey(RBTreeN_GetValue(tree, node));
			break;
		case MURO:
			wall = RBTreeN_GetValue(tree, node);
			s = calloc(128, sizeof(char));
			sprintf(s, "(%.1lf, %.2lf) (%.2lf, %.2lf)",
					Wall_GetX1(wall), Wall_GetY1(wall),
					Wall_GetX2(wall), Wall_GetY2(wall));
			return s;
			break;
		default:
			return NULL;
	}
}

void setNodeData(RBTree arvore, Node node, bool* isActive, bool* isRed, char** chaves, Node* nodeList, int index, TREE_TYPE tipo){
	isActive[index] = true;
	chaves[index] = getStringOf(arvore, node, tipo);
	isRed[index] = RBTreeN_GetColor(node) == RED;
	nodeList[index] = node;
}


void renderTree(Ponto *coord, WINDOW** nodes, char** chaves, bool *isRed, bool *isActive, int selected){
	for(int i = 0; i < 7; i++){
		if(!isActive[i]){
			werase(nodes[i]);
			wrefresh(nodes[i]);
			continue;
		}

		if(!isRed[i]){
			wattron(nodes[i], COLOR_PAIR(8));
		}else{
			wattron(nodes[i], COLOR_PAIR(9));
		}

		if(i == selected)
			wattron(nodes[i], A_STANDOUT);

		box(nodes[i], 0, 0);
		wrefresh(nodes[i]);
		if(!isRed[i]){
			wattroff(nodes[i], COLOR_PAIR(8));
		}else{
			wattroff(nodes[i], COLOR_PAIR(9));
		}

		char* str = chaves[i];
/* 		str = calloc(10, sizeof(char));
		sprintf(str, "%d", i); */
		int strl = strlen(str);
		if(strl < 10){
			int x = (10-strl);
			x = (x % 2 == 0 ? x/2 : x/2 + 1);

			mvwprintw(nodes[i], 1, 1, "%s", "         ");
			//Completa com espaço em branco 
			//depois ele da repaint com o texto por cima
			
			mvwprintw(nodes[i], 1, x, "%s", str);
		}else{
			mvwprintw(nodes[i], 1, 1, "%.7s..", str);
		}
		wrefresh(nodes[i]);

		if(i == selected)
			wattroff(nodes[i], A_STANDOUT);

	}
}

int processInputMovement(int atual, int read, bool* isActive){
    int ret = atual;
	if(read == 'A'){ //Up
		if(atual == 0)
			ret = -1;
		if(atual == 1 || atual == 2)
			ret = 0;
		if(atual == 4 || atual == 3)
			ret = 2;
		if(atual == 6 || atual == 5)
			ret = 1;
	}else if(read == 'B'){ //Down
		if(atual == 5 || atual == 6 || atual == 4 || atual == 3){
			ret = -1;
		}
	}else if(read == 'C'){ //Right
		if(atual == 0)
			ret = 1;
		if(atual == 1)
			ret = 5;
		if(atual == 2)
			ret = 3;
		if(atual == 4)
			ret = 3;
		if(atual == 6)
			ret = 5;
	}else if(read == 'D'){ //Left
		if(atual == 0)
			ret = 2;
		if(atual == 1)
			ret = 6;
		if(atual == 2)
			ret = 4;
		if(atual == 3)
			ret = 4;
		if(atual == 5)
			ret = 6;
	}

	if(isActive[ret] || ret == -1)
        return ret;
    else
        return atual;

}

void loadTreeData(RBTree tree, Node root, bool isActive[7], bool isRed[7], char* chaves[7], Node* nodeList[7], TREE_TYPE tipo){

	for(int i = 0; i < 7; i++){
		isActive[i] = false;
		isRed[i] = false;
		chaves[i] = NULL;
	}

	if(root == NULL){
		isActive[0] = true;
		chaves[0] = NULL;
	}else{
		setNodeData(tree, root, isActive, isRed, chaves, nodeList, 0, tipo);

		Node left = RBTreeN_GetLeftChild(tree, root);
		Node right = RBTreeN_GetRightChild(tree, root);

		if(right != NULL){
			setNodeData(tree, right, isActive, isRed, chaves, nodeList, 1, tipo);

			Node sonLeft = RBTreeN_GetLeftChild(tree, right);
			Node sonRight = RBTreeN_GetRightChild(tree, right);

			if(sonLeft != NULL)
				setNodeData(tree, sonLeft, isActive, isRed, chaves, nodeList, 6, tipo);

			if(sonRight != NULL)
				setNodeData(tree, sonRight, isActive, isRed, chaves, nodeList, 5, tipo);
		}

		if(left != NULL){
			setNodeData(tree, left, isActive, isRed, chaves, nodeList, 2, tipo);

			Node sonLeft = RBTreeN_GetLeftChild(tree, left);
			Node sonRight = RBTreeN_GetRightChild(tree, left);

			if(sonLeft != NULL)
				setNodeData(tree, sonLeft, isActive, isRed, chaves, nodeList, 4, tipo);

			if(sonRight != NULL)
				setNodeData(tree, sonRight, isActive, isRed, chaves, nodeList, 3, tipo);
		}
	}
}

char* waitCommand(){
	curs_set(0);
	noecho();

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
	
	TREE_TYPE tipo = QUADRA;

	RBTree tree = getBlockTree();

	Node root = RBTree_GetRoot(tree);

	WINDOW* nodes[7];

	int altura = 3, largura = 11;

	Ponto coord[7] = {{9,35},  //Pai
                      {12,55},  //Primeira Linha - Direita
					  {12,15},  //Primeira Linha - Esquerda
					  {17,23},  //Linha 2 - Bloco Esquerdo - Direita
					  {17,7},   //Linha 2 - Bloco Esquerdo - Esquerdo
					  {17,63},  //Linha 2 - Bloco Direito - Direita
					  {17, 47}}; //Linha 2 - Bloco Direito - Esquerda

	for(int i = 0; i < 7; i++)
		nodes[i] = newwin(altura, largura, coord[i].x, coord[i].y);
	
	char* chaves[7];
	bool isRed[7];
	Node nodeList[7];
	bool isActive[7];
	int selected = 0;

	loadTreeData(tree, root, isActive, isRed, chaves, nodeList, tipo);

	renderTreeLine(output, isActive);
	renderTree(coord, nodes, chaves, isRed, isActive, selected);

	while(true){
		int ch = getch();
		if(ch == '\033' && getch() == '['){ //Comando de escape
			switch(getch()){
				case 'A': //up
				{
					int resp = processInputMovement(selected, 'A', isActive);
					if(resp == -1){
						Node actual = nodeList[0];
						Node parent = RBTreeN_GetParent(tree, actual);
						if(parent != NULL){
							parent = RBTreeN_GetParent(tree, parent);
							if(parent!= NULL){
								loadTreeData(tree, parent, isActive, isRed, chaves, nodeList, tipo);
								selected = 0;
								for(int i = 0; i < 7; i++){
									if(nodeList[i] == actual)
										selected = i;
								}
							}
						}
							
					}else{
						selected = resp;
					}
					break;
				}
				case 'B': //down
				{
					int resp = processInputMovement(selected, 'B', isActive);
					if(resp == -1){
						loadTreeData(tree, nodeList[selected], isActive, isRed, chaves, nodeList, tipo);
						selected = 0;
					}else{
						selected = resp;
					}
					break;
				}
				case 'C': //right
				{
					selected = processInputMovement(selected, 'C', isActive);
					break;
				}
				case 'D': //left
				{
					selected = processInputMovement(selected, 'D', isActive);
					break;
				}
			}
		}else if(ch == 27){
			break;
		}

		cleanTree(output);
		renderTreeLine(output, isActive);
		renderTree(coord, nodes, chaves, isRed, isActive, selected);
	}
/* 
	for(int i = 0; i < 7; i++){
		nodes[i] = newwin(altura, largura, coord[i].x, coord[i].y);
		if(!isRed[i]){
			wattron(nodes[i], COLOR_PAIR(8));
		}else{
			wattron(nodes[i], COLOR_PAIR(9));
		}

		if(i == selected)
			wattron(nodes[i], A_STANDOUT);

		box(nodes[i], 0, 0);
		wrefresh(nodes[i]);
		if(!isRed[i]){
			wattroff(nodes[i], COLOR_PAIR(8));
		}else{
			wattroff(nodes[i], COLOR_PAIR(9));
		}

		char* str = chaves[i];
		int strl = strlen(str);
		if(strl < 10){
			int x = (10-strl);
			x = (x % 2 == 0 ? x/2 : x/2 + 1);

			mvwprintw(nodes[i], 1, 1, "%s", "         ");
			//Completa com espaço em branco 
			//depois ele da repaint com o texto por cima
			
			mvwprintw(nodes[i], 1, x, "%s", str);
		}else{
			mvwprintw(nodes[i], 1, 1, "%.7s..", str);
		}
		wrefresh(nodes[i]);

		if(i == selected)
			wattroff(nodes[i], A_STANDOUT);

	}
 */


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

void startGui(Files *files, char *baseDir, char *entryFileName) {

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
