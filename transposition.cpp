
#include "stdafx.h"
#include "transposition.h"


extern bool time_over;

szobrist zobrist;

stt_entry * tt;
spawntt_entry * ptt;
sevaltt_entry * ett;

int tt_size = 0;
int ptt_size = 0;
int ett_size = 0;
	
/* function taken from Sungorus chess engine */
U64 rand64() {
	static U64 next = 1;

	next = next * 1103515245 + 12345;
	return next;
}

int tt_init() {

	/* fill the zobrist struct with random numbers */

	for (int pnr = 0; pnr <= 5; pnr++) {
		for (int cnr = 0; cnr <= 1; cnr++) {
			for (int snr = 0; snr <= 127; snr++) {
				zobrist.piecesquare[pnr][cnr][snr] = rand64();
			}
		}
	}

	zobrist.color = rand64();

	for (int castling = 0; castling <= 15; castling++) {
		zobrist.castling[castling] = rand64();
	}

	for (int ep = 0; ep <= 127; ep++) {
		zobrist.ep[ep] = rand64();
	}	

	return 0;
}

int tt_setsize(int size) {

	/* check if size is a power of 2
		if not, make it the next lower power of 2
		this allows for a faster access of the entry needed:

		as sizeof(stt_entry) in our case is 16 Bytes long (see definition of stt_entry),
		we are creating size / 16 tt entries. The idea of making the size a power of 2 
		is important when accessing the table. By 'anding' the hash value and the number
		of entries -1 (tt_size), we get a number in the range of 0 and the number of
		entries very quickly, this is used to index the entry.
	*/

	free(tt);

	if (size & (size - 1)) {

        size--;
        for (int i=1; i<32; i=i*2)
			size |= size >> i;
        size++;
		size>>=1;

	}

	if (size < 16) {
		tt_size = 0;
		return 0;
	}

	tt_size = (size / sizeof(stt_entry)) -1;
	tt = (stt_entry *) malloc(size);

	return 0;
}

int tt_probe(U8 depth, int alpha, int beta, char * best) {

	if (!tt_size) return INVALID;

	/************************************************************************** 
	*   Before  searching  a certain position, look whether we have  done  so *
	*   before. This is done by comparing the hashkey of the current position *
	*   to the hashkey of the specific tt entry. If they are the same, we may * 
	*   use the move stored in the hash table to enhance move ordering.  When *
    *   the previous search was not shallower then the one needed now, we may * 
	*   use  the  information present in the transposition table  to  replace * 
	*   search altogether. We do it only if the value found is in the  proper *
	*   relation  to alpha and beta, i.e. when it would cause a cutoff.  Some *
	*	programs  do  use these informations to narrow the window,  but  then *
	*   you have to be extra careful to avoid search instability.             *
	**************************************************************************/	

	stt_entry * phashe = &tt[b.hash & tt_size];

	if (phashe->hash == b.hash) {

		/*************************************************** 
		*   The  position  matches, so  we  may  retrieve  * 
		*   a move that will be used for sorting purposes  *
		***************************************************/

		*best = phashe->bestmove;

		/*************************************************** 
		*   Now test if we can retrieve position value     *
		*  ( saved depth greater than current depth )      *
		***************************************************/

        if (phashe->depth >= depth) {

            if (phashe->flags == TT_EXACT)
                return phashe->val;

            if ((phashe->flags == TT_ALPHA) && (phashe->val <= alpha))
                return alpha;

            if ((phashe->flags == TT_BETA) && (phashe->val >= beta))
                return beta;

        }

    }

    return INVALID;

}

void tt_save(U8 depth, int val, char flags, char best) {

	if (!tt_size) return;
	if (time_over) return;

    stt_entry * phashe = &tt[b.hash & tt_size];

	if ( (phashe->hash == b.hash) && (phashe->depth > depth) ) return;

    phashe->hash = b.hash;
    phashe->val = val;
	phashe->flags = flags;
    phashe->depth = depth;
	phashe->bestmove = best;
}

int ttpawn_setsize(int size) {

	/* see tt_setsize for more details */

	free(ptt);

	if (size & (size - 1)) {

        size--;
        for (int i=1; i<32; i=i*2)
			size |= size >> i;
        size++;
		size>>=1;

	}

	if (size < 8) {
		ptt_size = 0;
		return 0;
	}

	ptt_size = (size / sizeof(spawntt_entry)) -1;
	ptt = (spawntt_entry *) malloc(size);

	return 0;
}

int ttpawn_probe() {

	if (!ptt_size) return INVALID;

	spawntt_entry * phashe = &ptt[b.phash & ptt_size];

	if (phashe->hash == b.phash) return phashe->val;

    return INVALID;

}

void ttpawn_save(int val) {

	if (!ptt_size) return;

    spawntt_entry * phashe = &ptt[b.phash & ptt_size];

    phashe->hash = b.phash;
    phashe->val = val;
}

int tteval_setsize(int size) {

	/* see tt_setsize for more details */

	free(ett);

	if (size & (size - 1)) {

        size--;
        for (int i=1; i<32; i=i*2)
			size |= size >> i;
        size++;
		size>>=1;

	}

	if (size < 16) {
		ett_size = 0;
		return 0;
	}

	ett_size = (size / sizeof(sevaltt_entry)) -1;
	ett = (sevaltt_entry *) malloc(size);

	return 0;
}

int tteval_probe() {

	if (!ett_size) return INVALID;

	sevaltt_entry * phashe = &ett[b.hash & ett_size];

	if (phashe->hash == b.hash) return phashe->val;

    return INVALID;

}

void tteval_save(int val) {

	if (!ett_size) return;

    sevaltt_entry * phashe = &ett[b.hash & ett_size];

    phashe->hash = b.hash;
    phashe->val = val;
}