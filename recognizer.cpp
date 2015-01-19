#include "stdafx.h"

int is_rim[64] = {
    1,   1,   1,   1,   1,   1,   1,   1,
    1,   0,   0,   0,   0,   0,   0,   1,
    1,   0,   0,   0,   0,   0,   0,   1,
    1,   0,   0,   0,   0,   0,   0,   1,
    1,   0,   0,   0,   0,   0,   0,   1,
    1,   0,   0,   0,   0,   0,   0,   1,
    1,   0,   0,   0,   0,   0,   0,   1,
    1,   1,   1,   1,   1,   1,   1,   1
};


/* please note that this recognizer assumes that the position is legal,
   i.e. side to move is not in check */

int isDraw() {

    /* no pawns */
    if (!b.pawn_material[WHITE] && !b.pawn_material[BLACK] ) {

        /**********************************************************************
		*  We act only if both sides have at most one minor piece. To  guard  *
		*  against  the possibility of a helpmate in the corner, we  do  not  *
		*  claim  an immediate draw if the king of the weaker side stands on  *
		*  the edge of the board. Our recognizer catches bare kings, Km vs K  *
		*  and Km vs Km without kings on the edge.                            *
		**********************************************************************/

        if ( b.piece_material[WHITE] < 400 
		&&   b.piece_material[BLACK] < 400 
		&&   ( !is_rim[b.king_loc[WHITE] ] || b.piece_material[BLACK] == 0 ) 
		&&   ( !is_rim[b.king_loc[BLACK] ] || b.piece_material[WHITE] == 0 )
           )
            return 1;

    }

    /* default: no draw spotted */
    return 0;
}