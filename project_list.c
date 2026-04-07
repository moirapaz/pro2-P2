/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Moira Paz Fernández     LOGIN 1: moira.paz
 * AUTHOR 2: Álvaro Francos Martínez LOGIN 2: alvaro.francos
 * GROUP: 4.1
 * DATE: 02 / 04 / 26
 */

#include "project_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void createEmptyListP(tListP *L) {
    *L = NULLP;
}

bool isEmptyListP(tListP L) {
    return L == NULLP;
}

tPosP firstP(tListP L) {
    return L;
}

tPosP lastP(tListP L) {
    tPosP p = L;
    // Avanzamos hasta el nodo cuyo ->next sea NULL
    while (p->next != NULLP) {
        p = p->next;
    }
    return p;
}

tPosP nextP(tPosP p, tListP L) {
    (void)L;
    return p->next;
}

tPosP previousP(tPosP p, tListP L) {
    // Si p es el primer nodo, no tiene anterior
    if (p == L) return NULLP;

    // Recorremos desde el inicio hasta encontrar el nodo cuyo ->next == p
    tPosP prev = L;
    while (prev->next != p) {
        prev = prev->next;
    }
    return prev;
}

bool insertItemP(tItemP d, tListP *L) {
    /* Reservamos memoria para el nuevo nodo */
    tNodeP *newNode = (tNodeP *)malloc(sizeof(tNodeP));
    if (newNode == NULL) return false; /* sin memoria */

    newNode->item = d;
    newNode->next = NULLP;

    /* Caso 1: lista vacía o el nuevo va antes que el primero */
    if (*L == NULLP || strcmp(d.projectName, (*L)->item.projectName) < 0) {
        newNode->next = *L;
        *L = newNode;
        return true;
    }

    /* Caso 2: buscamos el punto de inserción recorriendo la lista */
    tPosP prev = *L;
    while (prev->next != NULLP &&
           strcmp(d.projectName, prev->next->item.projectName) > 0) {
        prev = prev->next;
    }

    /* Insertamos newNode entre prev y prev->next */
    newNode->next = prev->next;
    prev->next = newNode;
    return true;
}

void deleteAtPositionP(tPosP p, tListP *L) {
    /* Caso: p es el primer nodo */
    if (p == *L) {
        *L = p->next;
        free(p);
        return;
    }

    /* Buscamos el nodo anterior a p */
    tPosP prev = *L;
    while (prev->next != p) {
        prev = prev->next;
    }
    prev->next = p->next;
    free(p);
}

tItemP getItemP(tPosP p, tListP L) {
    (void)L;
    return p->item;
}

void updateItemP(tItemP d, tPosP p, tListP *L) {
    (void)L;
    p->item = d;
}

tPosP findItemP(tProjectName name, tListP L) {
    tPosP p = L;
    while (p != NULLP) {
        int cmp = strcmp(name, p->item.projectName);
        if (cmp == 0) return p;   /* encontrado */
        if (cmp < 0)  return NULLP; /* pasamos el punto → no existe */
        p = p->next;
    }
    return NULLP;
}