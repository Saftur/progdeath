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

int is_directive(CodeBlock *block) {
    return block->type == CB_SETVAR || block->type == CB_IF;
}

int is_arg(CodeBlock *block) {
    return block->type == CB_VAR;
}

CodeBlock *empty_new() {
    return CodeBlock_new(CB_EMPTY, NULL, 0);
}
