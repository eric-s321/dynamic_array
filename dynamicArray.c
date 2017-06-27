#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* TODO
 * make initial size power of 2 
 * make add command compatible with many integers at once
 */

typedef struct{
    int *head;
    int currentSize;
    int maxSize;
}ArrayInfo;

void printArray(ArrayInfo arrayInfo){
    printf("Array contents...\n");
    printf("Current size: %d\n", arrayInfo.currentSize);
    printf("Maximum size: %d\n", arrayInfo.maxSize);
    for(int i = 0; i < arrayInfo.currentSize; i++){
        printf("%d\n", arrayInfo.head[i]); 
    }
}

void displayInstructions(){
    printf("Type 'add <int_to_add>' to add an integer to your array, 'p' to print out the current contents of your array, 'h' for help, or 'q' to quit this program.\n");
}

/*
 *Splits up the input string into an array of strings.
 *i.e. "add 100" will become {"add", "100"}
 *Returns NULL for invalid input (more then 2 words)
 */
const char** getSplitInput(char *input){
    const char *delimiter = " ";
    const int strLen = 255; 
    const int numStrings = 2;

    //Hey Steve you always ask when to use malloc for an array right? (this is a 2d array but same concept)
    //I had to use malloc here. If I didn't and did splitInput[][] then it would have been allocated on the stack instead of the heap.
    //This would be bad because if it was on the stack that memory would be erased after this function returns. So back in main when I read the array
    //it wouldn't be there and I'd read some garbage values or possibly seg fault if there was null memory there.
    const char **splitInput = malloc(numStrings * strLen * sizeof(char));
    char *word;

    //grab the first word
    word = strtok(input, delimiter);
    int i = 0;

    //Go through all words in the input
    while(word != NULL){
        //Means we read more then 2 words which isn't a valid input
        if(i > 1)
            return NULL;

        splitInput[i] = word;
        word = strtok(NULL, delimiter); //Passing NULL here tells strtok to keep parsing the original string
        i++;
    }

    return splitInput;
}


void addToArray(int num, ArrayInfo *arrayInfo){
    //This should never happen. Just an error check
    if(arrayInfo->currentSize > arrayInfo->maxSize){
        fprintf(stderr, "Error. Current size if bigger then the maximum size allowed\n");
        return;
    }

    //The array is filled up. Get some more space 
    else if(arrayInfo->currentSize == arrayInfo->maxSize){
        int oldSize = arrayInfo->maxSize;

        //Double the size of the array
        int *newArray = (int *) malloc(oldSize * 2 * sizeof(int));
        
        //Copy over old array into new array
        for(int i = 0; i < oldSize; i++)
            newArray[i] = arrayInfo->head[i];
        newArray[oldSize] = num; //Add new num to array

        free(arrayInfo->head); //Free the memory for the old array

        //Make head point at the new array
        arrayInfo->head = newArray;
        arrayInfo->maxSize = oldSize * 2;
        arrayInfo->currentSize += 1;
    } 

    //There is enough space. Append the int
    else{
        int nextIndex = arrayInfo->currentSize;
        arrayInfo->head[nextIndex] = num;
        arrayInfo->currentSize += 1;
    }
}

int main(int argc, char *argv[]){
    
    int numElementsIn;
    printf("How many ints would you like in the original array?\n");

    //Make sure they typed in an int
    //Scanf returns the number of successfully read values 
    if(scanf("%d", &numElementsIn) != 1){
        printf("Not an int! Idiot.");
        exit(EXIT_FAILURE);
    }
	// Take log2(numElementsIn) to make the first array size a power of 2
	double numElements = pow((double)(2), (double)((log((double)numElementsIn)/log(2.0)))); 
	printf("%f\n", log(numElementsIn));
	printf("numElements: %f\n", numElements);
	
	printf("Please enter each of the %f ints individually followed by the enter key.\n", numElements);
    
	int *dynamicArray = (int *) malloc(sizeof(int) * numElements);
	
    for(int i = 0; i < numElements; i++){
        scanf("%d", &dynamicArray[i]);   
    }

    ArrayInfo arrayInfo;
    arrayInfo.head = dynamicArray;
    arrayInfo.currentSize = arrayInfo.maxSize = numElements;

    size_t bufferSize = 255; //255 is not significant here. Just allocating space for a buffer 

    //Need to use malloc here because we pass it to getline which could possible reallocate the memory
    //if we run out of space (won't happen anyway because I allocated too much to begin with)
    //this is different then scanf
    char *input = (char *) malloc(sizeof(char) * bufferSize); 

    printf("Thanks! ");
    displayInstructions();
    do{
        getline(&input, &bufferSize, stdin);
        //The first time through this loop getline is capturing the newline character from the previous input
        //This is a workaround for now - there are probably better ways to do this
        if(strcmp(input, "\n") == 0)
            continue;

        const char **splitInput = getSplitInput(input); 
        if(splitInput == NULL){
            fprintf(stderr, "Invalid input\n");
            continue;
        }

        if(strcmp(splitInput[0], "add") == 0){
            if(splitInput[1] != NULL){
                int newInt = atoi(splitInput[1]);
                addToArray(newInt, &arrayInfo);
            }
        }
        else if(strcmp(input, "p\n") == 0) //getline includes the newline character at the end of the string
            printArray(arrayInfo);
        else if(strcmp(input, "h\n") == 0)
            displayInstructions();

    }while(strcmp(input, "q\n") != 0 && strcmp(input, "Q\n") != 0); 

    free(arrayInfo.head);
    return 0;
}
