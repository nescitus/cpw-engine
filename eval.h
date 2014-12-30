
/* king safety*/
int wKingShield();
int bKingShield();

/* pawn structure */
int getPawnScore();
int evalPawnStructure();
int EvalPawn(S8 sq, S8 side);
void EvalKnight(S8 sq, S8 side);
void EvalBishop(S8 sq, S8 side);
void EvalRook(S8 sq, S8 side);
void EvalQueen(S8 sq, S8 side);
int isPawnSupported(S8 sq, S8 side);
int isWPSupported(S8 sq);
int isBPSupported(S8 sq);

/* pattern detection */
void blockedPieces();
