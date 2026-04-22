/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Moira Paz Fernández     LOGIN 1: moira.paz
 * AUTHOR 2: Álvaro Francos Martínez LOGIN 2: alvaro.francos
 * GROUP: 4.1
 * DATE: 18 / 04 / 26
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "committee_list.h"

#define MAX_BUFFER 255 /* Tamaño máximo del buffer de lectura de línea */

/* -------------------------------------------------------
 * printStats
 * Objetivo: Imprime las estadísticas de un comité evaluador:
 *           lista de proyectos con porcentajes, votos nulos
 *           y participación.
 * Entradas: item - datos del comité a imprimir.
 * Salidas: (ninguna, imprime por stdout).
 * Precondiciones: item está correctamente inicializado.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void printStats(tItemC item) {
    tPosP  pos;  /* posición actual al recorrer la lista de proyectos */
    tItemP proj; /* proyecto en la posición actual */
    double pct;  /* porcentaje de votos del proyecto sobre votos válidos */

    printf("Committee %s\n", item.committeeName);

    if (isEmptyListP(item.projectList)) {
        printf("No projects\n");
    } else {
        pos = firstP(item.projectList);
        while (pos != NULLP) {
            proj = getItemP(pos, item.projectList);
            pct  = (item.validVotes > 0)
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
 *           ordenado por nombre en la lista de comités.
 * Entradas: commandNumber      - número de orden de la petición;
 *           committeeName      - nombre del nuevo comité;
 *           totalEvaluatorsStr - número total de evaluadores en
 *                                formato string;
 *           listC              - puntero a la lista de comités.
 * Salidas: (ninguna, imprime resultado por stdout).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Si tiene éxito, el comité queda insertado
 *                  en la lista ordenada.
 * ------------------------------------------------------- */
void processCreate(char *commandNumber, char *committeeName,
                   char *totalEvaluatorsStr, tListC *listC) {
    int    totalEvaluators; /* número de evaluadores convertido a entero */
    tItemC newItem;         /* nuevo item de comité a insertar */

    totalEvaluators = atoi(totalEvaluatorsStr);

    printf("********************\n");
    printf("%s C: committee %s totalevaluators %d\n",
           commandNumber, committeeName, totalEvaluators);

    /* Error si ya existe un comité con ese nombre */
    if (findItemC(committeeName, *listC) != NULLC) {
        printf("+ Error: Create not possible\n");
        return;
    }

    /* Construimos el nuevo comité con contadores a 0 y lista vacía */
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
 *           indicado, insertándolo ordenado por nombre.
 * Entradas: commandNumber - número de orden de la petición;
 *           committeeName - nombre del comité destino;
 *           projectName   - nombre del nuevo proyecto;
 *           categoryStr   - categoría del proyecto ("eco" o "non-eco");
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna, imprime resultado por stdout).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Si tiene éxito, el proyecto queda insertado
 *                  en la lista del comité ordenada por nombre.
 * ------------------------------------------------------- */
void processNew(char *commandNumber, char *committeeName,
                char *projectName, char *categoryStr, tListC *listC) {
    bool   isEco;     /* true si la categoría es "eco", false si "non-eco" */
    tPosC  posC;      /* posición del comité en la lista de comités */
    tItemC committee; /* datos del comité donde se inserta el proyecto */
    tItemP newProject;/* nuevo proyecto a insertar */

    isEco = (strcmp(categoryStr, "eco") == 0);

    printf("********************\n");
    printf("%s N: committee %s project %s category %s\n",
           commandNumber, committeeName, projectName, categoryStr);

    /* Comprobamos que el comité existe */
    posC = findItemC(committeeName, *listC);
    if (posC == NULLC) {
        printf("+ Error: New not possible\n");
        return;
    }

    committee = getItemC(posC, *listC);

    /* Error si el proyecto ya existe en este comité */
    if (findItemP(projectName, committee.projectList) != NULLP) {
        printf("+ Error: New not possible\n");
        return;
    }

    /* Construimos el proyecto e insertamos en la lista del comité */
    strcpy(newProject.projectName, projectName);
    newProject.numVotes   = 0;
    newProject.projectEco = isEco;

    if (!insertItemP(newProject, &committee.projectList)) {
        printf("+ Error: New not possible\n");
        return;
    }

    /* Guardamos el comité actualizado en la lista */
    updateItemC(committee, posC, listC);

    printf("* New: committee %s project %s category %s\n",
           committeeName, projectName, categoryStr);
}

/* -------------------------------------------------------
 * processVote
 * Objetivo: Emite un voto para un proyecto en un comité.
 *           Si el comité no existe → error genérico.
 *           Si el proyecto no existe en ese comité → voto nulo.
 *           Si existe → incrementa numVotes y validVotes.
 * Entradas: commandNumber - número de orden de la petición;
 *           committeeName - nombre del comité donde se vota;
 *           projectName   - nombre del proyecto votado;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna, imprime resultado por stdout).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Los contadores del comité y del proyecto
 *                  quedan actualizados.
 * ------------------------------------------------------- */
void processVote(char *commandNumber, char *committeeName,
                 char *projectName, tListC *listC) {
    tPosC  posC;      /* posición del comité en la lista de comités */
    tItemC committee; /* datos del comité donde se emite el voto */
    tPosP  posP;      /* posición del proyecto en la lista de proyectos */
    tItemP project;   /* datos del proyecto que recibe el voto */

    printf("********************\n");
    printf("%s V: committee %s project %s\n",
           commandNumber, committeeName, projectName);

    /* Comprobamos que el comité existe */
    posC = findItemC(committeeName, *listC);
    if (posC == NULLC) {
        printf("+ Error: Vote not possible\n");
        return;
    }

    committee = getItemC(posC, *listC);
    posP      = findItemP(projectName, committee.projectList);

    if (posP == NULLP) {
        /* El proyecto no existe en este comité: voto nulo */
        committee.nullVotes++;
        updateItemC(committee, posC, listC);
        printf("+ Error: Vote not possible. Project %s not found in committee %s. NULLVOTE\n",
               projectName, committeeName);
        return;
    }

    /* Incrementamos votos del proyecto y votos válidos del comité */
    project = getItemP(posP, committee.projectList);
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
 *           Si la lista de comités está vacía → error.
 * Entradas: commandNumber - número de orden de la petición;
 *           projectName   - nombre del proyecto a descalificar;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna, imprime resultado por stdout).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: El proyecto queda eliminado de todos los
 *                  comités donde existía; sus votos pasan a
 *                  nullVotes del comité correspondiente.
 * ------------------------------------------------------- */
void processDisqualify(char *commandNumber, char *projectName,
                       tListC *listC) {
    tPosC  posC;      /* posición del comité actual en el recorrido */
    tItemC committee; /* datos del comité actual */
    tPosP  posP;      /* posición del proyecto en la lista del comité */
    tItemP project;   /* datos del proyecto a descalificar */
    bool   first;     /* controla la línea en blanco entre comités */

    printf("********************\n");
    printf("%s D: project %s\n", commandNumber, projectName);

    if (isEmptyListC(*listC)) {
        printf("+ Error: Disqualify not possible\n");
        return;
    }

    posC  = firstC(*listC);
    first = true;

    while (posC != NULLC) {
        committee = getItemC(posC, *listC);

        /* Línea en blanco entre comités, no antes del primero */
        if (!first) printf("\n");
        first = false;

        printf("Committee %s\n", committee.committeeName);

        posP = findItemP(projectName, committee.projectList);
        if (posP == NULLP) {
            printf("No project %s\n", projectName);
        } else {
            /* Transferimos los votos del proyecto a votos nulos */
            project = getItemP(posP, committee.projectList);
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
 *           Si no hay ninguno a eliminar o la lista está
 *           vacía → error.
 * Entradas: commandNumber - número de orden de la petición;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna, imprime resultado por stdout).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: Los comités con validVotes == 0 quedan
 *                  eliminados de la lista.
 * ------------------------------------------------------- */
void processRemove(char *commandNumber, tListC *listC) {
    tPosC  posC;      /* posición del comité actual en el recorrido */
    tItemC committee; /* datos del comité actual */
    bool   removed;   /* indica si se eliminó al menos un comité */

    printf("********************\n");
    printf("%s R:\n", commandNumber);

    if (isEmptyListC(*listC)) {
        printf("+ Error: Remove not possible\n");
        return;
    }

    removed = false;
    posC    = firstC(*listC);

    while (posC != NULLC) {
        committee = getItemC(posC, *listC);

        if (committee.validVotes == 0) {
            printf("* Remove: committee %s\n", committee.committeeName);

            /* Vaciamos la lista de proyectos antes de borrar el comité
               (precondición de deleteAtPositionC) */
            while (!isEmptyListP(committee.projectList)) {
                deleteAtPositionP(firstP(committee.projectList),
                                  &committee.projectList);
            }
            updateItemC(committee, posC, listC);
            deleteAtPositionC(posC, listC);
            removed = true;
            /* Tras el borrado, los índices se desplazan -1: el mismo posC
               apunta ahora al elemento que era posC+1, así que no avanzamos */
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
 * Objetivo: Para cada comité muestra el proyecto ganador en
 *           cada categoría (eco y non-eco). Si hay empate o
 *           no hay proyectos en la categoría → "No winner".
 *           Un único proyecto con 0 votos sí es ganador.
 *           Lista vacía → error.
 * Entradas: commandNumber - número de orden de la petición;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna, imprime resultado por stdout).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void processWinners(char *commandNumber, tListC *listC) {
    tPosC  posC;             /* posición del comité actual en el recorrido */
    tItemC committee;        /* datos del comité actual */
    tPosP  posP;             /* posición del proyecto actual en el recorrido */
    tItemP proj;             /* datos del proyecto actual */
    bool   firstCommittee;   /* controla la línea en blanco entre comités */

    /* Variables para rastrear el ganador de la categoría eco */
    tPosP winnerEcoPos;      /* posición del proyecto eco con más votos */
    int   winnerEcoVotes;    /* votos del líder eco (-1 = sin candidatos aún) */
    bool  ecoTie;            /* true si hay empate en el máximo eco */
    int   ecoCount;          /* número de proyectos eco en el comité */

    /* Variables para rastrear el ganador de la categoría non-eco */
    tPosP winnerNonEcoPos;   /* posición del proyecto non-eco con más votos */
    int   winnerNonEcoVotes; /* votos del líder non-eco */
    bool  nonEcoTie;         /* true si hay empate en el máximo non-eco */
    int   nonEcoCount;       /* número de proyectos non-eco en el comité */

    tItemP winner;           /* proyecto ganador al imprimir el resultado */

    printf("********************\n");
    printf("%s W:\n", commandNumber);

    if (isEmptyListC(*listC)) {
        printf("+ Error: Winners not possible\n");
        return;
    }

    posC           = firstC(*listC);
    firstCommittee = true;

    while (posC != NULLC) {
        committee = getItemC(posC, *listC);

        if (!firstCommittee) printf("\n");
        firstCommittee = false;

        printf("Committee %s\n", committee.committeeName);

        /* Inicializamos los rastreadores de ganador */
        winnerEcoPos      = NULLP;
        winnerEcoVotes    = -1;
        ecoTie            = false;
        ecoCount          = 0;

        winnerNonEcoPos   = NULLP;
        winnerNonEcoVotes = -1;
        nonEcoTie         = false;
        nonEcoCount       = 0;

        /* Recorremos los proyectos buscando el ganador de cada categoría */
        if (!isEmptyListP(committee.projectList)) {
            posP = firstP(committee.projectList);
            while (posP != NULLP) {
                proj = getItemP(posP, committee.projectList);

                if (proj.projectEco) {
                    ecoCount++;
                    if (proj.numVotes > winnerEcoVotes) {
                        winnerEcoVotes = proj.numVotes;
                        winnerEcoPos   = posP;
                        ecoTie         = false;
                    } else if (proj.numVotes == winnerEcoVotes) {
                        ecoTie = true; /* empate: no hay ganador único */
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

        /* Imprimimos ganador eco:
           gana si hay exactamente 1 proyecto, o si hay más y no hay empate */
        if (ecoCount == 0 || (ecoCount > 1 && ecoTie)) {
            printf("Category eco: No winner\n");
        } else {
            winner = getItemP(winnerEcoPos, committee.projectList);
            printf("Category eco: Project %s numvotes %d\n",
                   winner.projectName, winner.numVotes);
        }

        /* Imprimimos ganador non-eco */
        if (nonEcoCount == 0 || (nonEcoCount > 1 && nonEcoTie)) {
            printf("Category non-eco: No winner\n");
        } else {
            winner = getItemP(winnerNonEcoPos, committee.projectList);
            printf("Category non-eco: Project %s numvotes %d\n",
                   winner.projectName, winner.numVotes);
        }

        posC = nextC(posC, *listC);
    }
    printf("\n");
}

/* -------------------------------------------------------
 * processStats
 * Objetivo: Muestra las estadísticas de todos los comités.
 *           Si la lista está vacía → error.
 * Entradas: commandNumber - número de orden de la petición;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna, imprime resultado por stdout).
 * Precondiciones: La lista de comités está inicializada.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void processStats(char *commandNumber, tListC *listC) {
    tPosC pos; /* posición del comité actual en el recorrido */

    printf("********************\n");
    printf("%s S:\n", commandNumber);

    if (isEmptyListC(*listC)) {
        printf("+ Error: Stats not possible\n");
        return;
    }

    pos = firstC(*listC);
    while (pos != NULLC) {
        printStats(getItemC(pos, *listC));
        pos = nextC(pos, *listC);
        printf("\n"); /* línea en blanco entre comités */
    }
}

/* -------------------------------------------------------
 * processCommand
 * Objetivo: Despacha cada operación leída del fichero al
 *           procesador correspondiente según su tipo.
 * Entradas: commandNumber - número de orden de la petición;
 *           command       - carácter que identifica la operación
 *                           (C, N, V, S, D, R, W);
 *           param1        - primer parámetro de la operación;
 *           param2        - segundo parámetro de la operación;
 *           param3        - tercer parámetro de la operación;
 *           listC         - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: La lista de comités está inicializada.
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
 * Objetivo: Lee el fichero de entrada línea a línea,
 *           tokeniza cada operación y llama a processCommand.
 * Entradas: filename - ruta del fichero de operaciones;
 *           listC    - puntero a la lista de comités.
 * Salidas: (ninguna).
 * Precondiciones: El fichero existe y tiene el formato esperado.
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
void readTasks(char *filename, tListC *listC) {
    FILE       *f;             /* puntero al fichero de entrada */
    char       *commandNumber; /* número de la petición (token 1) */
    char       *command;       /* tipo de operación (token 2) */
    char       *param1;        /* primer parámetro (token 3) */
    char       *param2;        /* segundo parámetro (token 4) */
    char       *param3;        /* tercer parámetro (token 5) */
    const char  delimiters[] = " \n\r"; /* separadores para strtok */
    char        buffer[MAX_BUFFER];     /* buffer de lectura de línea */

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
 * Objetivo: Declara e inicializa la lista de comités en local,
 *           determina el fichero de entrada y lanza readTasks.
 * Entradas: nargs - número de argumentos de línea de comandos;
 *           args  - array de argumentos de línea de comandos.
 * Salidas: 0 si la ejecución fue correcta.
 * Precondiciones: (ninguna).
 * Postcondiciones: (ninguna).
 * ------------------------------------------------------- */
int main(int nargs, char **args) {
    char   *file_name; /* nombre del fichero de entrada */
    tListC  listC;     /* lista de comités evaluadores */

    file_name = "create.txt";

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