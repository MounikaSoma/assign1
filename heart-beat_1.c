#include<stdio.h>
#include<stdlib.h>
struct node
{
	int data;
	struct node* next;
	struct node* previous;
};

void insert(int data, struct node** Head)
{
	struct node* temp, *new;
	if(*Head == NULL)
	{
		*Head = (struct node*)malloc(sizeof(struct node));
		(*Head)->data=data;
		(*Head)->next = NULL;
		(*Head)->previous = NULL;
	}
	else
	{
		temp = *Head;
		while(temp->next != NULL)
			temp = temp->next;
		new = (struct node*)malloc(sizeof(struct node));
		new->data = data;
		temp->next = new;
		new->next = NULL;
		new->previous = temp;
	}
}
int* sum_1(int* array, int position)
{
	int i;
	static int return_value[2];
	return_value[0] = 0;
	return_value[1] = 0;
	for(i=0;i<15;i++)
	{
		//printf("%d\n", position-i-1);
		return_value[0]+=array[position-i-1];
		return_value[1]+=array[position+i+1];
	}
	return return_value;
}
int main()
{
	int number,value, max, min, count=0, *array, i, f_count = 1, peak_value, final_heart_count = 0;
	struct node* Head,*temp,*new_head;
	Head = NULL;
	temp = NULL;
	new_head = NULL;
	FILE* in_file = fopen("da.txt", "r+");
	FILE* out_file = fopen("new.txt", "w+");
	if (! in_file ) // equivalent to saying if ( in_file == NULL ) 
	{
		printf("Sorry!This file cannot be read\n"); 
		return 1;
	}
	while ( fscanf(in_file, "%d", & number ) == 1 )  
	{
		fscanf(in_file,"%d", &value);
		insert(value, &Head);
		count+=1;
	}
	//Smoothening the curve
	// array is an array that has the differences of every 2 consecutive values.
	array = (int*)malloc(sizeof(int)*count-1);
	temp = Head->next;
	// Filling values into the array
	for(i=0;i<count-1;i++)
	{
		array[i] = (temp->data) - ((temp->previous)->data);
		temp = temp->next;
	}
	temp = (Head->next)->next;
	// Checking where all errors are there by seeing if the difference between two consecutive values is greater than the 1st value in array. If the difference itself is bigger than the array[i], then it means that the values have gone so high and hence array[i] is also so high. Thus we replace the value corresponding to array[i].
	for(i=1;i<count-2;i++)
	{
		if(abs(array[i] - array[i-1]) > abs(array[i-1]))
		{
			//printf("lol!\n");
			//printf("%p\n", temp);
			temp->data = (((temp->previous)->previous)->data + (temp->previous)->data + (temp->next)->data + ((temp->next)->next)->data)/4;
			if(i<count-2)
			{
				// to update the difference array after the value at a particular position is changed.Instead of checking every position, I am just updating all the values of the array.
				array[i+1] = ((temp->next)->data) - (temp->data);
				//printf("array[i+1]:%d\n", array[i+1]);
			}
		}
		temp = temp->next;
		//printf("I am here.\n");
	}
	temp = Head;
	// writing the updated values and a counter for X-axis(f_counter) to plot a graph.
	while(temp!=NULL)
	{
		fprintf(out_file, "%d\t%d\n", f_count, temp->data);
		f_count+=1;
		temp = temp->next;
	}
	//Finding the peaks
	// Adding 15 above values and 15 below values and taking their difference. If that difference is greater than 40 and its corresponding slope i.e differences array is 0, we consider it as a peak.
	for(i=15;i<count-16;i++)
	{
		peak_value=sum_1(array,i)[0] - sum_1(array,i)[1];
		if((peak_value>40) && (array[i] == 0))
		{
			//printf("%d\n", i+1);
			insert(i+1, &new_head);
		}
	}
	//printf("insert complete.\n");
	temp = new_head;
	while((temp->next)!=NULL)
	{
		//printf("%d\n", temp->data);
		temp =temp->next;
	}
	temp = new_head;
	// To avoid consecutive peaks, we check whether the difference between the indices of any two peaks is less than 40 or not inorder to call it as a peak.
	while((temp->next)!=NULL)
	{
		//printf("%d\n", temp->data);
		final_heart_count+=1;
		while(((temp->next)->data - temp->data) < 40)
			temp = temp->next;
		temp = temp->next;
	}
	printf("Number of peaks:%d\n", final_heart_count);
	//heart count = number of peaks* 5728/ sampling_rate.
	return 0;
}
