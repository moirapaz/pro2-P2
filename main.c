/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Moira Paz Fernández     LOGIN 1: moira.paz
 * AUTHOR 2: Álvaro Francos Martínez LOGIN 2: alvaro.francos
 * GROUP: 4.1
 * DATE: 15 / 04 / 26
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "committee_list.h"

#define MAX_BUFFER 255

/* -------------------------------------------------------
 * printStats
 * Objetivo: Imprime las estadísticas de un comité evaluador.
 * Entradas: item - el tItemC del comité a imprimir.
 * Salidas: (ninguna).
 * Precondiciones: item está correctamente inicializado.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void printStats(tItemC item) {
    printf("Committee %s\n", item.committeeName);

    if (isEmptyListP(item.projectList)) {
        printf("No projects\n");
    } else {
        tPosP pos = firstP(item.projectList);
        while (pos != NULLP) {
            tItemP proj = getItemP(pos, item.projectList);
            double pct = (item.validVotes > 0)
                             ? proj.numVotes * 100.0 / item.validVotes
                             : 0.0;
            printf("Project %s category %s numvotes %d (%.2f%%)\n",
                   proj.projectName,
                   proj.projectEco ? "eco" : "non-eco",
                   proj.numVotes, pct);
            pos = nextP(pos, item.projectList);
        }
    }

    printf("Nullvotes %d\n", item.nullVotes);
    printf("Participation: %d votes from %d evaluators (%.2f%%)\n",
           item.validVotes + item.nullVotes,
           item.totalEvaluators,
           item.totalEvaluators > 0
               ? (item.validVotes + item.nullVotes) * 100.0 / item.totalEvaluators
               : 0.0);
}

/* -------------------------------------------------------
 * processCreate
 * Objetivo: Crea un nuevo comité evaluador y lo inserta
 *           ordenado en la lista de comités.
 * Entradas: commandNumber      - número de orden;
 *           committeeName      - nombre del comité;
 *           totalEvaluatorsStr - nº de evaluadores (string);
 *           listC              - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Si tiene éxito, el comité queda en la lista.
 * ------------------------------------------------------- */
void processCreate(char *commandNumber, char *committeeName,
                   char *totalEvaluatorsStr, tListC *listC) {
    int totalEvaluators = atoi(totalEvaluatorsStr);

    printf("********************\n");
    printf("%s C: committee %s totalevaluators %d\n",
           commandNumber, committeeName, totalEvaluators);

    if (findItemC(committeeName, *listC) != NULLC) {
        printf("+ Error: Create not possible\n");
        return;
    }

    tItemC newItem;
    strcpy(newItem.committeeName, committeeName);
    newItem.totalEvaluators = totalEvaluators;
    newItem.validVotes      = 0;
    newItem.nullVotes       = 0;
    createEmptyListP(&newItem.projectList);

    if (!insertItemC(newItem, listC)) {
        printf("+ Error: Create not possible\n");
        return;
    }

    printf("* Create: committee %s totalevaluators %d\n",
           committeeName, totalEvaluators);
}

/* -------------------------------------------------------
 * processNew
 * Objetivo: Da de alta un proyecto con 0 votos en el comité
 *           indicado, ordenado por nombre.
 * Entradas: commandNumber - número de orden;
 *           committeeName - nombre del comité;
 *           projectName   - nombre del proyecto;
 *           categoryStr   - "eco" o "non-eco";
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Si tiene éxito, el proyecto queda en la
 *                  lista del comité.
 * ------------------------------------------------------- */
void processNew(char *commandNumber, char *committeeName,
                char *projectName, char *categoryStr, tListC *listC) {
    bool isEco = (strcmp(categoryStr, "eco") == 0);

    printf("********************\n");
    printf("%s N: committee %s project %s category %s\n",
           commandNumber, committeeName, projectName, categoryStr);

    tPosC posC = findItemC(committeeName, *listC);
    if (posC == NULLC) {
        printf("+ Error: New not possible\n");
        return;
    }

    tItemC committee = getItemC(posC, *listC);

    if (findItemP(projectName, committee.projectList) != NULLP) {
        printf("+ Error: New not possible\n");
        return;
    }

    tItemP newProject;
    strcpy(newProject.projectName, projectName);
    newProject.numVotes   = 0;
    newProject.projectEco = isEco;

    if (!insertItemP(newProject, &committee.projectList)) {
        printf("+ Error: New not possible\n");
        return;
    }

    updateItemC(committee, posC, listC);

    printf("* New: committee %s project %s category %s\n",
           committeeName, projectName, categoryStr);
}

/* -------------------------------------------------------
 * processVote
 * Objetivo: Emite un voto para un proyecto en un comité.
 *           Comité inexistente → error genérico.
 *           Proyecto no encontrado → voto nulo.
 *           Proyecto encontrado → incrementa contadores.
 * Entradas: commandNumber - número de orden;
 *           committeeName - nombre del comité;
 *           projectName   - nombre del proyecto;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Contadores actualizados.
 * ------------------------------------------------------- */
void processVote(char *commandNumber, char *committeeName,
                 char *projectName, tListC *listC) {
    printf("********************\n");
    printf("%s V: committee %s project %s\n",
           commandNumber, committeeName, projectName);

    tPosC posC = findItemC(committeeName, *listC);
    if (posC == NULLC) {
        printf("+ Error: Vote not possible\n");
        return;
    }

    tItemC committee = getItemC(posC, *listC);
    tPosP  posP      = findItemP(projectName, committee.projectList);

    if (posP == NULLP) {
        committee.nullVotes++;
        updateItemC(committee, posC, listC);
        printf("+ Error: Vote not possible. Project %s not found in committee %s. NULLVOTE\n",
               projectName, committeeName);
        return;
    }

    tItemP project = getItemP(posP, committee.projectList);
    project.numVotes++;
    updateItemP(project, posP, &committee.projectList);
    committee.validVotes++;
    updateItemC(committee, posC, listC);

    printf("* Vote: committee %s project %s category %s numvotes %d\n",
           committeeName, projectName,
           project.projectEco ? "eco" : "non-eco",
           project.numVotes);
}

/* -------------------------------------------------------
 * processDisqualify
 * Objetivo: Elimina un proyecto de todos los comités,
 *           transfiriendo sus votos a nullVotes.
 *           Lista vacía → error.
 * Entradas: commandNumber - número de orden;
 *           projectName   - nombre del proyecto;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: El proyecto eliminado de todos los comités
 *                  donde existía; sus votos pasan a nullVotes.
 * ------------------------------------------------------- */
void processDisqualify(char *commandNumber, char *projectName,
                       tListC *listC) {
    printf("********************\n");
    printf("%s D: project %s\n", commandNumber, projectName);

    if (isEmptyListC(*listC)) {
        printf("+ Error: Disqualify not possible\n");
        return;
    }

    tPosC posC  = firstC(*listC);
    bool  first = true;

    while (posC != NULLC) {
        tItemC committee = getItemC(posC, *listC);

        if (!first) printf("\n");
        first = false;

        printf("Committee %s\n", committee.committeeName);

        tPosP posP = findItemP(projectName, committee.projectList);
        if (posP == NULLP) {
            printf("No project %s\n", projectName);
        } else {
            tItemP project = getItemP(posP, committee.projectList);
            committee.nullVotes  += project.numVotes;
            committee.validVotes -= project.numVotes;
            deleteAtPositionP(posP, &committee.projectList);
            updateItemC(committee, posC, listC);
            printf("Project %s disqualified\n", projectName);
        }

        posC = nextC(posC, *listC);
    }
}

/* -------------------------------------------------------
 * processRemove
 * Objetivo: Elimina todos los comités con 0 votos válidos.
 *           Sin candidatos o lista vacía → error.
 * Entradas: commandNumber - número de orden;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Comités con validVotes==0 eliminados.
 * ------------------------------------------------------- */
void processRemove(char *commandNumber, tListC *listC) {
    printf("********************\n");
    printf("%s R:\n", commandNumber);

    if (isEmptyListC(*listC)) {
        printf("+ Error: Remove not possible\n");
        return;
    }

    bool  removed = false;
    tPosC posC    = firstC(*listC);

    while (posC != NULLC) {
        tItemC committee = getItemC(posC, *listC);

        if (committee.validVotes == 0) {
            printf("* Remove: committee %s\n", committee.committeeName);

            /* Vaciamos proyectos antes de borrar (precondición deleteAtPositionC) */
            while (!isEmptyListP(committee.projectList)) {
                deleteAtPositionP(firstP(committee.projectList),
                                  &committee.projectList);
            }
            updateItemC(committee, posC, listC);
            deleteAtPositionC(posC, listC);
            removed = true;
            /* Tras el borrado los índices se desplazan -1;
               el mismo posC ahora apunta al siguiente elemento */
            if (posC >= listC->count) break;
        } else {
            posC = nextC(posC, *listC);
        }
    }

    if (!removed) {
        printf("+ Error: Remove not possible\n");
    }
}

/* -------------------------------------------------------
 * processWinners
 * Objetivo: Para cada comité muestra el ganador de cada
 *           categoría. Empate o sin proyectos → "No winner".
 *           Un único proyecto con 0 votos sí es ganador.
 *           Lista vacía → error.
 * Entradas: commandNumber - número de orden;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void processWinners(char *commandNumber, tListC *listC) {
    printf("********************\n");
    printf("%s W:\n", commandNumber);

    if (isEmptyListC(*listC)) {
        printf("+ Error: Winners not possible\n");
        return;
    }

    tPosC posC           = firstC(*listC);
    bool  firstCommittee = true;

    while (posC != NULLC) {
        tItemC committee = getItemC(posC, *listC);

        if (!firstCommittee) printf("\n");
        firstCommittee = false;

        printf("Committee %s\n", committee.committeeName);

        tPosP winnerEcoPos      = NULLP;
        int   winnerEcoVotes    = -1;
        bool  ecoTie            = false;
        int   ecoCount          = 0;

        tPosP winnerNonEcoPos   = NULLP;
        int   winnerNonEcoVotes = -1;
        bool  nonEcoTie         = false;
        int   nonEcoCount       = 0;

        if (!isEmptyListP(committee.projectList)) {
            tPosP posP = firstP(committee.projectList);
            while (posP != NULLP) {
                tItemP proj = getItemP(posP, committee.projectList);

                if (proj.projectEco) {
                    ecoCount++;
                    if (proj.numVotes > winnerEcoVotes) {
                        winnerEcoVotes = proj.numVotes;
                        winnerEcoPos   = posP;
                        ecoTie         = false;
                    } else if (proj.numVotes == winnerEcoVotes) {
                        ecoTie = true;
                    }
                } else {
                    nonEcoCount++;
                    if (proj.numVotes > winnerNonEcoVotes) {
                        winnerNonEcoVotes = proj.numVotes;
                        winnerNonEcoPos   = posP;
                        nonEcoTie         = false;
                    } else if (proj.numVotes == winnerNonEcoVotes) {
                        nonEcoTie = true;
                    }
                }
                posP = nextP(posP, committee.projectList);
            }
        }

        if (ecoCount == 0 || (ecoCount > 1 && ecoTie)) {
            printf("Category eco: No winner\n");
        } else {
            tItemP w = getItemP(winnerEcoPos, committee.projectList);
            printf("Category eco: Project %s numvotes %d\n",
                   w.projectName, w.numVotes);
        }

        if (nonEcoCount == 0 || (nonEcoCount > 1 && nonEcoTie)) {
            printf("Category non-eco: No winner\n");
        } else {
            tItemP w = getItemP(winnerNonEcoPos, committee.projectList);
            printf("Category non-eco: Project %s numvotes %d\n",
                   w.projectName, w.numVotes);
        }

        posC = nextC(posC, *listC);
    }
    printf("\n");
}

/* -------------------------------------------------------
 * processStats
 * Objetivo: Muestra estadísticas de todos los comités.
 *           Lista vacía → error.
 * Entradas: commandNumber - número de orden;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void processStats(char *commandNumber, tListC *listC) {
    printf("********************\n");
    printf("%s S:\n", commandNumber);

    if (isEmptyListC(*listC)) {
        printf("+ Error: Stats not possible\n");
        return;
    }

    tPosC pos = firstC(*listC);
    while (pos != NULLC) {
        printStats(getItemC(pos, *listC));
        pos = nextC(pos, *listC);
        printf("\n");
    }
}

/* -------------------------------------------------------
 * processCommand
 * Objetivo: Despacha cada operación al procesador correcto.
 * Entradas: commandNumber - número de orden;
 *           command       - carácter de operación;
 *           param1..3     - parámetros opcionales;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: La lista está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void processCommand(char *commandNumber, char command,
                    char *param1, char *param2, char *param3,
                    tListC *listC) {
    switch (command) {
        case 'C': processCreate(commandNumber, param1, param2, listC);           break;
        case 'N': processNew(commandNumber, param1, param2, param3, listC);      break;
        case 'V': processVote(commandNumber, param1, param2, listC);             break;
        case 'S': processStats(commandNumber, listC);                            break;
        case 'D': processDisqualify(commandNumber, param1, listC);               break;
        case 'R': processRemove(commandNumber, listC);                           break;
        case 'W': processWinners(commandNumber, listC);                          break;
        default:  break;
    }
}

/* -------------------------------------------------------
 * readTasks
 * Objetivo: Lee el fichero y procesa cada operación.
 * Entradas: filename - ruta del fichero de entrada;
 *           listC    - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: El fichero existe con el formato esperado.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void readTasks(char *filename, tListC *listC) {
    FILE *f = NULL;
    char *commandNumber, *command, *param1, *param2, *param3;
    const char delimiters[] = " \n\r";
    char buffer[MAX_BUFFER];

    f = fopen(filename, "r");
    if (f != NULL) {
        while (fgets(buffer, MAX_BUFFER, f)) {
            commandNumber = strtok(buffer, delimiters);
            command       = strtok(NULL, delimiters);
            param1        = strtok(NULL, delimiters);
            param2        = strtok(NULL, delimiters);
            param3        = strtok(NULL, delimiters);
            processCommand(commandNumber, command[0], param1, param2, param3, listC);
        }
        fclose(f);
    } else {
        printf("Cannot open file %s.\n", filename);
    }
}

/* -------------------------------------------------------
 * main
 * Objetivo: Declara la lista de comités en local, la
 *           inicializa y lanza la lectura del fichero.
 * Entradas: nargs - número de argumentos; args - argumentos.
 * Salidas: 0.
 * Precondiciones: (ninguna).
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
int main(int nargs, char **args) {
    char *file_name = "create.txt";
    tListC listC;  /* variable local, se pasa por puntero a todo */

    if (nargs > 1) {
        file_name = args[1];
    } else {
        #ifdef INPUT_FILE
        file_name = INPUT_FILE;
        #endif
    }

    createEmptyListC(&listC);
    readTasks(file_name, &listC);
    return 0;
}