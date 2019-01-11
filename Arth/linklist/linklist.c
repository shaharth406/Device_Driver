#include<stdio.h>
#include<stdlib.h>

struct node 
{

	int info;
	struct node *next;

};


struct node *START=NULL;

struct node* createNode()
{

	struct node *temp;

	temp=(struct node*)malloc(sizeof(struct node));
	return temp;

}

void InsertNode()
{
	struct node *temp,*t;

	temp=createNode();
	printf("Insert the no:\n");
	scanf("%d",&temp->info);
	temp->next=NULL;
	if(START==NULL)
		START=temp;
	else
	{
		t=START;
		while(t->next !=NULL)
			t=t->next;
		t->next=temp;



	}
}

void DeleteNode()
{
	struct node *r;
	r=START;
	if(START==NULL)
		printf("List is empty");
	else
	{
		START=START->next;
		free(r);
	}


}

void viewlist()
{
	struct node *t;

	if(START==NULL)
		printf("List is empty\n");
	else
	{
		t=START;
		while(t!=NULL)
		{
			printf("%d",t->info);
			t=t->next;
		}

	}



}

int menu()
{

	int ch;

	printf("Enter the choice:\n");
	printf("1.Insert Node\n");
	printf("2.Delete Node\n");
	printf("3.Viewlist\n");
	printf("4.Exit\n");
	scanf("%d",&ch);
	return ch;

}


main()
{

	while(1)
	{

		switch(menu())
		{

			case 1:
				InsertNode();
				break;
			case 2:
				DeleteNode();
				break;
			case 3:
				viewlist();
				break;
			case 4:
				exit(0);
				
			default:
				printf("Enter valid choice:\n");
				break;







		}


	}




}


