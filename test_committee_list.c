#include <stdio.h>
#include <string.h>

#include "committee_list.h"

void print_list(tListC listC) {
    tPosC posC;
    tItemC itemC;

    printf("(");
    if (!isEmptyListC(listC)) {
        posC = firstC(listC);
        while (posC != NULLC) {
            itemC = getItemC(posC, listC);
            printf(" %s totalEvaluators %d",  itemC.committeeName, itemC.totalEvaluators);
            posC = nextC(posC, listC);
        }
    }
    printf(")\n");
}

int main() {
    tListC listC;
    tPosC posC;
    tItemC itemC1, itemC2;


    /* create */
    
    createEmptyListC(&listC);
    print_list(listC);

    /* insert */
    strcpy(itemC1.committeeName, "committee3");
    itemC1.totalEvaluators = 10;

    insertItemC(itemC1, &listC);
    print_list(listC);

    strcpy(itemC1.committeeName, "committee1");
    insertItemC(itemC1, &listC);
    print_list(listC);

    strcpy(itemC1.committeeName, "committee5");
    insertItemC(itemC1, &listC);
    print_list(listC);

    strcpy(itemC1.committeeName, "committee2");
    insertItemC(itemC1,&listC);
    print_list(listC);

    strcpy(itemC1.committeeName, "committee4");
    insertItemC(itemC1, &listC);
    print_list(listC);


    /* find */
    posC = findItemC("committee33", listC);
    if (posC == NULLC) {
        printf("committee33 Not found\n");
    }


    posC = findItemC("committee3", listC);
    itemC2 = getItemC(posC, listC);
    printf("%s\n", itemC2.committeeName);


    /* update */
    itemC2 = getItemC(posC, listC);
    itemC2.totalEvaluators = 15;
    updateItemC(itemC2,posC,&listC);
    printf("%s\n", itemC2.committeeName);
    print_list(listC);

    /* remove */
    deleteAtPositionC(nextC(firstC(listC),listC), &listC);
    print_list(listC);

    deleteAtPositionC(previousC(lastC(listC),listC), &listC);
    print_list(listC);

    deleteAtPositionC(firstC(listC),&listC);
    print_list(listC);

    deleteAtPositionC(lastC(listC),&listC);
    print_list(listC);

    deleteAtPositionC(firstC(listC),&listC);
    print_list(listC);

    insertItemC(itemC1, &listC);
    print_list(listC);

    deleteAtPositionC(firstC(listC),&listC);
    print_list(listC);

}
