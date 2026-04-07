/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: ***************************** LOGIN 1: **********
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: *.*
 * DATE: ** / ** / **
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "committee_list.h"

#define MAX_BUFFER 255

/* Lista global de comités */
tListC committeeList;

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
 *           ordenado en la lista global.
 * Entradas: commandNumber      - número de orden;
 *           committeeName      - nombre del comité;
 *           totalEvaluatorsStr - nº de evaluadores (string).
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Si tiene éxito, el comité queda en la lista.
 * ------------------------------------------------------- */
void processCreate(char *commandNumber, char *committeeName,
                   char *totalEvaluatorsStr) {
    int totalEvaluators = atoi(totalEvaluatorsStr);

    printf("********************\n");
    printf("%s C: committee %s totalevaluators %d\n",
           commandNumber, committeeName, totalEvaluators);

    if (findItemC(committeeName, committeeList) != NULLC) {
        printf("+ Error: Create not possible\n");
        return;
    }

    tItemC newItem;
    strcpy(newItem.committeeName, committeeName);
    newItem.totalEvaluators = totalEvaluators;
    newItem.validVotes      = 0;
    newItem.nullVotes       = 0;
    createEmptyListP(&newItem.projectList);

    if (!insertItemC(newItem, &committeeList)) {
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
 *           categoryStr   - "eco" o "non-eco".
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Si tiene éxito, el proyecto queda en la
 *                  lista del comité.
 * ------------------------------------------------------- */
void processNew(char *commandNumber, char *committeeName,
                char *projectName, char *categoryStr) {
    bool isEco = (strcmp(categoryStr, "eco") == 0);

    printf("********************\n");
    printf("%s N: committee %s project %s category %s\n",
           commandNumber, committeeName, projectName, categoryStr);

    tPosC posC = findItemC(committeeName, committeeList);
    if (posC == NULLC) {
        printf("+ Error: New not possible\n");
        return;
    }

    tItemC committee = getItemC(posC, committeeList);

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

    updateItemC(committee, posC, &committeeList);

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
 *           projectName   - nombre del proyecto.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Contadores actualizados.
 * ------------------------------------------------------- */
void processVote(char *commandNumber, char *committeeName,
                 char *projectName) {
    printf("********************\n");
    printf("%s V: committee %s project %s\n",
           commandNumber, committeeName, projectName);

    tPosC posC = findItemC(committeeName, committeeList);
    if (posC == NULLC) {
        printf("+ Error: Vote not possible\n");
        return;
    }

    tItemC committee = getItemC(posC, committeeList);
    tPosP  posP      = findItemP(projectName, committee.projectList);

    if (posP == NULLP) {
        committee.nullVotes++;
        updateItemC(committee, posC, &committeeList);
        printf("+ Error: Vote not possible. Project %s not found in committee %s. NULLVOTE\n",
               projectName, committeeName);
        return;
    }

    tItemP project = getItemP(posP, committee.projectList);
    project.numVotes++;
    updateItemP(project, posP, &committee.projectList);
    committee.validVotes++;
    updateItemC(committee, posC, &committeeList);

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
 *           projectName   - nombre del proyecto.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: El proyecto eliminado de todos los comités
 *                  donde existía; sus votos pasan a nullVotes.
 * ------------------------------------------------------- */
void processDisqualify(char *commandNumber, char *projectName) {
    printf("********************\n");
    printf("%s D: project %s\n", commandNumber, projectName);

    if (isEmptyListC(committeeList)) {
        printf("+ Error: Disqualify not possible\n");
        return;
    }

    tPosC posC  = firstC(committeeList);
    bool  first = true;

    while (posC != NULLC) {
        tItemC committee = getItemC(posC, committeeList);

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
            updateItemC(committee, posC, &committeeList);
            printf("Project %s disqualified\n", projectName);
        }

        posC = nextC(posC, committeeList);
    }
}

/* -------------------------------------------------------
 * processRemove
 * Objetivo: Elimina todos los comités con 0 votos válidos.
 *           Sin candidatos o lista vacía → error.
 * Entradas: commandNumber - número de orden.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Comités con validVotes==0 eliminados.
 * ------------------------------------------------------- */
void processRemove(char *commandNumber) {
    printf("********************\n");
    printf("%s R:\n", commandNumber);

    if (isEmptyListC(committeeList)) {
        printf("+ Error: Remove not possible\n");
        return;
    }

    bool removed = false;
    tPosC posC = firstC(committeeList);

    while (posC != NULLC) {
        tItemC committee = getItemC(posC, committeeList);

        if (committee.validVotes == 0) {
            printf("* Remove: committee %s\n", committee.committeeName);

            /* Vaciamos la lista de proyectos antes de borrar el comité
               (precondición de deleteAtPositionC) */
            while (!isEmptyListP(committee.projectList)) {
                deleteAtPositionP(firstP(committee.projectList),
                                  &committee.projectList);
            }
            updateItemC(committee, posC, &committeeList);
            deleteAtPositionC(posC, &committeeList);
            removed = true;
            /* Tras el borrado, el elemento que estaba en posC+1 pasa a posC.
               No avanzamos posC: el mismo índice ahora apunta al siguiente. */
            if (posC >= committeeList.count) break;
        } else {
            posC = nextC(posC, committeeList);
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
 * Entradas: commandNumber - número de orden.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void processWinners(char *commandNumber) {
    printf("********************\n");
    printf("%s W:\n", commandNumber);

    if (isEmptyListC(committeeList)) {
        printf("+ Error: Winners not possible\n");
        return;
    }

    tPosC posC           = firstC(committeeList);
    bool  firstCommittee = true;

    while (posC != NULLC) {
        tItemC committee = getItemC(posC, committeeList);

        if (!firstCommittee) printf("\n");
        firstCommittee = false;

        printf("Committee %s\n", committee.committeeName);

        /* Rastreamos ganador eco y non-eco por separado */
        tPosP winnerEcoPos    = NULLP;
        int   winnerEcoVotes  = -1;
        bool  ecoTie          = false;
        int   ecoCount        = 0;

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

        /* Categoría eco */
        if (ecoCount == 0 || (ecoCount > 1 && ecoTie)) {
            printf("Category eco: No winner\n");
        } else {
            tItemP w = getItemP(winnerEcoPos, committee.projectList);
            printf("Category eco: Project %s numvotes %d\n",
                   w.projectName, w.numVotes);
        }

        /* Categoría non-eco */
        if (nonEcoCount == 0 || (nonEcoCount > 1 && nonEcoTie)) {
            printf("Category non-eco: No winner\n");
        } else {
            tItemP w = getItemP(winnerNonEcoPos, committee.projectList);
            printf("Category non-eco: Project %s numvotes %d\n",
                   w.projectName, w.numVotes);
        }

        posC = nextC(posC, committeeList);
    }
    printf("\n");
}

/* -------------------------------------------------------
 * processStats
 * Objetivo: Muestra estadísticas de todos los comités.
 *           Lista vacía → error.
 * Entradas: commandNumber - número de orden.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void processStats(char *commandNumber) {
    printf("********************\n");
    printf("%s S:\n", commandNumber);

    if (isEmptyListC(committeeList)) {
        printf("+ Error: Stats not possible\n");
        return;
    }

    tPosC pos = firstC(committeeList);
    while (pos != NULLC) {
        printStats(getItemC(pos, committeeList));
        pos = nextC(pos, committeeList);
        printf("\n");
    }
}

/* -------------------------------------------------------
 * processCommand
 * Objetivo: Despacha cada operación al procesador correcto.
 * Entradas: commandNumber - número de orden;
 *           command       - carácter de operación;
 *           param1..3     - parámetros opcionales.
 * Salidas: (ninguna).
 * Precondiciones: La lista global está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void processCommand(char *commandNumber, char command,
                    char *param1, char *param2, char *param3) {
    switch (command) {
        case 'C': processCreate(commandNumber, param1, param2);            break;
        case 'N': processNew(commandNumber, param1, param2, param3);       break;
        case 'V': processVote(commandNumber, param1, param2);              break;
        case 'S': processStats(commandNumber);                             break;
        case 'D': processDisqualify(commandNumber, param1);                break;
        case 'R': processRemove(commandNumber);                            break;
        case 'W': processWinners(commandNumber);                           break;
        default:  break;
    }
}

/* -------------------------------------------------------
 * readTasks
 * Objetivo: Lee el fichero y procesa cada operación.
 * Entradas: filename - ruta del fichero de entrada.
 * Salidas: (ninguna).
 * Precondiciones: El fichero existe con el formato esperado.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void readTasks(char *filename) {
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
            processCommand(commandNumber, command[0], param1, param2, param3);
        }
        fclose(f);
    } else {
        printf("Cannot open file %s.\n", filename);
    }
}

/* -------------------------------------------------------
 * main
 * Objetivo: Inicializa la lista global y lanza la lectura.
 * Entradas: nargs - número de argumentos; args - argumentos.
 * Salidas: 0.
 * Precondiciones: (ninguna).
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
int main(int nargs, char **args) {
    char *file_name = "create.txt";

    if (nargs > 1) {
        file_name = args[1];
    } else {
        #ifdef INPUT_FILE
        file_name = INPUT_FILE;
        #endif
    }

    createEmptyListC(&committeeList);
    readTasks(file_name);
    return 0;
}