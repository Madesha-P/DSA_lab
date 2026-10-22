// iNITIALIZE THE STACK AND CONVERT INFIX TO POSTFIX

#include <stdio.h>
#include <stdlib.h>
int spre(char);
int inputpre(char);
char pop();
void push(char);
char postfix[20], infix[20],S[50];
int top=-1;

void main(){
    int i, j=0;
    char ch, ele;
    printf("Enter a valid Infix Expression:");
    gets(infix);
    push('#');  
    for ( i = 0; infix[i] != '\0'; i++)
    {ch = infix[i];
        if (isalnum(ch))
        {
            postfix[j++] = ch;
        }
        else if (ch == '(')
        {
            push(ch);
        }
        else if (ch == ')')
        {
            while (S[top] != '(')
            {
                postfix[j++] = pop();
            }
            ele = pop(); // pop '(' from stack  
        }
        else
        {
            while (spre(S[top]) >= inputpre(ch))
            {
                postfix[j++] = pop();
            }
            push(ch);
        }
    }
    while (S[top] != '#')
    {
        postfix[j++] = pop();
    }
    postfix[j] = '\0';
    printf("Postfix Expression is: %s\n", postfix);
}

int inputpre(char ch)
{
    switch (ch)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
    case '%':
        return 3;
    case '^':
        return 6;
    case '(':
    case '#':
        return 0;
    }
}

int spre(char ch)
{
    switch (ch)
    {
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
    case '%':
        return 4;
    case '^':
        return 5;
    case '(':
    case '#':
        return 0;
    }
}
void push(char ele)
{
    S[++top] = ele;
}
char pop()
{
    return (S[top--]);
}       