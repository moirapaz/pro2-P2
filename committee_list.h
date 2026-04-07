/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: ***************************** LOGIN 1: **********
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: *.*
 * DATE: ** / ** / **
 */

#ifndef COMMITTEE_LIST_H
#define COMMITTEE_LIST_H

#include "types.h"
#include "project_list.h"
#include <stdbool.h>

/* Capacidad máxima de la lista estática */
#define MAX_COMMITTEES 10

/* Constante para posición nula */
#define NULLC -1

/* tItemC: datos de un comité evaluador */
typedef struct {
    tCommitteeName committeeName;
    tNumVotes      totalEvaluators;
    tNumVotes      validVotes;
    tNumVotes      nullVotes;
    tListP         projectList;
} tItemC;

/* tPosC: índice en el array (int) */
typedef int tPosC;

/* tListC: lista estática de comités */
typedef struct {
    tItemC items[MAX_COMMITTEES]; /* array de elementos          */
    int    count;                 /* número de elementos actuales */
} tListC;

/*
 * Objetivo: Inicializa la lista de comités como vacía.
 * Entradas: L - puntero a la lista.
 * Salidas: (ninguna).
 * Precondiciones: (ninguna).
 * Postcondiciones: L->count == 0.
 */
void createEmptyListC(tListC *L);

/*
 * Objetivo: Indica si la lista de comités está vacía.
 * Entradas: L - la lista.
 * Salidas: true si está vacía, false si no.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: (ninguna).
 */
bool isEmptyListC(tListC L);

/*
 * Objetivo: Devuelve la posición del primer elemento.
 * Entradas: L - la lista.
 * Salidas: Índice 0 (primer elemento).
 * Precondiciones: La lista no está vacía.
 * Postcondiciones: (ninguna).
 */
tPosC firstC(tListC L);

/*
 * Objetivo: Devuelve la posición del último elemento.
 * Entradas: L - la lista.
 * Salidas: Índice del último elemento.
 * Precondiciones: La lista no está vacía.
 * Postcondiciones: (ninguna).
 */
tPosC lastC(tListC L);

/*
 * Objetivo: Devuelve la posición siguiente a p.
 * Entradas: p - posición actual; L - la lista.
 * Salidas: p+1 si existe, NULLC en caso contrario.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 */
tPosC nextC(tPosC p, tListC L);

/*
 * Objetivo: Devuelve la posición anterior a p.
 * Entradas: p - posición actual; L - la lista.
 * Salidas: p-1 si existe, NULLC si p es el primero.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 */
tPosC previousC(tPosC p, tListC L);

/*
 * Objetivo: Inserta d en la lista manteniendo el orden
 *           alfabético por committeeName.
 * Entradas: d - item a insertar; L - puntero a la lista.
 * Salidas: true si se insertó, false si la lista está llena.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: Las posiciones posteriores al nuevo elemento
 *                  pueden haber variado.
 */
bool insertItemC(tItemC d, tListC *L);

/*
 * Objetivo: Elimina el elemento en la posición p.
 * Entradas: p - posición a eliminar; L - puntero a la lista.
 * Salidas: (ninguna).
 * Precondiciones: p es válida en L y la projectList del comité
 *                 en p está vacía.
 * Postcondiciones: Las posiciones posteriores a p pueden variar.
 */
void deleteAtPositionC(tPosC p, tListC *L);

/*
 * Objetivo: Devuelve el elemento en la posición p.
 * Entradas: p - posición; L - la lista.
 * Salidas: El tItemC en la posición p.
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: (ninguna).
 */
tItemC getItemC(tPosC p, tListC L);

/*
 * Objetivo: Sustituye el elemento en la posición p por d.
 * Entradas: d - nuevo item; p - posición; L - puntero a la lista.
 * Salidas: (ninguna).
 * Precondiciones: p es una posición válida en L.
 * Postcondiciones: El orden de la lista no se modifica.
 */
void updateItemC(tItemC d, tPosC p, tListC *L);

/*
 * Objetivo: Busca el primer comité con committeeName == name.
 *           Aprovecha el orden para parar antes si corresponde.
 * Entradas: name - nombre a buscar; L - la lista.
 * Salidas: Posición del comité encontrado, o NULLC si no existe.
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: (ninguna).
 */
tPosC findItemC(tCommitteeName name, tListC L);

#endif