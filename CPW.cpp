
#include "stdafx.h"
#include "transposition.h"
#include "0x88_math.h"

int com_init();
void setDefaultEval();
int tt_init();
int tt_setsize(int size);



enum etask {
    TASK_NOTHING,
    TASK_SEARCH
} extern task;


void time_uci_go(char * command);

int main() {
    com_init();
    setDefaultEval();
    tt_init();
    tt_setsize(0x4000000);     //64m
    ttpawn_setsize(0x1000000); //16m
    tteval_setsize(0x2000000); //32m
    initBook();

    board_loadFromFen(STARTFEN);

    for(;;) {

        if (task == TASK_NOTHING) {
            com();
        } else {
            search_run();
            task = TASK_NOTHING;
        }

    }

}