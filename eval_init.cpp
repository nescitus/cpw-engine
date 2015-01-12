#include "stdafx.h"
#include "0x88_math.h"


s_eval_data e;

// tables used for translating piece/square tables to internal 0x88 representation

int index_white[64] = {
    A8, B8, C8, D8, E8, F8, G8, H8,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A1, B1, C1, D1, E1, F1, G1, H1
};

int index_black[64] = {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8
};

/******************************************************************************
*                           PAWN PCSQ                                         *
*                                                                             *
*  Unlike TSCP, CPW generally doesn't want to advance its pawns. Its piece/   *
*  square table for pawns takes into account the following factors:           *
*                                                                             *
*  - file-dependent component, encouraging program to capture                 *
*    towards the center                                                       *
*  - small bonus for staying on the 2nd rank                                  *
*  - small bonus for standing on a3/h3                                        *
*  - penalty for d/e pawns on their initial squares                           *
*  - bonus for occupying the center                                           *
******************************************************************************/

int pawn_pcsq_mg[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
    -6,  -4,   1,   1,   1,   1,  -4,  -6,
    -6,  -4,   1,   2,   2,   1,  -4,  -6,
    -6,  -4,   2,   8,   8,   2,  -4,  -6,
    -6,  -4,   5,  10,  10,   5,  -4,  -6,
    -4,  -4,   1,   5,   5,   1,  -4,  -4,
    -6,  -4,   1, -24,  -24,  1,  -4,  -6,
     0,   0,   0,   0,   0,   0,   0,   0
};

int pawn_pcsq_eg[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
    -6,  -4,   1,   1,   1,   1,  -4,  -6,
    -6,  -4,   1,   2,   2,   1,  -4,  -6,
    -6,  -4,   2,   8,   8,   2,  -4,  -6,
    -6,  -4,   5,  10,  10,   5,  -4,  -6,
    -4,  -4,   1,   5,   5,   1,  -4,  -4,
    -6,  -4,   1, -24,  -24,  1,  -4,  -6,
     0,   0,   0,   0,   0,   0,   0,   0
};

/******************************************************************************
*    KNIGHT PCSQ                                                              *
*                                                                             *
*   - centralization bonus                                                    *
*   - rim and back rank penalty, including penalty for not being developed    *
******************************************************************************/

int knight_pcsq_mg[64] = {
    -8,  -8,  -8,  -8,  -8,  -8,  -8,  -8,
    -8,   0,   0,   0,   0,   0,   0,  -8,
    -8,   0,   4,   6,   6,   4,   0,  -8,
    -8,   0,   6,   8,   8,   6,   0,  -8,
    -8,   0,   6,   8,   8,   6,   0,  -8,
    -8,   0,   4,   6,   6,   4,   0,  -8,
    -8,   0,   1,   2,   2,   1,   0,  -8,
   -16, -12,  -8,  -8,  -8,  -8, -12,  -16
};

int knight_pcsq_eg[64] = {
    -8,  -8,  -8,  -8,  -8,  -8,  -8,  -8,
    -8,   0,   0,   0,   0,   0,   0,  -8,
    -8,   0,   4,   6,   6,   4,   0,  -8,
    -8,   0,   6,   8,   8,   6,   0,  -8,
    -8,   0,   6,   8,   8,   6,   0,  -8,
    -8,   0,   4,   6,   6,   4,   0,  -8,
    -8,   0,   1,   2,   2,   1,   0,  -8,
   -16, -12,  -8,  -8,  -8,  -8, -12,  -16
};

/******************************************************************************
*                BISHOP PCSQ                                                  *
*                                                                             *
*   - centralization bonus, smaller than for knight                           *
*   - penalty for not being developed                                         *
*   - good squares on the own half of the board                               *
******************************************************************************/

int bishop_pcsq_mg[64] = {
    -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,
    -4,   0,   0,   0,   0,   0,   0,  -4,
    -4,   0,   2,   4,   4,   2,   0,  -4,
    -4,   0,   4,   6,   6,   4,   0,  -4,
    -4,   0,   4,   6,   6,   4,   0,  -4,
    -4,   1,   2,   4,   4,   2,   1,  -4,
    -4,   2,   1,   1,   1,   1,   2,  -4,
    -4,  -4, -12,  -4,  -4, -12,  -4,  -4
};

int bishop_pcsq_eg[64] = {
    -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,
    -4,   0,   0,   0,   0,   0,   0,  -4,
    -4,   0,   2,   4,   4,   2,   0,  -4,
    -4,   0,   4,   6,   6,   4,   0,  -4,
    -4,   0,   4,   6,   6,   4,   0,  -4,
    -4,   1,   2,   4,   4,   2,   1,  -4,
    -4,   2,   1,   1,   1,   1,   2,  -4,
    -4,  -4, -12,  -4,  -4, -12,  -4,  -4
};

/******************************************************************************
*                        ROOK PCSQ                                            *
*                                                                             *
*    - bonus for 7th and 8th ranks                                            *
*    - penalty for a/h columns                                                *
*    - small centralization bonus                                             *
******************************************************************************/

int rook_pcsq_mg[64] = {
     5,   5,   5,   5,   5,   5,   5,   5,
    20,  20,  20,  20,  20,  20,  20,  20,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
     0,   0,   0,   2,   2,   0,   0,   0
};

int rook_pcsq_eg[64] = {
     5,   5,   5,   5,   5,   5,   5,   5,
    20,  20,  20,  20,  20,  20,  20,  20,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
     0,   0,   0,   2,   2,   0,   0,   0
};

/******************************************************************************
*                     QUEEN PCSQ                                              *
*                                                                             *
* - small bonus for centralization in the endgame                             *
* - penalty for staying on the 1st rank, between rooks in the midgame         *
******************************************************************************/

int queen_pcsq_mg[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   1,   1,   1,   1,   0,   0,
     0,   0,   1,   2,   2,   1,   0,   0,
     0,   0,   2,   3,   3,   2,   0,   0,
     0,   0,   2,   3,   3,   2,   0,   0,
     0,   0,   1,   2,   2,   1,   0,   0,
     0,   0,   1,   1,   1,   1,   0,   0,
    -5,  -5,  -5,  -5,  -5,  -5,  -5,  -5
};

int queen_pcsq_eg[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   1,   1,   1,   1,   0,   0,
     0,   0,   1,   2,   2,   1,   0,   0,
     0,   0,   2,   3,   3,   2,   0,   0,
     0,   0,   2,   3,   3,   2,   0,   0,
     0,   0,   1,   2,   2,   1,   0,   0,
     0,   0,   1,   1,   1,   1,   0,   0,
    -5,  -5,  -5,  -5,  -5,  -5,  -5,  -5
};

int king_pcsq_mg[64] = {
   -40, -30, -50, -70, -70, -50, -30, -40,
   -30, -20, -40, -60, -60, -40, -20, -30,
   -20, -10, -30, -50, -50, -30, -10, -20,
   -10,   0, -20, -40, -40, -20,   0, -10,
     0,  10, -10, -30, -30, -10,  10,   0,
    10,  20,   0, -20, -20,   0,  20,  10,
    30,  40,  20,   0,   0,  20,  40,  30,
    40,  50,  30,  10,  10,  30,  50,  40
};

int king_pcsq_eg[64] = {
   -72, -48, -36, -24, -24, -36, -48, -72,
   -48, -24, -12,   0,   0, -12, -24, -48,
   -36, -12,   0,  12,  12,   0, -12, -36,
   -24,   0,  12,  24,  24,  12,   0, -24,
   -24,   0,  12,  24,  24,  12,   0, -24,
   -36, -12,   0,  12,  12,   0, -12, -36,
   -48, -24, -12,   0,   0, -12, -24, -48,
   -72, -48, -36, -24, -24, -36, -48, -72
};

/******************************************************************************
*                     WEAK PAWNS PCSQ                                         *
*                                                                             *
*  Current version of CPW-engine does not differentiate between isolated and  *
*  backward pawns, using one  generic  cathegory of  weak pawns. The penalty  *
*  is bigger in the center, on the assumption that weak central pawns can be  *
*  attacked  from many  directions. If the penalty seems too low, please note *
*  that being on a semi-open file will come into equation, too.               *
******************************************************************************/

int weak_pawn_pcsq[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
   -10, -12, -14, -16, -16, -14, -12, -10,
   -10, -12, -14, -16, -16, -14, -12, -10,
   -10, -12, -14, -16, -16, -14, -12, -10,
   -10, -12, -14, -16, -16, -14, -12, -10,
   -10, -12, -14, -16, -16, -14, -12, -10,
   -10, -12, -14, -16, -16, -14, -12, -10,
     0,   0,   0,   0,   0,   0,   0,   0
};

int passed_pawn_pcsq[64] = {
     0,   0,   0,   0,   0,   0,   0,   0,
   140, 140, 140, 140, 140, 140, 140, 140,
    92,  92,  92,  92,  92,  92,  92,  92,
    56,  56,  56,  56,  56,  56,  56,  56,
    32,  32,  32,  32,  32,  32,  32,  32,
    20,  20,  20,  20,  20,  20,  20,  20,
    20,  20,  20,  20,  20,  20,  20,  20,
     0,   0,   0,   0,   0,   0,   0,   0
};

void setDefaultEval() {

    setBasicValues();
    setSquaresNearKing();
    setPcsq();
    readIniFile();
    correctValues();
}

void setBasicValues() {

    /********************************************************************************
    *  We use material values by IM Larry Kaufman with additional + 10 for a Bishop *
    *  and only +30 for a Bishop pair 	                                            *
    ********************************************************************************/

    e.PIECE_VALUE[KING]   = 0;
    e.PIECE_VALUE[QUEEN]  = 975;
    e.PIECE_VALUE[ROOK]   = 500;
    e.PIECE_VALUE[BISHOP] = 335;
    e.PIECE_VALUE[KNIGHT] = 325;
    e.PIECE_VALUE[PAWN]   = 100;

    e.BISHOP_PAIR   = 30;
    e.P_KNIGHT_PAIR = 8;
    e.P_ROOK_PAIR   = 16;

    /*************************************************
    * Values used for sorting captures are the same  *
    * as normal piece values, except for a king.     *
    *************************************************/

    for (int i = 0; i < 6; ++i) {
        e.SORT_VALUE[i] = e.PIECE_VALUE[i];
    }
    e.SORT_VALUE[KING] = SORT_KING;

    /* trapped and blocked pieces */
    e.P_KING_BLOCKS_ROOK   = 24;
    e.P_BLOCK_CENTRAL_PAWN = 24;
    e.P_BISHOP_TRAPPED_A7  = 150;
    e.P_BISHOP_TRAPPED_A6  = 50;
    e.P_KNIGHT_TRAPPED_A8  = 150;
    e.P_KNIGHT_TRAPPED_A7  = 100;

    /* minor penalties */
    e.P_C3_KNIGHT = 5;
    e.P_NO_FIANCHETTO = 4;

    /* king's defence */
    e.SHIELD_1 = 10;
    e.SHIELD_2 = 5;
    e.P_NO_SHIELD = 10;

    /* minor bonuses */
    e.ROOK_OPEN = 10;
    e.ROOK_HALF = 5;
    e.RETURNING_BISHOP = 20;
    e.FIANCHETTO = 4;
    e.TEMPO = 10;

    e.ENDGAME_MAT = 1300;
}

void setSquaresNearKing() {
    for (int i = 0; i < 128; ++i)
        for (int j = 0; j < 128; ++j)
        {

            e.sqNearK[WHITE][i][j] = 0;
            e.sqNearK[BLACK][i][j] = 0;

            if (IS_SQ(i) && IS_SQ(j)) {

                /* squares constituting the ring around both kings */
                if (j == i + NORTH || j == i + SOUTH 
				||  j == i + EAST  || j == i + WEST 
				||  j == i + NW    || j == i + NE 
				||  j == i + SW    || j == i + SE) {

                    e.sqNearK[WHITE][i][j] = 1;
                    e.sqNearK[BLACK][i][j] = 1;
                }

                /* squares in front of the white king ring */
                if (j == i + NORTH + NORTH 
				||  j == i + NORTH + NE 
				||  j == i + NORTH + NW)
                    e.sqNearK[WHITE][i][j] = 1;

                /* squares in front og the black king ring */
                if (j == i + SOUTH + SOUTH 
				||  j == i + SOUTH + SE 
				||  j == i + SOUTH + SW)
                    e.sqNearK[WHITE][i][j] = 1;
            }
        }
}


void setPcsq() {

    for (int i = 0; i < 64; ++i) {

        e.weak_pawn[WHITE][index_white[i]] = weak_pawn_pcsq[i];
        e.weak_pawn[BLACK][index_black[i]] = weak_pawn_pcsq[i];
        e.passed_pawn[WHITE][index_white[i]] = passed_pawn_pcsq[i];
        e.passed_pawn[BLACK][index_black[i]] = passed_pawn_pcsq[i];

        /* protected passers are slightly stronger than ordinary passers */

        e.protected_passer[WHITE][index_white[i]] = (passed_pawn_pcsq[i] * 10) / 8;
        e.protected_passer[BLACK][index_black[i]] = (passed_pawn_pcsq[i] * 10) / 8;

        /* now set the piece/square tables for each color and piece type */

        e.mgPst[PAWN][WHITE][index_white[i]] = pawn_pcsq_mg[i];
        e.mgPst[PAWN][BLACK][index_black[i]] = pawn_pcsq_mg[i];
        e.mgPst[KNIGHT][WHITE][index_white[i]] = knight_pcsq_mg[i];
        e.mgPst[KNIGHT][BLACK][index_black[i]] = knight_pcsq_mg[i];
        e.mgPst[BISHOP][WHITE][index_white[i]] = bishop_pcsq_mg[i];
        e.mgPst[BISHOP][BLACK][index_black[i]] = bishop_pcsq_mg[i];
        e.mgPst[ROOK][WHITE][index_white[i]] = rook_pcsq_mg[i];
        e.mgPst[ROOK][BLACK][index_black[i]] = rook_pcsq_mg[i];
        e.mgPst[QUEEN][WHITE][index_white[i]] = queen_pcsq_mg[i];
        e.mgPst[QUEEN][BLACK][index_black[i]] = queen_pcsq_mg[i];
        e.mgPst[KING][WHITE][index_white[i]] = king_pcsq_mg[i];
        e.mgPst[KING][BLACK][index_black[i]] = king_pcsq_mg[i];

        e.egPst[PAWN][WHITE][index_white[i]] = pawn_pcsq_eg[i];
        e.egPst[PAWN][BLACK][index_black[i]] = pawn_pcsq_eg[i];
        e.egPst[KNIGHT][WHITE][index_white[i]] = knight_pcsq_eg[i];
        e.egPst[KNIGHT][BLACK][index_black[i]] = knight_pcsq_eg[i];
        e.egPst[BISHOP][WHITE][index_white[i]] = bishop_pcsq_eg[i];
        e.egPst[BISHOP][BLACK][index_black[i]] = bishop_pcsq_eg[i];
        e.egPst[ROOK][WHITE][index_white[i]] = rook_pcsq_eg[i];
        e.egPst[ROOK][BLACK][index_black[i]] = rook_pcsq_eg[i];
        e.egPst[QUEEN][WHITE][index_white[i]] = queen_pcsq_eg[i];
        e.egPst[QUEEN][BLACK][index_black[i]] = queen_pcsq_eg[i];
        e.egPst[KING][WHITE][index_white[i]] = king_pcsq_eg[i];
        e.egPst[KING][BLACK][index_black[i]] = king_pcsq_eg[i];
    }
}

/* This function is meant to be used in conjunction with the *.ini file.
Its aim is to make sure that all the assumptions made within the program
are met.  */

void correctValues() {
    if (e.PIECE_VALUE[BISHOP] == e.PIECE_VALUE[KNIGHT])
        ++e.PIECE_VALUE[BISHOP];
}

void readIniFile() {
    FILE *cpw_init;
    char line[256];

    /* if the cpw.ini file does not exist, then exit */

    if ((cpw_init = fopen("cpw.ini", "r")) == NULL) {
        printf("Cannot open cpw.ini, default settings will be used \n");
        return;
    }

    /* process cpw.ini file line by line */

    while (fgets(line, 250, cpw_init)) {
        if (line[0] == ';') continue; // don't process comment lines
        processIniString(line);
    }
}

void processIniString(char line[250]) {
	int converted;
    /* piece values */
    if (!strncmp(line, "PAWN_VALUE", 10))
		converted = sscanf(line, "PAWN_VALUE %d", &e.PIECE_VALUE[PAWN]);
    else if (!strncmp(line, "KNIGHT_VALUE", 12))
		converted = sscanf(line, "KNIGHT_VALUE %d", &e.PIECE_VALUE[KNIGHT]);
    else if (!strncmp(line, "BISHOP_VALUE", 12))
		converted = sscanf(line, "BISHOP_VALUE %d", &e.PIECE_VALUE[BISHOP]);
    else if (!strncmp(line, "ROOK_VALUE", 10))
		converted = sscanf(line, "ROOK_VALUE %d", &e.PIECE_VALUE[ROOK]);
    else if (!strncmp(line, "QUEEN_VALUE", 11))
		converted = sscanf(line, "QUEEN_VALUE %d", &e.PIECE_VALUE[QUEEN]);

    /* piece pairs */
    else if (!strncmp(line, "BISHOP_PAIR", 11))
		converted = sscanf(line, "BISHOP_PAIR %d", &e.BISHOP_PAIR);
    else if (!strncmp(line, "PENALTY_KNIGHT_PAIR", 19))
		converted = sscanf(line, "PENALTY_KNIGHT_PAIR %d", &e.P_KNIGHT_PAIR);
    else if (!strncmp(line, "PENALTY_ROOK_PAIR", 17))
		converted = sscanf(line, "PENALTY_ROOK_PAIR %d", &e.P_KNIGHT_PAIR);

    /* pawn shield */
    else if (!strncmp(line, "SHIELD_1", 8))
		converted = sscanf(line, "SHIELD_1 %d", &e.SHIELD_1);
    else if (!strncmp(line, "SHIELD_2", 8))
		converted = sscanf(line, "SHIELD_2 %d", &e.SHIELD_2);
    else if (!strncmp(line, "PENALTY_NO_SHIELD", 17))
		converted = sscanf(line, "PENALTY_NO_SHIELD %d", &e.P_NO_SHIELD);

    /* major penalties */
    else if (!strncmp(line, "PENALTY_BISHOP_TRAPPED_A7", 25))
		converted = sscanf(line, "PENALTY_BISHOP_TRAPPED_A7 %d", &e.P_BISHOP_TRAPPED_A7);
    else if (!strncmp(line, "PENALTY_BISHOP_TRAPPED_A6", 25))
		converted = sscanf(line, "PENALTY_BISHOP_TRAPPED_A6 %d", &e.P_BISHOP_TRAPPED_A6);
    else if (!strncmp(line, "PENALTY_KNIGHT_TRAPPED_A8", 25))
		converted = sscanf(line, "PENALTY_KNIGHT_TRAPPED_A8 %d", &e.P_KNIGHT_TRAPPED_A8);
    else if (!strncmp(line, "PENALTY_KNIGHT_TRAPPED_A7", 25))
		converted = sscanf(line, "PENALTY_KNIGHT_TRAPPED_A7 %d", &e.P_KNIGHT_TRAPPED_A7);
    else if (!strncmp(line, "PENALTY_KING_BLOCKS_ROOK", 24))
		converted = sscanf(line, "PENALTY_KNIGHT_TRAPPED_A7 %d", &e.P_KING_BLOCKS_ROOK);
    else if (!strncmp(line, "PENALTY_BLOCKED_CENTRAL_PAWN", 28))
		converted = sscanf(line, "PENALTY_BLOCKED_CENTRAL_PAWN %d", &e.P_BLOCK_CENTRAL_PAWN);

    /* minor penalties */
    else if (!strncmp(line, "PENALTY_KNIGHT_BLOCKS_C", 23))
		converted = sscanf(line, "PENALTY_KNIGHT_BLOCKS_C %d", &e.P_C3_KNIGHT);
    else if (!strncmp(line, "PENALTY_NO_FIANCHETTO", 21))
		converted = sscanf(line, "PENALTY_NO_FIANCHETTO %d", &e.P_NO_FIANCHETTO);

    /* minor positional bonuses */
    else if (!strncmp(line, "ROOK_OPEN", 9))
		converted = sscanf(line, "ROOK_OPEN %d", &e.ROOK_OPEN);
    else if (!strncmp(line, "ROOK_HALF_OPEN", 14))
		converted = sscanf(line, "ROOK_HALF_OPEN %d", &e.ROOK_HALF);
    else if (!strncmp(line, "FIANCHETTO", 10))
		converted = sscanf(line, "FIANCHETTO %d", &e.FIANCHETTO);
    else if (!strncmp(line, "RETURNING_BISHOP", 16))
		converted = sscanf(line, "RETURNING_BISHOP %d", &e.RETURNING_BISHOP);
    else if (!strncmp(line, "TEMPO", 5))
		converted = sscanf(line, "TEMPO %d", &e.TEMPO);

    /* variables deciding about inner workings of evaluation function */
    else if (!strncmp(line, "ENDGAME_MATERIAL", 16))
		converted = sscanf(line, "ENDGAME_MATERIAL %d", &e.ENDGAME_MAT);
}