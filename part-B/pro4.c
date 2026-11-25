#include <stdio.h>
#include <stdlib.h>

struct node
{
    int num;
    struct node *next;
};

typedef struct node NODE;

NODE* insertfront(NODE *, int);
void display(NODE *);
NODE* concatenate(NODE *, NODE *);

void main()
{
    NODE *first1 = NULL, *first2 = NULL;
    int num, choice;

    while(1)
    {
        printf("Enter your choice\n");
        printf("1. Create List1\n");
        printf("2. Create List2\n");
        printf("3. Concatenate\n");
        printf("4. Display\n");
        printf("5. Stop\n");

        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("Enter data to insert in List1\n");
                scanf("%d", &num);
                first1 = insertfront(first1, num);
                break;

            case 2:
                printf("Enter data to insert in List2\n");
                scanf("%d", &num);
                first2 = insertfront(first2, num);
                break;

            case 3:
                first1 = concatenate(first1, first2);
                break;

            case 4:
                display(first1);
                break;

            case 5:
                exit(0);
        }
    }
}

NODE* concatenate(NODE *first1, NODE *first2)
{
    NODE *curr;

    if(first1 == NULL)
        return first2;

    if(first2 == NULL)
        return first1;

    curr = first1;

    while(curr->next != NULL)
        curr = curr->next;

    curr->next = first2;

    return first1;
}