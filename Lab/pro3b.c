#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>

#define max 20

void push(int e);
int pop();

int a[max], top=-1;

void main()
{
    char post[max], ch;
    int i, op1, op2, result;

    printf("Enter a valid postfix expression: ");
    scanf("%s", post);

    for(i=0; post[i]!='\0'; i++)
    {
        ch = post[i];

        if(isdigit(ch))
        {
            push(ch-'0');
        }
        else
        {
            op2 = pop();
            op1 = pop();

            switch(ch)
            {
                case '+': result=op1+op2;
                          break;

                case '-': result=op1-op2;
                          break;

                case '*': result=op1*op2;
                          break;

                case '/': result=op1/op2;
                          break;

                case '%': result=op1%op2;
                          break;

                case '^': result=pow(op1,op2);
                          break;

                default: printf("Invalid operator");
                         exit(0);
            }

            push(result);
        }
    }

    printf("Result is %d", pop());
}

void push(int e)
{
    a[++top]=e;
}

int pop()
{
    return a[top--];
}