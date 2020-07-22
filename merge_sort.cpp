#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 15


typedef struct halves {
    int* array1;
    int* array2;
    int size1;
    int size2;
} splitted;

int* merge(int* array, int* array2, int* size, int* size2);
int* divide_left(int* array, int* new_array, int MAX, int* size);
int* divide_right(int* array, int* new_array2, int MAX, int* size);
splitted divide(int* array, int* size);
void swap(int* array, int* array2, int i, int j);
void readFile(char* string);

//initialize global counter
int div_count = 0;

//function that sorts two numbers
void swap(int* array, int i, int j)
{
    int n = 0;
    n = array[i];
    array[i] = array[j];
    array[j] = n;
}


//function that sorts the left half
int* divide_left(int* array, int* new_array, int MAX, int *size)
{
    
    int half1[MAX];
    int half2[MAX];

    if(MAX == 1)
    {
        div_count++;
        
        if(div_count%2 == 0)
        {   
            *(new_array + div_count-1) = array[0];

            if(new_array[div_count-2] > new_array[div_count-1])
            {
                swap(new_array, div_count-2, div_count-1);
            }
            
        }
        
        if(div_count%2 == 1){

            *(new_array + div_count-1) = array[0];
       
        }

        //if the splitting process is done, merge halves
        if(div_count == *size)
        {
            
            for(int i = 0; i< *size-1; i++)
            {
                for(int j = i +1; j< *size; j++)
                {
                    if(new_array[i] > new_array[j])
                    {
                        swap(new_array, i, j);
                    }
                }
            }
            
            
        }
        
    }
    else
    {
        //odd case
        if(MAX%2 == 1)
        {
            
            MAX = MAX/2;
            //fill 1st half
            for(int i = 0; i<MAX; i++)
            {
                half1[i] = array[i];
            }

            //fill 2nd half
            for(int i = 0; i<MAX+1; i++)
            {
                half2[i] = array[i + MAX];
            }

            new_array = divide_left(half1, new_array, MAX, size);

            new_array = divide_left(half2, new_array, MAX+1, size);
        //even case
        }else{
            
            MAX = MAX/2;
            
            //fill 1st half
            for(int i = 0; i<MAX; i++)
            {
                half1[i] = array[i];
            }

            //fill 2nd half
            for(int i = 0; i<MAX; i++)
            {
                half2[i] = array[i + MAX];
            }

            new_array = divide_left(half1, new_array, MAX, size);

            new_array = divide_left(half2, new_array, MAX, size);
        } 


    }
    
    return new_array;
       
}

//function that sorts the right half
int* divide_right(int* array, int* new_array2, int MAX, int *size)
{
    
    int half1[MAX];
    int half2[MAX];

    if(MAX == 1)
    {
        div_count++;

        if(div_count%2 == 0)
        {   
            *(new_array2 + div_count-1) = array[0];

            if(new_array2[div_count-2] > new_array2[div_count-1])
            {
                swap(new_array2, div_count-2, div_count-1);
            }
            
        }
        
        if(div_count%2 == 1){

            *(new_array2 + div_count-1) = array[0];
       
        }

        //if the splitting process is done merge halves
        if(div_count == *size)
        {
            
            for(int i = 0; i< *size-1; i++)
            {
               for(int j = i +1; j<*size; j++)
                {
                    if(new_array2[i] > new_array2[j])
                    {
                        swap(new_array2, i, j);
                    }
                }
            }
            
        }
        
    }
    else
    {
        //odd case
        if((int)MAX%2 == 1)
        {
            
            MAX = MAX/2;
            //fill 1st half
            for(int i = 0; i<MAX; i++)
            {
                half1[i] = array[i];
            }

            //fill 2nd half
            for(int i = 0; i<MAX+1; i++)
            {
                half2[i] = array[i + MAX];
            }
            
            new_array2 = divide_right(half1, new_array2, MAX, size);

            new_array2 = divide_right(half2, new_array2, MAX+1, size);
        //even case
        }else{
            
            MAX = MAX/2;
            
            //fill 1st half
            for(int i = 0; i<MAX; i++)
            {
                half1[i] = array[i];
            }

            //fill 2nd half
            for(int i = 0; i<MAX; i++)
            {
                half2[i] = array[i + MAX];
            }

            new_array2 = divide_right(half1, new_array2, MAX, size);

            new_array2 = divide_right(half2, new_array2, MAX, size);

        }

    }
    
    return new_array2;
       
}

splitted divide(int* array, int* size)
{
    splitted big_array;
    //declare size variables
    int new_size = 0;
    int new_size2 = 0;

    //declare new halves of the array
    int* new_array = NULL;
    int* new_array2 = NULL;

    //check if array is odd
    bool odd = false;

    //initialize new process
    pid_t new_pid;

    new_size = *size/2;
    new_size2 = *size/2;

    new_array = (int *) malloc(new_size* sizeof(int));
    new_array2 = (int *) malloc(new_size2* sizeof(int));

    //if odd, resize second half of the array
    if(*size%2 == 1)
    {
        odd = true;
        new_size2++;
        new_array2 = (int*) realloc(new_array2, new_size2 * sizeof(int));
    }


    //fill in both halves of the original array
    if(odd)
    {
    
        for(int i = 0; i<new_size; i++)
        {
            new_array[i] = array[i];
        }

        for(int i = 0; i<new_size2; i++)
        {
            new_array2[i] = array[i + new_size];
        }

        //create a new process
        new_pid = fork();
        //make two recursive calls
        if(new_pid == 0)
        {
            //child sorts left half
            new_array = divide_left(new_array, new_array, new_size, &new_size);
            exit(1);
        }
        //reset counter
        //div_count = 0;
        else if (new_pid > 0)
        {
            //parent sorts right half
            new_array2 = divide_right(new_array2, new_array2, new_size2, &new_size2);
            //wait for the left half to be sorted
            wait(NULL);
        }

    }else{

        for(int i = 0; i<new_size; i++)
        {
            new_array[i] = array[i];
        }

        for(int i = 0; i<new_size2; i++)
        {
            new_array2[i] = array[i + new_size];
        }

        new_pid = fork();
        //make two recursive calls
        if(new_pid == 0)
        {
            //child sorts left half
            new_array = divide_left(new_array, new_array, new_size, &new_size);
            exit(1);
        }
        //reset counter
        //div_count = 0;
        else if(new_pid > 0)
        {
            //parent sorts right half
            new_array2 = divide_right(new_array2, new_array2, new_size, &new_size2);
            //wait for the left half to be sorted
            wait(NULL);
        }

        

    }  

    //store values
    big_array.size1 = new_size;
    big_array.size2 = new_size2;
    big_array.array1 = (int *) malloc(new_size* sizeof(int));
    big_array.array2 = (int *) malloc(new_size2* sizeof(int));

    for(int i = 0; i< new_size; i++)
    {
        big_array.array1[i] = new_array[i];
    }

    for(int i = 0; i< new_size2; i++)
    {
        big_array.array2[i] = new_array2[i];
    }


    //free memory
    free(new_array);
    free(new_array2);

    return big_array;

}


int* merge(int* array, int* array2, int* size, int* size2)
{
    int* sorted_array = NULL;
    int original_size = 0;
    original_size = *size + *size2;
    sorted_array = (int*)malloc(original_size * sizeof(int));
    //fill sorted first half
    for(int i = 0; i<*size; i++)
    {
        sorted_array[i] = array[i];
    }
    //fill sorted second half
    for(int i = 0; i<*size2; i++)
    {
        sorted_array[i + *size] = array2[i];
    }

    //make the last big merge
    for(int i = 0; i<original_size-1; i++)
    {
        for(int j = i+1; j<original_size; j++)
        {
            if(sorted_array[i] > sorted_array[j])
            {
                swap(sorted_array, i, j);
            }
        }
    }

    return sorted_array;

}

void readFile(char* string)
{
    //initialize array
    int *array = NULL;
    int *sorted_array = NULL;
    int size = 0;

    //initialize data struct
    splitted big_array;

    //initialize file pointers
    FILE * file;
    FILE * sorted_file;
    file = fopen(string, "r+");

    if(!file)
    {
        printf("ERROR opening file");
        exit(1);
    }
    //store size
    fscanf(file, "%d", &size);

    //allocate memory to arrays
    array = (int *) malloc(size * sizeof(int));
    sorted_array = (int *) malloc(size * sizeof(int));

    //store numbers
    for(int i = 0; i<size; i++)
    {
       fscanf(file, "%d", &array[i]);
    }
    
    //call divide and merge functions
    big_array = divide(array, &size);

    sorted_array = merge(big_array.array1, big_array.array2, &big_array.size1, &big_array.size2);
   
    //create new file to store sorted numbers
    sorted_file = fopen("sorted_numbers.txt", "w");

    //print sorted array
    printf("the sorted numbers are: \n");
    for(int i = 0; i<size; i++)
    {
        printf("%d ", sorted_array[i]);
        fprintf(sorted_file, "%d\t", sorted_array[i]);
    }

    printf("\n");

    //free memory
    free(array);
    free(sorted_array);
}

int main(int argc, char *argv[])
{   
    //initialize string to read file name
    char string[BUFFER_SIZE];

    if(argc >= 2)
    {
        strcpy(string, argv[1]);
    }
    else{
        printf("%s + file.txt\n", argv[0]);
        return 1;
    }

    //send file
    readFile(string);
     
    return 0;
} 
    
     
 
