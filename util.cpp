
#include "stdafx.h"
#include "0x88_math.h"
#include "transposition.h"

extern bool time_over;

unsigned int gettime();

/* retrieving pv from hash table */
int util_pv(char * pv) {

    sboard rootb = b;

    char best;
    smove m[256];
    int mcount = 0;

    for(U8 depth=1; depth<=sd.depth; depth++) {

        best = -1;
        tt_probe(0,0,0,&best);

        if (best == -1) break;

        mcount = movegen(m, 0xFF);

        for (int i=0; i<mcount; i++) {
            if (m[i].id == best) {
                move_make(m[i]);

                pv = algebraic_writemove(m[i], pv);
                pv[0] = ' ';
                pv++;

                break;
            }
        }
    }

    pv[0] = 0;
    b = rootb;
    return 0;
}

void perft_start(char * command) {
	int converted;
    unsigned int starttime = gettime();

    int depth;
    converted = sscanf(command + 6, "%d", &depth);

    printf("Performance Test\n");
    for (U8 d=1; d<=depth; d++) {
        printf("%d:\t%d\t%llu\n", (int) d, gettime()-starttime,perft(d));
    }
}

U64 perft(U8 depth) {

    U64 nodes = 0;

    if (depth == 0) return 1;

    smove m[256];
    int mcount = movegen(m, 0xFF);

    for (int i = 0; i < mcount; i++) {
        move_make(m[i]);

        if (!isAttacked(b.stm, b.KingLoc[!b.stm]))
            nodes += perft(depth - 1);

        move_unmake(m[i]);
    }

    return nodes;
}

void util_bench(char * command) {
	int converted;
    unsigned int starttime = gettime();

    int depth;
    converted = sscanf(command + 6, "%d", &depth);

    time_over = false;

    sd.myside = b.stm;
    sd.starttime = gettime();
    sd.depth = 0;
    sd.nodes = 0;

    task = TASK_SEARCH;

    chronos.depth = depth;
    chronos.flags = FDEPTH;

    search_run();

    task = TASK_NOTHING;

    if (gettime()-starttime < 1000) starttime = gettime() - 1000;

    printf("Nodes:\t%d\nTime:\t%d ms\nNPS:\t%llu\n", (int) sd.nodes, gettime()-starttime, sd.nodes / ((gettime()-starttime)/1000));

    return;
}

smove strToMove(char * a) {
    smove m;

    m.from = convert_a_0x88(a);
    m.to = convert_a_0x88(a+2);

    m.piece_from = b.pieces[m.from];
    m.piece_to = b.pieces[m.from];
    m.piece_cap = b.pieces[m.to];

    m.flags = 0;
    m.castle = 0;
    m.ep = -1;
    m.ply = 0;
    m.score = 0;

    /* default promotion to queen */

    if ( ( m.piece_to == PAWN ) &&
            ( ROW(m.to) == ROW_1 || ROW(m.to) == ROW_8 ) )
        m.piece_to = QUEEN;


    switch (a[4]) {
    case 'q':
        m.piece_to = QUEEN;
        a++;
        break;
    case 'r':
        m.piece_to = ROOK;
        a++;
        break;
    case 'b':
        m.piece_to = BISHOP;
        a++;
        break;
    case 'n':
        m.piece_to = KNIGHT;
        a++;
        break;
    }

    //castling
    if ((m.piece_from == KING) &&
            ((m.from == E1 && (m.to == G1 || m.to == C1)) ||
             (m.from == E8 && (m.to == G8 || m.to == C8)))) {
        m.flags = MFLAG_CASTLE;
    }

    /* ep
    	if the moving-piece is a Pawn, the square it moves to is empty and
    	it was a diagonal move it has to be an en-passant capture.
    */
    if ((m.piece_from == PAWN) &&
            (m.piece_cap == PIECE_EMPTY) &&
            ((abs(m.from-m.to)==15)||(abs(m.from-m.to)==17))) {
        m.flags = MFLAG_EPCAPTURE;
    }

    if ((m.piece_from == PAWN) && (abs(m.from - m.to) == 32)) {
        m.flags |= MFLAG_EP;
    }

    return m;
}

int algebraic_moves(char * a) {

    smove m;
    int found_match = 0;

    while (a[0]) {

        if (!((a[0] >= 'a') && (a[0] <= 'h'))) {
            a++;
            continue;
        }

        m = strToMove(a);

        found_match = move_isLegal(m);

        if ( found_match ) {

            move_make(m);

            if ( ( m.piece_from == PAWN ) ||
                    ( move_iscapt(m) ) ||
                    ( m.flags == MFLAG_CASTLE ) )
                b.rep_index = 0;

        } else {

            break;

        }

        a += 4;
        if (a[0] == 0) break;
        if (a[0] != ' ') a++;
    }

    return found_match;
}


//returns new pointer
char * algebraic_writemove(smove m, char * a) {
    char parray[5] = {0,'q','r','b','n'};

    convert_0x88_a(m.from, a);
    convert_0x88_a(m.to, a+2);
    a+=4;
    if (m.piece_to != m.piece_from) {
        a[0] = parray[m.piece_to];
        a++;
    }
    a[0] = 0;
    return a;
}

void convert_0x88_a(S8 sq, char * a) {
    a[0] = COL(sq) + 'a';
    a[1] = ROW(sq) + '1';
    a[2] = 0;
}

U8 convert_a_0x88(char * a) {
    S8 sq;
    sq = a[0] - 'a';
    sq += (a[1] - '1') * 16;

    return sq;
}
