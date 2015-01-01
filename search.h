/*********************************************************************
*                              CPW-engine                            *
*           created by some members of Chessprogramming Wiki         *
*                                                                    *
* search.h  -  this file names functions coded within search.cpp.    *
* Outside that file they are represented by the interface function   *
* called search_run().                                               *
*********************************************************************/

void search_iterate();
int search_widen(int depth, int val);
void search_clearDriver();
int search_root(U8 depth, int alpha, int beta);
int Search(U8 depth, U8 ply, int alpha, int beta, int can_null, int is_pv);
void setKillers(smove m, U8 ply);
void ReorderMoves(smove * m, U8 mcount, U8 ply);
int info_currmove(smove m, int nr);
int info_pv(int val);
unsigned int countNps(unsigned int nodes, unsigned int time);
void ageHistoryTable();
int contempt();