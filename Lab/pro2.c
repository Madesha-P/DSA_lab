#include <stdio.h>
#include <stdlib.h>
#define smax 5
void push();
void pop();
void palindrome();
void display();
int top=-1,stk[smax];

void main()
{


    int ch,d;
    while(1)
    { printf("\n Stack Menu :-");
        printf("\n1.Push\n2.Pop\n3.Display\n4.Palindrome\n5.Exit");
        printf("\nEnter your choice:");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1: push();
                    break;
            case 2: pop();
                    break;
            case 3: display();
                    break;
            case 4: palindrome();
                    break;
            case 5: exit(0);
            default: printf("\nInvalid choice");
        }
    }
    }
    void push(){    
        int item;
        if(top==smax-1)
        {
            printf("\nStack Full");
        }
        else
        {
            printf("\nEnter the item to be pushed:");
            scanf("%d",&item);
            stk[++top]=item;
        }}

        void pop(){
            int d;
            if (top==-1)
            {
                printf("\nStack is Empty");
            }
            else
            {
                d=stk[top--]; // d=stk[top];
                // Since we have used top--, the value of d will be assigned the value of the top item in the stack before it is decremented. This means that the top item will be removed from the stack and the top index will be updated to point to the new top item.
                // We can use top-- to decrement the top index after popping the item from the stack. This effectively removes the top item from the stack and updates the top index to point to the new top item.
                // top --;
                printf("\nPopped item is %d",d);
            }
        }

void display()
{
    int i;
    if (top == -1)
        printf("\nStack is Empty");
    else
    {
        printf("\nStack elements are:\n");
        for (i = top; i >= 0; i--)
            printf("%d\n", stk[i]);
    }
}

void palindrome()
{
    int i, j = 0, flag = 1;
    int rev[smax];

    if (top == -1 || top == 0)
    {
        printf("\ncannot check for palindrome");
        return;
    }

    for (i = top; i >= 0; i--)
        rev[j++] = stk[i];

    for (i = 0; i <= top; i++)
    {
        if (stk[i] != rev[i])
        {
            flag = 0;
            break;
        }
    }

    if (flag){
          printf("\nStack is a palindrome");
    }
        
    else{
        printf("\nStack is not a palindrome");
    }
       
}

