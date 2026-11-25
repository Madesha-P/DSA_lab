#include <stdio.h>
#include<stdlib.h>
#define MAX 5
int front=-1,rear=-1;
typedef struct process
{
	int info;
	int pr;
}job;
job pjob[MAX];
void insert()
{
	int info,pr;
	if(rear==MAX-1)
	{
		printf("Overflow");
	}
	else
	{
		printf("Enter information  and its priority: ");
		scanf("%d %d",&info,&pr);
		if(rear==-1)
		{
			rear++;
			front++;
		}
		else
		{
			rear++;
		}
		pjob[rear].info=info;
		pjob[rear].pr=pr;
	}
}
void delete()
{
	int i, pos=0,max=0;
	if(front==-1)
	{
		printf("Underflow\n");
	}
	else
	{
		if(front==rear)
		{
			front=-1;
			rear=-1;
		}
		else
		{
			for(i=front;i<=rear;i++)
			{
				if(pjob[i].pr>max)
				{
					max=pjob[i].pr;
					pos=i;
				}
			}
			for(i=pos;i<=rear;i++)
			{
				pjob[i].info=pjob[i+1].info;
				pjob[i].pr=pjob[i+1].pr;
			}
			rear--;
		}
	}
}
void display()
{
	if(front==-1)
	{
		printf("Queue is Empty\n");
	}
	else
	{
		for(int i=front;i<=rear;i++)
		{
			printf("Info\t PR\n");
			printf("%d\t %d\n",pjob[i].info,pjob[i].pr);
		}
	}
}
void main()
{
	int ch;
	while(1)
	{
		printf("\n1.Insert\t 2.Display\t 3.Delete\t 4.Exit\n");
		printf("\nEnter your choice: ");
		scanf("%d", &ch);
		switch(ch)
		{
			case 1: insert();
				break;
			case 2: display();
				break;
			case 3: delete();
				break;
			case 4: exit(0);
				break;
			default: printf("\nInvalid choice:\n");
				break;
		}
	}
}