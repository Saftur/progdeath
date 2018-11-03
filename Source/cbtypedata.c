/**
 * @file cbtypedata.c
 * @author Arthur Bouvier
 * @date 
 * 
 */
#include "cbtypedata.h"

int tablevel = 0;

void incTab() {
    tablevel++;
}

void decTab() {
    tablevel--;
}

void addTabs(char *txt) {
    for (int i = 0; i < tablevel; i++) {
#if TABS_AS_SPACES
        strncpy(txt, "    ", 4);
        txt += 4;
#else
        strncpy(txt, "\t", 1);
        txt += 1;
#endif
    }
}
