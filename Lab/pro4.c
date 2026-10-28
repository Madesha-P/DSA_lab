
/// Circular queue

#include<stdio.h>
#include<stdlib.h>
#define max 4
void push();
void pop();
void display();

int queue[max];
int R=-1;
int F=-1;
int count=0;

void main()
{


    int ch;
    while(1)
    { printf("\n Circular Queue Menu :-");
        printf("\n1.Push\n2.Pop\n3.Display\n4.Exit");
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
            case 4: exit(0);
            default: printf("\nInvalid choice");
        }
    }
    }



    void push(){
        char ele, ch;
        if (count>=max)

        {
           printf("\n Queue is full !");

        } 

        else{
            ch=getchar();
            printf("\n Enter a element to insert : ");

            scanf("%c",&ele);

            if (F==-1){
                F++;
            }

            R=(R+1)%max;
            queue[R] =ele;
            count++;
            printf("The value of count is %d",count);
       }
    }

    void pop(){
            char d;
            if (count<=0)
            {
                printf("\n queue is Empty");
                return;
            }
            else
            {
                d=queue[F]; 
                F=(F+1)%max;
                printf("\nPopped item is %c",d);
                count --;
            }
        }

void display()
{
    int i,j;
    if (count <= 0)
        printf("\nQueue is Empty");
    else
    {i=F;
        printf("\nThe value of i is : %d",i);
        printf("\nCircular Q  elements are:\n");
        for (j = 1; j <= count; j++){
            printf("%c\t", queue[i]);
            i=(i+1)%max;
        }
    }
}
