/* 
   Erik Soto 
   Lab 5
   cmp223
   april 12
   */ 

#include <stdio.h>
#define NULL_VALUE -1
#define DATA_COL 0
#define NEXT_COL 1
#define IN_USE_COL 2

typedef struct arraylist{
    int list[100] [3]; 

    int next_free;
    int head;
} ArrayList;

/* INITIALIZES AND SETS VALUES */

void init(ArrayList * al)
{
    al->head = NULL_VALUE;
    al->next_free = 0;
    int i= 0;
    for( i; i < 100; i++)
    {
        al->list[i][DATA_COL] = DATA_COL;
        al->list[i][NEXT_COL] = NULL_VALUE;
        al->list[i][IN_USE_COL] = 0;

    }

}
/* FINDS NEXT EMPTY VALUE */

int find_next(ArrayList al)
{
    if(al.next_free != NULL_VALUE)
    {
        return al.next_free;
    }
    else


        return -1;

}
/* checks if array is FULL */

int is_full( ArrayList al)
{

    if(al.next_free == NULL_VALUE)
    {
        return 1;
    }

    else 
        return 0;
}

/* checks to see if array is empty*/

int is_empty(ArrayList al)
{
    if(al.head == NULL_VALUE)
    {
        return 1;
    }
    else 
        return 0;

}
/* adds a value to the array */

int add(ArrayList * al , int value)
{   
    int chk = is_full( *al);
    int chk2 = is_empty(*al);

    if(chk == 0 && chk2 == 1)
    {
        int nxt = find_next(* al);

        al->list[nxt][DATA_COL] = value;
        al->list[nxt][2] = 1;
        al->next_free = nxt +1;
        al->head =nxt; 
        return 1;

    }
      /* if array is not empty */

    if(chk == 0 && chk2 == 0)
    {
        int nxt = find_next(* al);
        al->list[nxt][0] = value;
        al->list[nxt][2] = 1;
        al->next_free = nxt+1;
        al->head = nxt;
        return 1;
    }


    return 0;
}
/* shoes every single slot*/

void show_all(ArrayList *al)
{   
    int i =0;
    for( i; i < 100; i++)
    {
        printf("%5d " , al->list[i][DATA_COL]);
  
    }   
}
/* shows only the list */

void show_list(ArrayList *al)
{
    int i =0;
    for(i; i<100; i++)
    {
        if(al->list[i][2] == 1 )
        {

        printf("%5d" , al->list[i][0]);
        }

    }

}



int main()
{
    ArrayList al;


    char yeet;
    do
    {
        /* menu and switch statements doing the task enetered */

        printf("\n");
        printf("\n");
        printf("Add             A \n");
        printf("Show List       B  \n");
        printf("Show All        C \n");
        printf("Is empty        D \n");
        printf("Is Full         E \n");
        printf("Find Next       F \n");
        printf("Init            G \n");

        scanf("%c", &yeet);


        switch(yeet)
        {
            case 'A':
                {

                    int choice1;

                    printf(" what number do you want to add \n");
                    scanf("%d", &choice1);
                    int res = add(& al, choice1); 
                    if(res == 1)
                    {
                        printf(" ADDED! \n");
                    }
                    else 
                        printf( " IS FULL \n");

                    break;
                }
            case 'B':
                show_list(&al);

                break;

            case 'C':
                show_all(&al);
                break;

            case 'D':

                {
                    int  empty = is_empty(al);
                    if(empty == 1)
                    {
                        printf(" Empty\n ");
                    }
                    else 
                        printf(" IS not empty\n ");

                    break;
                }
            case 'E':
                {
                    int full;
                    full = is_full(al);

                    if(full = 1)
                    {
                        printf(" IS FULL \n");
                    }
                    else 
                        printf(" not full \n");


                    break;  
                }

            case 'F':
                {
                    int n;
                    n= find_next(al);
                    if(n != -1)
                    {
                        printf(" next free is at: %d \n",n);
                    }
                    else 
                        printf("FULL\n");
                    break;

                    case 'G':
                                init(&al);
                    break;

                }
        }
    }while(yeet != 'Q');

    return 0;

}

