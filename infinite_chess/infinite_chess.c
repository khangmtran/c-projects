/*
 * Khang Tran
 * Program to see possible locations of king, pawn, or brigadier when have a x moves
 */
#include <stdlib.h>
#include <stdio.h>
/*
 * function to calculates the possible locations for king
 * return: total number of possible location
 * parameter: move: number of moves for chess piece
 */
int poLoK(int move){
	int total;
	total = (move*2+1) * (move*2+1);
	return total;
}

/*
 * function to calculates possible locations for brigadier
 * return: total number of possible location
 * parameter: move: number of moves for chess piece
 */
int poLoB(int move){
	int total;
	total = ((move + 1) * (move + 1)) + (move * move);
	return total;
}

/*
 * function to calculate possible locations for pawn
 * return: total nubmer of possible location
 * parameter: move: number of moves for chess piece
 */
int poLoP(int move){
	int total;
	total = move + 1;
	return total;
}
int main(){
	char inputChess = '\0';
	int inputMove = 0;
	printf("Enter piece type (k, b, p):\n");
	scanf(" %c", &inputChess); //get type, king, bridagier, or pawn
        printf("Enter number of moves:\n");
	scanf("%d", &inputMove);// get total moves of the input chess
	if(inputChess == 'k'){//Use different functions for different chess pieces
		int totalK = poLoK(inputMove);
	printf("possible locations: %d\n", totalK);}
	if(inputChess == 'b'){
	printf("possible locations: %d\n", poLoB(inputMove));}
	if(inputChess == 'p'){
	printf("possible locations: %d\n", poLoP(inputMove));}
	return 0;	
}
