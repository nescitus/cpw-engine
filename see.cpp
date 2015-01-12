
#include "stdafx.h"
#include "0x88_math.h"

int See(smove move) {
	int sq_to = move.to;
	int sq_fr = move.from;
	int pc_fr = b.pieces[sq_fr];
	int pc_to = b.pieces[sq_to];
	int val = e.SORT_VALUE[pc_to];

	/* captures by pawn do not lose material */
	if (pc_fr == PAWN) return 0;

	/* Captures "lower takes higher" (as well as BxN) are good by definition. */
	if (e.SORT_VALUE[pc_to] >= e.SORT_VALUE[pc_fr] - 50)
		return 0;

	clearSq(sq_fr);
	
	if (!isAttacked(!b.stm, sq_to)) {    // no defender
		fillSq(b.stm, pc_fr, sq_fr);
		return val;
	}
	else {
		if (!isAttacked(b.stm, sq_to)) { // no second attacker
		   fillSq(b.stm, pc_fr, sq_fr);
		   return val - e.SORT_VALUE[pc_fr];
		}
	}

	fillSq(b.stm, pc_fr, sq_fr);
	return 0; // We know nothing, Jon Snow!
}