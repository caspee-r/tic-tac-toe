#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

enum Symbol{
	X = 'X',
	O = 'O',
};

int check_winner(int positions[],int sym){
	int winner = 0;
	int i;
	int counter;
	for (i = 1;i < 10;i= i+3){
		if (positions[i-1] == sym && positions[i] == sym && positions[i+1] == sym){
			winner = 1;
			return winner;
		}
	}
	for (i = 0;i<=4;i++){
		if (positions[i] == sym && positions[i+3] == sym && positions[i+6] == sym){
			winner = 1;
			return winner;
		}
	}
	if (positions[0] == sym && positions[4] == sym && positions[8] == sym){
		winner = 1;
		return winner;
	}
	else if(positions[2] == sym && positions[4] == sym && positions[6] == sym){
		winner = 1;
		return winner;
	}
	return winner;
}


int redraw(int positions[],char sym){

	printf("|---|---|---|\n");
	int status = 1;
	int i;
	for (i = 1; i < 10; i++) {
		if (positions[i-1] == 0){
			if (status){
				printf("|");
				status = 0;
			}
			printf("%3d|",i );
			if (i % 3 == 0){
				printf("\n");
				printf("|---|---|---|\n");
				status = 1;
			}

		}
		else  {
			if (status){
				printf("|");
				status = 0;
			}
			printf("%3c|",positions[i-1]);
			if (i % 3 == 0){
				printf("\n");
				printf("|---|---|---|\n");
				status = 1;
			}
		}

	}

}



int main(void)
{
	int positions[9];
	int i2;
	for (i2 = 0; i2 < 10; i2++){
		positions[i2] = 0;
	}
	int i;
	int c = 1;
	int turn;
	printf("TIC TAC TOE\n");
	printf("|---|---|---|\n");
	for (i = 0; i < 3;i++){
		printf("|%3d|%3d|%3d|\n",c,c+1,c+2);
		printf("|---|---|---|\n") ;
		c = c + 3;
	}
	//choosing X or O?
	char user_symbol, my_symbol;
	printf(" X or O ?\n");
	scanf("%c", &user_symbol);
	
	switch (user_symbol) {
	case X: {
		my_symbol = O;
		break;
	}
	case O: {
		my_symbol = X;
		break;
	}
	case 'x': {
		my_symbol = O;
		user_symbol = X;
		break;
	}
	case 'o': {
		my_symbol = X;
		user_symbol = O;
		break;
	}
	default:
		return 0;
	}


	// who is gonna start first?
	srand(time(NULL));
	turn = rand() % 2;

	int counter = 0;
	short int pos;
	while (counter <= 9) {
		if (turn){
			
			printf("Your Turn\n");
			printf("pos:");
			scanf("%d", &pos);
			if ((0 < pos < 10) && (positions[pos-1] != user_symbol) && (positions[pos-1] != my_symbol)){
				turn = 0;
				positions[pos-1] = user_symbol;
				counter++;
				redraw(positions,user_symbol);
		 		if (counter >= 5 && check_winner(positions,user_symbol) ){
					printf("YOU WON\n");
					break;
				}
			}
			else {
				printf("Invalid Input\n");
				continue;
			}
		}
		else {
			printf("My Turn\n");
			turn = 1;
			int k;
			for (k = 0; k < 10;k++){
				if (positions[k] == 0){
					positions[k] = my_symbol;
					break;
				}

			}
			counter++;
			sleep(1);
			redraw(positions,my_symbol);
			if (counter >= 5 && check_winner(positions,my_symbol) ){
				printf("I WON\n");
				break;
			}


		}
	}
	return 1;
}

