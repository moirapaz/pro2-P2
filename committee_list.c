/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Moira Paz Fernández     LOGIN 1: moira.paz
 * AUTHOR 2: Álvaro Francos Martínez LOGIN 2: alvaro.francos
 * GROUP: 4.1
 * DATE: 02 / 04 / 26
 */


#include "committee_list.h"
#include <string.h>
#include <stdio.h>

void createEmptyListC(tListC *L) {
    L->count = 0;
}

bool isEmptyListC(tListC L) {
    return L.count == 0;
}

tPosC firstC(tListC L) {
    (void)L;
    return 0;
}

tPosC lastC(tListC L) {
    return L.count - 1;
}

tPosC nextC(tPosC p, tListC L) {
    if (p + 1 < L.count) return p + 1;
    return NULLC;
}

tPosC previousC(tPosC p, tListC L) {
    (void)L;
    if (p > 0) return p - 1;
    return NULLC;
}

bool insertItemC(tItemC d, tListC *L) {
    /* Lista llena */
    if (L->count >= MAX_COMMITTEES) return false;

    /* Buscamos la posición de inserción (primer índice mayor que d) */
    int pos = 0;
    while (pos < L->count &&
           strcmp(d.committeeName, L->items[pos].committeeName) > 0) {
        pos++;
    }

    /* Desplazamos los elementos desde el final hasta pos una posición a la derecha */
    for (int i = L->count; i > pos; i--) {
        L->items[i] = L->items[i - 1];
    }

    /* Insertamos en pos */
    L->items[pos] = d;
    L->count++;
    return true;
}

void deleteAtPositionC(tPosC p, tListC *L) {
    /* Desplazamos los elementos posteriores a p una posición a la izquierda */
    for (int i = p; i < L->count - 1; i++) {
        L->items[i] = L->items[i + 1];
    }
    L->count--;
}

tItemC getItemC(tPosC p, tListC L) {
    return L.items[p];
}

void updateItemC(tItemC d, tPosC p, tListC *L) {
    L->items[p] = d;
}

tPosC findItemC(tCommitteeName name, tListC L) {
    for (int i = 0; i < L.count; i++) {
        int cmp = strcmp(name, L.items[i].committeeName);
        if (cmp == 0) return i;    /* encontrado */
        if (cmp < 0)  return NULLC; /* pasamos el punto → no existe */
    }
    return NULLC;
}