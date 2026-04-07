/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Moira Paz Fernández     LOGIN 1: moira.paz
 * AUTHOR 2: Álvaro Francos Martínez LOGIN 2: alvaro.francos
 * GROUP: 4.1
 * DATE: 02 / 04 / 26
 */

#ifndef PROJECT_LIST_H
#define PROJECT_LIST_H

#include "types.h"
#include <stdbool.h>

/* Constante para representar posición nula */
#define NULLP NULL

/* tItemP: datos de un proyecto */
typedef struct {
    tProjectName projectName;
    tNumVotes    numVotes;
    tProjectEco  projectEco;
} tItemP;

/* Nodo de la lista enlazada */
typedef struct tNodeP {
    tItemP        item;
    struct tNodeP *next;
} tNodeP;

/* tListP: puntero al primer nodo (lista dinámica simplemente enlazada) */
typedef tNodeP* tListP;

/* tPosP: puntero a un nodo */
typedef tNodeP* tPosP;

/*
 * Objetivo: Inicializa la lista como vacía.
 * Entradas: L - puntero a la lista a inicializar.
 * Salidas: (ninguna, modifica L).
 * Precondiciones: (ninguna).
 * Postcondiciones: La lista queda vacía.
 */
void createEmptyListP(tListP *L);

/*
 * Objetivo: Indica si la lista está vacía.
 * Entradas: L - la lista.
 * Salidas: true si está vacía, false si no.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: (ninguna).
 */
bool isEmptyListP(tListP L);

/*
 * Objetivo: Devuelve la posición del primer elemento.
 * Entradas: L - la lista.
 * Salidas: Posición del primer elemento.
 * Precondiciones: La lista no está vacía.
 * Postcondiciones: (ninguna).
 */
tPosP firstP(tListP L);

/*
 * Objetivo: Devuelve la posición del último elemento.
 * Entradas: L - la lista.
 * Salidas: Posición del último elemento.
 * Precondiciones: La lista no está vacía.
 * Postcondiciones: (ninguna).
 */
tPosP lastP(tListP L);

/*
 * Objetivo: Devuelve la posición siguiente a la indicada.
 * Entradas: p - posición actual; L - la lista.
 * Salidas: Posición siguiente o NULLP si no hay siguiente.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 */
tPosP nextP(tPosP p, tListP L);

/*
 * Objetivo: Devuelve la posición anterior a la indicada.
 * Entradas: p - posición actual; L - la lista.
 * Salidas: Posición anterior o NULLP si no hay anterior.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 */
tPosP previousP(tPosP p, tListP L);

/*
 * Objetivo: Inserta un elemento de forma ordenada por projectName.
 * Entradas: d - item a insertar; L - puntero a la lista.
 * Salidas: true si se insertó correctamente, false en caso contrario.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: Las posiciones posteriores al elemento insertado pueden variar.
 */
bool insertItemP(tItemP d, tListP *L);

/*
 * Objetivo: Elimina el elemento en la posición indicada.
 * Entradas: p - posición a eliminar; L - puntero a la lista.
 * Salidas: (ninguna, modifica L).
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: Las posiciones posteriores a p pueden variar.
 */
void deleteAtPositionP(tPosP p, tListP *L);

/*
 * Objetivo: Devuelve el elemento en la posición indicada.
 * Entradas: p - posición; L - la lista.
 * Salidas: El item en la posición p.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 */
tItemP getItemP(tPosP p, tListP L);

/*
 * Objetivo: Modifica el elemento en la posición indicada.
 * Entradas: d - nuevo item; p - posición; L - puntero a la lista.
 * Salidas: (ninguna, modifica L).
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: El orden de la lista no se modifica.
 */
void updateItemP(tItemP d, tPosP p, tListP *L);

/*
 * Objetivo: Busca un proyecto por nombre aprovechando el orden de la lista.
 * Entradas: name - nombre a buscar; L - la lista.
 * Salidas: Posición del elemento o NULLP si no existe.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: (ninguna).
 */
tPosP findItemP(tProjectName name, tListP L);

#endif