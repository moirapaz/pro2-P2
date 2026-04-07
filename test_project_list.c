#include <stdio.h>
#include <string.h>

#include "project_list.h"

void print_list(tListP listP) {
    tPosP posP;
    tItemP itemP;

    printf("(");
    if (!isEmptyListP(listP)) {
        posP = firstP(listP);
        while (posP != NULLP) {
            itemP = getItemP(posP, listP);
            printf(" %s numvotes %d", itemP.projectName, itemP.numVotes);
            posP = nextP(posP, listP);
        }
    }
    printf(")\n");
}

int main() {
    tListP listP;
    tPosP posP;
    tItemP itemP1, itemP2;


    /* create */
    
    createEmptyListP(&listP);
    print_list(listP);

    /* insert */
    strcpy(itemP1.projectName, "project3");
    itemP1.numVotes = 0;

    insertItemP(itemP1, &listP);
    print_list(listP);

    strcpy(itemP1.projectName, "project1");
    insertItemP(itemP1, &listP);
    print_list(listP);

    strcpy(itemP1.projectName, "project5");
    insertItemP(itemP1, &listP);
    print_list(listP);

    strcpy(itemP1.projectName, "project2");
    insertItemP(itemP1,&listP);
    print_list(listP);

    strcpy(itemP1.projectName, "project4");
    insertItemP(itemP1, &listP);
    print_list(listP);


    /* find */
    posP = findItemP("project33", listP);
    if (posP == NULLP) {
        printf("project33 Not found\n");
    }


    posP = findItemP("project3", listP);
    itemP2 = getItemP(posP, listP);
    printf("%s\n", itemP2.projectName);


    /* update */
    itemP2 = getItemP(posP, listP);
    itemP2.numVotes = 2;
    updateItemP(itemP2,posP,&listP);
    printf("%s\n", itemP2.projectName);
    print_list(listP);

    /* remove */
    deleteAtPositionP(nextP(firstP(listP),listP), &listP);
    print_list(listP);

    deleteAtPositionP(previousP(lastP(listP),listP), &listP);
    print_list(listP);

    deleteAtPositionP(firstP(listP),&listP);
    print_list(listP);

    deleteAtPositionP(lastP(listP),&listP);
    print_list(listP);

    deleteAtPositionP(firstP(listP),&listP);
    print_list(listP);

    insertItemP(itemP1, &listP);
    print_list(listP);

    deleteAtPositionP(firstP(listP),&listP);
    print_list(listP);

}
