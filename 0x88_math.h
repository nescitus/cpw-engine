
/* row identifiers */

#define ROW_1   ( A1 >> 4 )
#define ROW_2   ( A2 >> 4 )
#define ROW_3   ( A3 >> 4 )
#define ROW_4   ( A4 >> 4 )
#define ROW_5   ( A5 >> 4 )
#define ROW_6   ( A6 >> 4 )
#define ROW_7   ( A7 >> 4 )
#define ROW_8   ( A8 >> 4 )

/* column identifiers */

#define COL_A  ( A1 & 7 )
#define COL_B  ( B1 & 7 )
#define COL_C  ( C1 & 7 )
#define COL_D  ( D1 & 7 )
#define COL_E  ( E1 & 7 )
#define COL_F  ( F1 & 7 )
#define COL_G  ( G1 & 7 )
#define COL_H  ( H1 & 7 )

/* vectors */

#define NORTH  16
#define NN    ( NORTH + NORTH )
#define SOUTH  -16
#define SS    ( SOUTH + SOUTH )
#define EAST  1
#define WEST  -1
#define NE    17
#define SW    -17
#define NW    15
#define SE    -15

/* generate square number from row and column */
#define SET_SQ(row,col) (row * 16 + col)

/* does a given number represent a square on the board? */
#define IS_SQ(x)  ( (x) & 0x88 ) ? (0) : (1)

/* get board column that a square is part of */
#define COL(sq)  ( (sq) & 7 )

/* get board row that a square is part of */
#define ROW(sq)  ( (sq) >> 4 )

/* determine if two squares lie on the same column */
#define SAME_COL(sq1,sq2) ( ( COL(sq1) == COL(sq2) ) ? (1) : (0) )

/* determine if two squares lie in the same row */
#define SAME_ROW(sq1,sq2) ( ( ROW(sq1) == ROW(sq2) ) ? (1) : (0) )