#include <stdio.h>
#include <stdlib.h>
// SLL with void functions

typedef struct node {
    int rno;
    char name[25];
    struct node *link;
} NODE;

void getnode(NODE **x) {
    *x = (NODE *)malloc(sizeof(NODE));
    if (*x == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter the information of NODE (Roll No and Name): ");
    scanf("%d %24s", &(*x)->rno, (*x)->name);
    (*x)->link = NULL;
}

void insert_front(NODE **first) {
    NODE *temp;
    getnode(&temp);
    temp->link = *first;
    *first = temp;
}

void insert_rear(NODE **first) {
    NODE *temp;
    NODE *cur;

    getnode(&temp);
    if (*first == NULL) {
        *first = temp;
        return;
    }

    cur = *first;
    while (cur->link != NULL) {
        cur = cur->link;
    }
    cur->link = temp;
}

void delete_front(NODE **first) {
    NODE *cur;

    if (*first == NULL) {
        printf("List is Empty\n");
        return;
    }

    cur = *first;
    *first = (*first)->link;
    free(cur);
}

void delete_rear(NODE **first) {
    NODE *cur;
    NODE *prev;

    if (*first == NULL) {
        printf("List is Empty\n");
        return;
    }

    if ((*first)->link == NULL) {
        free(*first);
        *first = NULL;
        return;
    }

    prev = NULL;
    cur = *first;
    while (cur->link != NULL) {
        prev = cur;
        cur = cur->link;
    }
    prev->link = NULL;
    free(cur);
}

void search(NODE *first) {
    int rno;
    NODE *cur;

    if (first == NULL) {
        printf("List is Empty\n");
        return;
    }

    printf("Enter the rno of Student to be Searched: ");
    scanf("%d", &rno);

    cur = first;
    while (cur != NULL && cur->rno != rno) {
        cur = cur->link;
    }

    if (cur != NULL) {
        printf("Element found = %d (%s)\n", cur->rno, cur->name);
    } else {
        printf("Element not found\n");
    }
}

void display(NODE *first) {
    NODE *cur;
    int count = 0;

    if (first == NULL) {
        printf("No Data present, List is Empty\n");
        return;
    }

    printf("\nRoll No\tName\n");
    cur = first;
    while (cur != NULL) {
        printf("%d\t%s\n", cur->rno, cur->name);
        cur = cur->link;
        count++;
    }
    printf("No of nodes is %d\n", count);
}

int main(void) {
    NODE *first = NULL;
    int ch;

    while (1) {
        printf("\n--- Singly Linked List Operations ---\n");
        printf("1. Insert Front\n");
        printf("2. Insert Rear\n");
        printf("3. Delete Front\n");
        printf("4. Delete Rear\n");
        printf("5. Search\n");
        printf("6. Display\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1:
                insert_front(&first);
                break;
            case 2:
                insert_rear(&first);
                break;
            case 3:
                delete_front(&first);
                break;
            case 4:
                delete_rear(&first);
                break;
            case 5:
                search(first);
                break;
            case 6:
                display(first);
                break;
            case 7:
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }
}
