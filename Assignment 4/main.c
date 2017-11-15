//1. A C source file with just the main function.

//included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//included headers
#include "other.h"
#include "BSTheaders.h"
#include "hashHeaders.h"

//Constants
#define LINESIZE 100

//main method
int main( int argc, char *argv[] )  {
	//variables
	int hashTableSize = 0; //an integer to hold the hash table size supplied
	int address = 0; //an integer to help figure out the address of each variable
	FILE * instructionSetFile; //A FILE type for the instructionSet input file
	FILE * programFile; //A FILE type for the program input file
	char line [LINESIZE]; //A char pointer that will store each line in the instructionSet input file
	char tempInstruction [6]; //The temp five character long instruction
	unsigned char tempOpcode; //The temp unsigned char to hold the opcode integer 
	unsigned char tempFormat; //The temp unsigned char to hold the format integer
	char * token; //A char pointer used when tokenizing a line in the instructionSetFile
	instructionSet * head = NULL; //A instructionSet pointer to the head node of the used BST
	instructionSet * tempNode; //A instructionSet pointer to the head node of the temp BST node
	char hashInstruction[6]; //The buffer for the Instruction to be used in the searchFunctionBST
	char symbol[11]; //the symbol buffer to be inserted into the hash table.
	hashNode * tempHashNode; //The node we assign the symbol and address to to add to the hash table.
	
	//Check number of arguments
	if(argc < 4){
		fprintf(stderr, "There were only %d arguments supplied. Expected 4.\nExiting...\n", argc);
		exit(-1);
	}
	else if(argc > 4){
		fprintf(stderr, "There were %d arguments supplied. Expected 4.\nExiting...\n", argc);
		exit(-1);
	}
	else{
		//Print out supplied arguments
		printf("\nfirstpass has been called.\n");
		printf("The supplied instructionset filename: \"%s\".\n", argv[1]);
		printf("The supplied programfile filename: \"%s\".\n", argv[2]);
		printf("The supplied hashtablesize: %s.\n", argv[3]);
	}
	
	//Check if the supplied hashtablesize is not a number
	if(atoi(argv[3]) == 0){
		fprintf(stderr, "\nThe supplied hashtablesize is not a number.\nExiting...\n");
		exit(-1);
	}

	//Assign int to converted command line arg.
	hashTableSize = atoi(argv[3]);
	hashNode * hashTable[hashTableSize];
	
	int i = 0;
	for (i = 0; i < hashTableSize; i++) {
		hashTable[i] = NULL;
	}
	
	//Check if the instructionset file doesn't exists or is unreadable
	if(fileExistsAndReadable(argv[1])==0){
		fprintf(stderr, "\nThe supplied instructionset filename, \"%s\", cannot be opened or cannot be read.\nExiting...\n", argv[1]);
		exit(-1);
	}

	//Check if the programfile file doesn't exists or is unreadable
	if(fileExistsAndReadable(argv[2])==0){
		fprintf(stderr, "\nThe supplied programfile filename, \"%s\", cannot be opened or cannot be read.\nExiting...\n", argv[2]);
		exit(-1);
	}

	//Check if the supplied hashtablesize is not atleast 2
	if(hashTableSize < 2){
		fprintf(stderr, "\nThe supplied hashtablesize is not atleast 2.\nExiting...\n");
		exit(-1);
	}

	//Open and check the instructionset filename
	instructionSetFile = fopen(argv[1], "r");
	if(instructionSetFile == NULL){
        	fprintf(stderr, "Could not allocate memory for instructionSetFile.\nExiting...\n");
        	exit(1);
	}

	printf("\n");
	//Parse line by line, inserting into BST
 	while (fgets(line, LINESIZE, instructionSetFile)) {
		tempNode = NULL; //Set the tempNode to NULL at every loop

		//Parse (and print) the lines of the instructionSetFile
		//printf("%s", line);
		token = strdup(line);

		//The instruction is the line up to the first space
		token = strtok(token, " ");
		strcpy(tempInstruction, token);
		//printf("%s ", tempInstruction);

		//The opcode is the rest of the line up to the second space
		token = strtok(NULL, " ");
		tempOpcode = atoi(token);
		//printf("%d ", tempOpcode);

		//The format is the rest of the line up to the newline char
		token = strtok(NULL, "\n");
		tempFormat = atoi(token);
		//printf("%d\n", tempFormat);

		//create tempNode
		tempNode = newInstructionSet(tempInstruction, tempOpcode, tempFormat);

		//print attributes of tempNode
		printf("%s ", tempNode->instruction);
		printf("%d ", tempNode->opcode);
		printf("%d\n", tempNode->format);
		
		//Insert tempNode into head
		insertBST(&head, tempNode);
		
		
	}

	//Print the BST (in-order)
	printf("\n");
	printInOrder(head);
	printf("\b\b \n");
	
	//Print the max height
	printf("Max Height : \"%d\"\n", maxHeight(head));
	
	//Open program file
	programFile = fopen(argv[2], "r");
	if(programFile == NULL){
    	fprintf(stderr, "Could not allocate memory for programFile.\nExiting...\n");
    	exit(1);
	}
	
	printf("\n");

	hashInstruction[0] = '\0';
	
	//Start parsing
	while(fgets(line, LINESIZE, programFile)) {
		
		//deletes the \n character from the line
		line[strlen(line) - 2] = '\0';
		
		//set token equal to line, so that we can modify it
		token = strdup(line);
		printf("ADDRESS #%d : %s\n", address, token);
		
		//first split of the line
		token = strtok(token, " ");
		
		//If the first character of the first token is equal to tab, then
		//the token MUST be the symbol.
		if (token[0] != '\t') {
			strcpy(symbol, token);
		}
		
		//counter to help determine where the instruction is.
		int counter = 1;
		while(token) {
			//if the token is equal to a tab, increment the pointer up to eliminate it, and this
			//tells us the instruction is in this token
			//else, if the counter is equal to two then we know the hash instruction is in this token.
			if (token[0] == '\t') {
				//printf("\nThere's a tab\n");
				token++;
				strcpy(hashInstruction, token);
			} else if ((counter % 2 == 0) && hashInstruction[0] == '\0') {
				strcpy(hashInstruction, token);
			}
			
			//printf("\"%s\" ", token);
			counter++;
			//next token
			token = strtok(NULL, " ");
		}
		
		//if the symbol is NOT empty, then we can insert it and the address into the hashtable.
		if (symbol[0] != '\0') {
			//create new node here
			tempHashNode = newHashNode(symbol, address);
			printf("Symbol : \"%s\", Address : %d\n", tempHashNode->symbol, tempHashNode->address); fflush(stdout);
			//insert into the hash function here
			int hashIndex = hashFunction(tempHashNode->symbol, hashTableSize);
			printf("Has index = %d\n", hashIndex);
			insertHash(&hashTable[hashIndex], tempHashNode);
		}
		
		if ((int) searchFormatBST(head, hashInstruction) == 0) {
			address++;
		} else {
			address += (int) searchFormatBST(head, hashInstruction);
		}
		hashInstruction[0] = '\0';
		symbol[0] = '\0';
		printf("\n\n");
		
	}
	
	//Close opened files and print done
	fclose(instructionSetFile);
	printf("\nDone.\n");
}
