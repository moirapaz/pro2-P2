/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: ***************************** LOGIN 1: **********
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: *.*
 * DATE: ** / ** / **
 */

#include "committee_list.h"
#include <string.h>
#include <stdio.h>

/* -------------------------------------------------------
 * createEmptyListC
 * Objetivo: Inicializa la lista estática poniendo count a 0.
 * Entradas: L - puntero a la lista.
 * Salidas: (ninguna).
 * Precondiciones: (ninguna).
 * Postcondiciones: L->count == 0.
 * ------------------------------------------------------- */
void createEmptyListC(tListC *L) {
    L->count = 0;
}

/* -------------------------------------------------------
 * isEmptyListC
 * Objetivo: Comprueba si la lista está vacía.
 * Entradas: L - la lista.
 * Salidas: true si count == 0, false en caso contrario.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
bool isEmptyListC(tListC L) {
    return L.count == 0;
}

/* -------------------------------------------------------
 * firstC
 * Objetivo: Devuelve la posición del primer elemento (índice 0).
 * Entradas: L - la lista (no usada, pero requerida por la interfaz).
 * Salidas: 0.
 * Precondiciones: La lista no está vacía.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tPosC firstC(tListC L) {
    (void)L;
    return 0;
}

/* -------------------------------------------------------
 * lastC
 * Objetivo: Devuelve el índice del último elemento.
 * Entradas: L - la lista.
 * Salidas: L.count - 1.
 * Precondiciones: La lista no está vacía.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tPosC lastC(tListC L) {
    return L.count - 1;
}

/* -------------------------------------------------------
 * nextC
 * Objetivo: Devuelve el índice siguiente a p.
 * Entradas: p - posición actual; L - la lista.
 * Salidas: p+1 si existe dentro del rango, NULLC en caso contrario.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tPosC nextC(tPosC p, tListC L) {
    if (p + 1 < L.count) return p + 1;
    return NULLC;
}

/* -------------------------------------------------------
 * previousC
 * Objetivo: Devuelve el índice anterior a p.
 * Entradas: p - posición actual; L - la lista.
 * Salidas: p-1 si p > 0, NULLC si p es el primero.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tPosC previousC(tPosC p, tListC L) {
    (void)L;
    if (p > 0) return p - 1;
    return NULLC;
}

/* -------------------------------------------------------
 * insertItemC
 * Objetivo: Inserta d manteniendo el orden alfabético por
 *           committeeName. Desplaza los elementos necesarios.
 * Entradas: d - item a insertar; L - puntero a la lista.
 * Salidas: true si se insertó, false si la lista está llena.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: Las posiciones posteriores al nuevo elemento
 *                  pueden haber variado.
 * ------------------------------------------------------- */
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

/* -------------------------------------------------------
 * deleteAtPositionC
 * Objetivo: Elimina el elemento en la posición p desplazando
 *           los siguientes una posición a la izquierda.
 * Entradas: p - índice a eliminar; L - puntero a la lista.
 * Salidas: (ninguna).
 * Precondiciones: p es válida en L y la projectList del comité
 *                 en p está vacía.
 * Postcondiciones: Las posiciones posteriores a p pueden variar.
 * ------------------------------------------------------- */
void deleteAtPositionC(tPosC p, tListC *L) {
    /* Desplazamos los elementos posteriores a p una posición a la izquierda */
    for (int i = p; i < L->count - 1; i++) {
        L->items[i] = L->items[i + 1];
    }
    L->count--;
}

/* -------------------------------------------------------
 * getItemC
 * Objetivo: Devuelve el item almacenado en la posición p.
 * Entradas: p - índice; L - la lista.
 * Salidas: El tItemC en L.items[p].
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tItemC getItemC(tPosC p, tListC L) {
    return L.items[p];
}

/* -------------------------------------------------------
 * updateItemC
 * Objetivo: Sustituye el elemento en la posición p por d.
 * Entradas: d - nuevo item; p - índice; L - puntero a la lista.
 * Salidas: (ninguna).
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: El orden de la lista no se modifica.
 * ------------------------------------------------------- */
void updateItemC(tItemC d, tPosC p, tListC *L) {
    L->items[p] = d;
}

/* -------------------------------------------------------
 * findItemC
 * Objetivo: Busca el comité con committeeName == name.
 *           Como la lista está ordenada, para en cuanto el
 *           nombre actual sea mayor que name.
 * Entradas: name - nombre a buscar; L - la lista.
 * Salidas: Índice del comité encontrado, o NULLC si no existe.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
tPosC findItemC(tCommitteeName name, tListC L) {
    for (int i = 0; i < L.count; i++) {
        int cmp = strcmp(name, L.items[i].committeeName);
        if (cmp == 0) return i;    /* encontrado */
        if (cmp < 0)  return NULLC; /* pasamos el punto → no existe */
    }
    return NULLC;
}