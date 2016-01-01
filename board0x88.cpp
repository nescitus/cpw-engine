
#include "stdafx.h"
#include "0x88_math.h"
#include "transposition.h"

sboard b;


void clearBoard() {

    //reset b struct

    for (int sq=0; sq<128; sq++) {
        b.pieces[sq] = PIECE_EMPTY;
        b.color[sq]  = COLOR_EMPTY;
		for (int cl = 0; cl < 2; cl++) {
			b.pawn_ctrl[cl][sq] = 0;
		}
    }

    b.castle = 0;
    b.ep     = -1;
    b.ply    = 0;
    b.hash   = 0;
    b.phash  = 0;
    b.stm    = 0;
    b.rep_index=0;

    // reset perceived values

    b.piece_material[WHITE] = 0;
    b.piece_material[BLACK] = 0;
    b.pawn_material[WHITE]  = 0;
    b.pawn_material[BLACK]  = 0;
    b.pcsq_mg[WHITE] = 0;
    b.pcsq_mg[BLACK] = 0;
    b.pcsq_eg[WHITE] = 0;
    b.pcsq_eg[BLACK] = 0;


    // reset counters

    for (int i=0; i<6; i++) {
        b.piece_cnt[ WHITE ] [ i ] = 0;
        b.piece_cnt[ BLACK ] [ i ] = 0;
    }

	for (int i = 0; i<8; i++) {
		b.pawns_on_file[WHITE][i] = 0;
		b.pawns_on_file[BLACK][i] = 0;
		b.pawns_on_rank[WHITE][i] = 0;
		b.pawns_on_rank[BLACK][i] = 0;
	}
}

/******************************************************************************
* fillSq() and clearSq(), beside placing a piece on a given square or erasing *
* it,  must  take care for all the incrementally updated  stuff:  hash  keys, *
* piece counters, material and pcsq values, pawn-related data, king location. *
******************************************************************************/

void fillSq(U8 color, U8 piece, S8 sq) {

    // place a piece on the board
    b.pieces[sq] = piece;
    b.color[sq] = color;

    // update king location
    if (piece == KING)
        b.king_loc[color] = sq;

	/**************************************************************************
	* Pawn structure changes slower than piece position, which allows reusing *
	* some data, both in pawn and piece evaluation. For that reason we do     *
	* some extra work here, expecting to gain extra speed elsewhere.          *
	**************************************************************************/

    if ( piece == PAWN ) {
        // update pawn material
        b.pawn_material[color] += e.PIECE_VALUE[piece];

        // update pawn hashkey
        b.phash ^= zobrist.piecesquare[piece][color][sq];

		// update counter of pawns on a given rank and file
		++b.pawns_on_file[color][COL(sq)];
		++b.pawns_on_rank[color][ROW(sq)];

		// update squares controlled by pawns
		if (color == WHITE) {
			if (IS_SQ(sq + NE)) b.pawn_ctrl[WHITE][sq + NE]++;
			if (IS_SQ(sq + NW)) b.pawn_ctrl[WHITE][sq + NW]++;
		} else {
			if (IS_SQ(sq + SE)) b.pawn_ctrl[BLACK][sq + SE]++;
			if (IS_SQ(sq + SW)) b.pawn_ctrl[BLACK][sq + SW]++;
		}
    }
    else {
        // update piece material
        b.piece_material[color] += e.PIECE_VALUE[piece];
    }

    // update piece counter
    b.piece_cnt[color][piece]++;

    // update piece-square value
    b.pcsq_mg[color] += e.mgPst[piece][color][sq];
    b.pcsq_eg[color] += e.egPst[piece][color][sq];

    // update hash key
    b.hash ^= zobrist.piecesquare[piece][color][sq];
}


void clearSq(S8 sq) {

    // set intermediate variables, then do the same
    // as in fillSq(), only backwards

    U8 color = b.color[sq];
    U8 piece = b.pieces[sq];

    b.hash ^= zobrist.piecesquare[piece][color][sq];

    if ( piece == PAWN ) {
		// update squares controlled by pawns
		if (color == WHITE) {
			if (IS_SQ(sq + NE)) b.pawn_ctrl[WHITE][sq + NE]--;
			if (IS_SQ(sq + NW)) b.pawn_ctrl[WHITE][sq + NW]--;
		} else {
			if (IS_SQ(sq + SE)) b.pawn_ctrl[BLACK][sq + SE]--;
			if (IS_SQ(sq + SW)) b.pawn_ctrl[BLACK][sq + SW]--;
		}

		--b.pawns_on_file[color][COL(sq)];
		--b.pawns_on_rank[color][ROW(sq)];
        b.pawn_material[color] -= e.PIECE_VALUE[piece];
        b.phash ^= zobrist.piecesquare[piece][color][sq];
    }
    else
        b.piece_material[color] -= e.PIECE_VALUE[piece];

    b.pcsq_mg[color] -= e.mgPst[piece][color][sq];
    b.pcsq_eg[color] -= e.egPst[piece][color][sq];

    b.piece_cnt[color][piece]--;

    b.pieces[sq] = PIECE_EMPTY;
    b.color[sq] = COLOR_EMPTY;
}


int board_loadFromFen(char * fen) {

    clearBoard();
    clearHistoryTable();

    char * f = fen;

    char col = 0;
    char row = 7;

    do {
        switch( f[0] ) {
        case 'K':
            fillSq(WHITE, KING, SET_SQ(row, col));
            col++;
            break;
        case 'Q':
            fillSq(WHITE, QUEEN, SET_SQ(row, col));
            col++;
            break;
        case 'R':
            fillSq(WHITE, ROOK, SET_SQ(row, col));
            col++;
            break;
        case 'B':
            fillSq(WHITE, BISHOP, SET_SQ(row, col));
            col++;
            break;
        case 'N':
            fillSq(WHITE, KNIGHT, SET_SQ(row, col));
            col++;
            break;
        case 'P':
            fillSq(WHITE, PAWN, SET_SQ(row, col));
            col++;
            break;
        case 'k':
            fillSq(BLACK, KING, SET_SQ(row, col));
            col++;
            break;
        case 'q':
            fillSq(BLACK, QUEEN, SET_SQ(row, col));
            col++;
            break;
        case 'r':
            fillSq(BLACK, ROOK, SET_SQ(row, col));
            col++;
            break;
        case 'b':
            fillSq(BLACK, BISHOP, SET_SQ(row, col));
            col++;
            break;
        case 'n':
            fillSq(BLACK, KNIGHT, SET_SQ(row, col));
            col++;
            break;
        case 'p':
            fillSq(BLACK, PAWN, SET_SQ(row, col));
            col++;
            break;
        case '/':
            row--;
            col=0;
            break;
        case '1':
            col+=1;
            break;
        case '2':
            col+=2;
            break;
        case '3':
            col+=3;
            break;
        case '4':
            col+=4;
            break;
        case '5':
            col+=5;
            break;
        case '6':
            col+=6;
            break;
        case '7':
            col+=7;
            break;
        case '8':
            col+=8;
            break;
        };

        f++;
    } while ( f[0] != ' ' );

    f++;

    if (f[0]=='w') {
        b.stm = WHITE;
    } else {
        b.stm = BLACK;
        b.hash ^= zobrist.color;
    }

    f+=2;

    do {
        switch( f[0] ) {
        case 'K':
            b.castle |= CASTLE_WK;
            break;
        case 'Q':
            b.castle |= CASTLE_WQ;
            break;
        case 'k':
            b.castle |= CASTLE_BK;
            break;
        case 'q':
            b.castle |= CASTLE_BQ;
            break;
        }

        f++;
    } while (f[0] != ' ' );

    b.hash ^= zobrist.castling[b.castle];

    f++;

    if (f[0] != '-') {
        b.ep = convert_a_0x88(f);
        b.hash ^= zobrist.ep[b.ep];
    }

    do {
        f++;
    }
    while (f[0] != ' ' );
    f++;
	int ply = 0;
	int converted;
    converted = sscanf(f, "%d", &ply);
	b.ply = (unsigned char) ply;

    b.rep_index = 0;
    b.rep_stack[b.rep_index] = b.hash;

    return 0;
}


void board_display() {

    S8 sq;

    char parray[3][7] = { {'K','Q','R','B','N','P'},
        {'k','q','r','b','n','p'},
        { 0 , 0 , 0 , 0 , 0,  0, '.'}
    };

    printf("   a b c d e f g h\n\n");

    for (S8 row=7; row>=0; row--) {

        printf("%d ", row+1);

        for (S8 col=0; col<8; col++) {
            sq = SET_SQ(row, col);
            printf(" %c",parray[b.color[sq]][b.pieces[sq]] );
        }

        printf("  %d\n", row+1);

    }

    printf("\n   a b c d e f g h\n\n");
}