/*
 * Khang Tran
 * Implementation of Beaufort Cipher Encrypt for encrypting messages
 * June 16, 2023
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
 * Encrypt function: Encrypts input string using cipher  algorithm 
 * Parameters:
 * - key: Pointer to the key string used for encryption
 * - input: Pointer to the input string to be encrypted
 * - index: Index used to track the current position in the key string
 * Return: The updated index value after encryption
 */
int encrypt(char * key, char * input, int index){
	char result[128] = "";
	char getKey;
	char getInput;
	int keyLength = strlen(key);
	for(int i = 0; input[i] != '\0'; i++){
		if(input[i]  == '\n'){
			continue;
		}
		if(input[i] == ' '){
			result[i] = ' ';
			continue;
		}
		getKey = key[index];
		getInput = input[i];
		result[i] = (((getKey - getInput) + 26) % 26) + 65;
		if(index < keyLength - 2){
			index++;}
		else{
			index = 0;}
	}
	printf("%s\n", result);
	return index;
}

int main(){
	int index = 0;
	char key[128];
	char input[128];
	fgets(key, sizeof(key), stdin);//Read the key from the user input
	if(strlen(key) == 1){
		return 0;}
	while(fgets(input, sizeof(input), stdin) != NULL){//Read input strings until an empty line is entered
		if(strlen(input) == 1){
			break;
		}
		index = encrypt(key, input, index);
	}
	return 0;
}
