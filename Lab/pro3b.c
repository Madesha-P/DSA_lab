// Evaluation part

#include<stdio.h>
#include<ctype.h>
#include<math.h>
#define max 20

void push(int e);
int pop();
int a[max], top=-1;

void main(){
    char post[max],ch;
    int i ,op1,op2, result,op;
    printf("\n Enter a valid postfix expression :");
    scanf("%s ", post);
    for (i=0;post[i]!='\0';i++){
        ch =post[i];
        if (isdigit(ch)){
            push(ch-'0');
        }
        
        else{op2=pop();
            op1=pop();
            
        
        switch(op){
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
        

            default:
            printf("Invalid choice !");

            
        }
        push(result);
         }
    }
    printf("Result is %d",pop());

}

void push(int e ){
    ++ top;
    a[top]=e;
}

int pop(){
    int ele;
    ele =a[top];
    -- top;
    return (ele);
}
