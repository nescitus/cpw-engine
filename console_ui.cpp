#include "stdafx.h"

void printWelcome() {
	 printf(" CPW chess engine ");
	 printf( VERSION_STRING );
	 printf( "\n");
	 printf( COMPILE_STRING );
	 printf("\n");
	 printf(" created by some members of Chessprogramming Wiki \n");
	 printf(" http://chessprogramming.wikispaces.com/ \n\n");
	 printf(" type 'help' for a list of commands \n\n");
}

void printHelp() {
printf("------------------------------------------ \n");
printf("d        =  display current board position \n");
printf("bench n  =  test search speed to depth n \n");
printf("perft n  =  test perft numbers up to depth n \n");
printf("eval     =  display evaluation details \n");
printf("stat     =  display search statistics \n");
printf("go       =  play for the side to move \n");
printf("new      =  start a new game \n");
printf("sd n     =  set search depth to n plies \n");
printf("st n     =  set search time to n seconds \n");
printf("quit     =  exit CPW engine \n");
printf("\n");
printf("Please enter moves in algebraic notation (e2e4 d7d5 e4d5 d8d5 ... b7b8q) \n");
printf("or better use a GUI compliant with the UCI protocol \n");
printf("------------------------------------------ \n");
}

void printStats() {
	U64 nodes = sd.nodes + (sd.nodes == 0);

	printf("-----------------------------\n");
	printf("Nodes       : %d \n", sd.nodes);
	printf("Quiesc nodes: %d \n", sd.q_nodes);
	printf("Ratio       : %d %\n", sd.q_nodes * 100 / nodes);
	printf("-----------------------------\n");
}

void printSearchHeader() {
  	 printf("-------------------------------------------------------\n");
     printf( "ply      nodes   time score pv\n");
	 printf("-------------------------------------------------------\n");
}