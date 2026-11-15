#include <stdio.h>
#include <stdlib.h>

// Structure definition for a node in SLL
typedef struct node {
    int rno;
    char name[25];
    struct node *link;
} NODE;

// 1. Memory Allocation (getnode)
NODE* getnode() {
    NODE *x;
    x = (NODE*)malloc(sizeof(NODE));
    printf("Enter the information of NODE (Roll No and Name): ");
    scanf("%d %s", &x->rno, x->name);
    x->link = NULL;
    return x;
}

// 2. Insert Front
NODE* insert_front(NODE *first) {
    NODE *temp;
    temp = getnode();
    if (first == NULL) {
        first = temp;
    } else {
        temp->link = first;
        first = temp;
    }
    return first;
}

// 3. Insert Rear
NODE* insert_rear(NODE *first) {
    NODE *temp, *cur;
    temp = getnode();
    if (first == NULL) {
        first = temp;
    } else {
        cur = first;
        while (cur->link != NULL) {
            cur = cur->link;
        }
        cur->link = temp;
    }
    return first;
}

// 4. Delete Front
NODE* delete_front(NODE *first) {
    NODE *cur;
    if (first == NULL) {
        printf("List is Empty\n");
    } else {
        cur = first;
        first = first->link;
        free(cur);
    }
    return first;
}

// 5. Delete Rear
NODE* delete_rear(NODE *first) {
    NODE *cur, *prev;
    if (first == NULL) {
        printf("List is Empty\n");
    } else if (first->link == NULL) {
        free(first);
        return NULL;
    } else {
        prev = NULL;
        cur = first;
        while (cur->link != NULL) {
            prev = cur;
            cur = cur->link;
        }
        prev->link = NULL;
        free(cur);
    }
    return first;
}

// 6. Search by Roll Number
void search(NODE *first) {
    int rno;
    NODE *cur;
    if (first == NULL) {
        printf("List is Empty\n");
    } else {
        printf("Enter the rno of Student to be Searched: ");
        scanf("%d", &rno);
        cur = first;
        while (cur != NULL) {
            if (cur->rno == rno) {
                break;
            }
            cur = cur->link;
        }
        if (cur != NULL) {
            printf("Element found = %d (%s)\n", cur->rno, cur->name);
        } else {
            printf("Element not found\n");
        }
    }
}

// 7. Traversal (Display)
NODE* display(NODE *first) {
    NODE *cur;
    int count = 0;
    if (first == NULL) {
        printf("No Data present, List is Empty\n");
    } else {
        cur = first;
        printf("\nRoll No\tName\n");
        while (cur != NULL) {
            printf("%d\t%s\n", cur->rno, cur->name);
            cur = cur->link;
            count++;
        }
        printf("No of nodes is %d\n", count);
    }
    return first;
}

// Main function with menu-driven switch cases
int main() {
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
                first = insert_front(first);
                break;
            case 2:
                first = insert_rear(first);
                break;
            case 3:
                first = delete_front(first);
                break;
            case 4:
                first = delete_rear(first);
                break;
            case 5:
                search(first);
                break;
            case 6:
                first = display(first);
                break;
            case 7:
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}