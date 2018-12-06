/**
 * @file cbtypedata.c
 * @author Arthur Bouvier
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
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

char *getTabs() {
    char *tabs = malloc((CURR_TABSIZE+1) * sizeof(char));
    addTabs(tabs);
    tabs[CURR_TABSIZE] = 0;
    return tabs;
}
