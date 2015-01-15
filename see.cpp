
#include "stdafx.h"
#include "0x88_math.h"

/******************************************************************************
*  This is not yet proper static exchange evaluation, but an approximation    *
*  proposed by Harm Geert Mueller under the acronym BLIND (better, or lower   *
*  if not defended. As the name indicates, it detects only obviously good     *
*  captures, but it seems enough to improve move ordering.                    *
******************************************************************************/

int Blind (smove move) {
	int sq_to = move.to;
	int sq_fr = move.from;
	int pc_fr = b.pieces[sq_fr];
	int pc_to = b.pieces[sq_to];
	int val = e.SORT_VALUE[pc_to];

	/* captures by pawn do not lose material */
	if (pc_fr == PAWN) return 1;

	/* Captures "lower takes higher" (as well as BxN) are good by definition. */
	if (e.SORT_VALUE[pc_to] >= e.SORT_VALUE[pc_fr] - 50)
		return 1;

	/* Make the first capture, so that X-ray defender show up*/
	clearSq(sq_fr);

	/* Captures of undefended pieces are good by definition */
	if (!isAttacked(!b.stm, sq_to)) {
		fillSq(b.stm, pc_fr, sq_fr);
		return 1;
	}
	
	fillSq(b.stm, pc_fr, sq_fr);
	return 0; // of other captures we know nothing, Jon Snow!
}