/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: ***************************** LOGIN 1: **********
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: *.*
 * DATE: ** / ** / **
 */

#include "project_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* -------------------------------------------------------
 * createEmptyListP
 * Objetivo: Inicializa la lista vacía poniendo el puntero a NULL.
 * Entradas: L - puntero a la lista.
 * Salidas: (ninguna).
 * Precondiciones: (ninguna).
 * Postcondiciones: *L == NULLP.
 * ------------------------------------------------------- */
void createEmptyListP(tListP *L) {
    *L = NULLP;
}

/* -------------------------------------------------------
 * isEmptyListP
 * Objetivo: Comprueba si la lista está vacía.
 * Entradas: L - la lista.
 * Salidas: true si L == NULLP, false en caso contrario.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
bool isEmptyListP(tListP L) {
    return L == NULLP;
}

/* -------------------------------------------------------
 * firstP
 * Objetivo: Devuelve la posición del primer elemento.
 * Entradas: L - la lista.
 * Salidas: Puntero al primer nodo.
 * Precondiciones: La lista no está vacía.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tPosP firstP(tListP L) {
    return L;
}

/* -------------------------------------------------------
 * lastP
 * Objetivo: Devuelve la posición del último elemento.
 * Entradas: L - la lista.
 * Salidas: Puntero al último nodo.
 * Precondiciones: La lista no está vacía.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tPosP lastP(tListP L) {
    tPosP p = L;
    /* Avanzamos hasta el nodo cuyo ->next sea NULL */
    while (p->next != NULLP) {
        p = p->next;
    }
    return p;
}

/* -------------------------------------------------------
 * nextP
 * Objetivo: Devuelve la posición siguiente a p.
 * Entradas: p - posición actual; L - la lista (no se usa pero
 *           se incluye por consistencia con la interfaz).
 * Salidas: p->next, o NULLP si p es el último.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tPosP nextP(tPosP p, tListP L) {
    (void)L; /* parámetro requerido por la interfaz, no necesario aquí */
    return p->next;
}

/* -------------------------------------------------------
 * previousP
 * Objetivo: Devuelve la posición anterior a p.
 * Entradas: p - posición actual; L - la lista (necesaria para
 *           recorrerla desde el inicio).
 * Salidas: Puntero al nodo anterior, o NULLP si p es el primero.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tPosP previousP(tPosP p, tListP L) {
    /* Si p es el primer nodo, no tiene anterior */
    if (p == L) return NULLP;

    /* Recorremos desde el inicio hasta encontrar el nodo cuyo ->next == p */
    tPosP prev = L;
    while (prev->next != p) {
        prev = prev->next;
    }
    return prev;
}

/* -------------------------------------------------------
 * insertItemP
 * Objetivo: Inserta d en la lista manteniendo el orden
 *           alfabético por projectName.
 * Entradas: d - item a insertar; L - puntero a la lista.
 * Salidas: true si se insertó, false si no hay memoria.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: Las posiciones posteriores al nuevo nodo
 *                  pueden haber variado.
 * ------------------------------------------------------- */
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

/* -------------------------------------------------------
 * deleteAtPositionP
 * Objetivo: Elimina el nodo en la posición p.
 * Entradas: p - posición a eliminar; L - puntero a la lista.
 * Salidas: (ninguna).
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: Las posiciones posteriores a p pueden variar.
 * ------------------------------------------------------- */
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

/* -------------------------------------------------------
 * getItemP
 * Objetivo: Devuelve el item almacenado en la posición p.
 * Entradas: p - posición; L - la lista (no usada aquí).
 * Salidas: El tItemP del nodo p.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tItemP getItemP(tPosP p, tListP L) {
    (void)L;
    return p->item;
}

/* -------------------------------------------------------
 * updateItemP
 * Objetivo: Sustituye el item de la posición p por d.
 * Entradas: d - nuevo item; p - posición; L - puntero a la lista.
 * Salidas: (ninguna).
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: El orden de la lista no se modifica.
 * ------------------------------------------------------- */
void updateItemP(tItemP d, tPosP p, tListP *L) {
    (void)L;
    p->item = d;
}

/* -------------------------------------------------------
 * findItemP
 * Objetivo: Busca el primer nodo con projectName == name.
 *           Aprovecha que la lista está ordenada para parar
 *           en cuanto el nombre actual sea mayor que name.
 * Entradas: name - nombre a buscar; L - la lista.
 * Salidas: Posición del nodo encontrado, o NULLP si no existe.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
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