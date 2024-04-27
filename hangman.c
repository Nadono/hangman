#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "hangman.h"


int get_word(char secret[]){
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if( fp == NULL ){
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do{
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if( result != EOF )
            break;
    }while(1);

    fclose(fp);

    return 0;
}

int is_word_guessed(const char secret[], const char letters_guessed[]){
    int findFlag;
    for (int i=0;i<strlen(secret);i++){
        findFlag = 0;
        for(int j=0; j < strlen(letters_guessed);j++){
            if (secret[i] == letters_guessed[j]){
                findFlag = 1;
                break;
            }
        }
        if (findFlag == 0){
            return 0;

        }
    }
    return 1;
}

void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]) {
    for (int i = 0; i < strlen(secret); i++) {
        guessed_word[i] = '_'; 
        for (int j = 0; j < strlen(letters_guessed); j++) {
            if (letters_guessed[j] == secret[i]) {
                guessed_word[i] = secret[i]; 
                break;
            }
        }
    }
    guessed_word[strlen(secret)] = '\0';
   
}
void get_available_letters(const char letters_guessed[], char available_letters[]){
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int index = 0;
    for (int i = 0; alphabet[i] != '\0'; i++) {
        available_letters[index++] = alphabet[i];
    }
    available_letters[index] = '\0';
    int guessed_len = 0;
    while (letters_guessed[guessed_len] != '\0') {
        int alphabet_len = 0;
        while (available_letters[alphabet_len] != '\0') {
            if (letters_guessed[guessed_len] == available_letters[alphabet_len]) {
                
                for (int k = alphabet_len; available_letters[k] != '\0'; k++) {
                    available_letters[k] = available_letters[k + 1];
                }
                break;
            }
            alphabet_len++;
        }
        guessed_len++;
    }
}

void hangman(const char secret[]) 
{
    int secretLen = strlen(secret);
    int attempts = 8;
    char available_letters[40];
    char letters_guessed[40];
    int letters_guessed_index = 0;
    char guessed_word[40];
 
 
    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %d letters long.\n", secretLen);
    printf("-------------\n");
    
    while (attempts > 0) {
        printf("You have %d guesses left.\n", attempts);
        get_available_letters(letters_guessed, available_letters);
        printf("Available letters: %s\n", available_letters);
        printf("Please guess a letter:\n ");
        char tmpInput[20];
        fgets(tmpInput, 20, stdin);
        if (strlen(tmpInput) - 1 == 1) {
           
            if (tmpInput[0] >= 97 && tmpInput[0] <= 122) { 
                letters_guessed[letters_guessed_index] = tmpInput[0];
                letters_guessed_index++;
            } else if (tmpInput[0] >= 65 && tmpInput[0] <= 90) { 
                tmpInput[0] += 32;
                letters_guessed[letters_guessed_index] = tmpInput[0];
                letters_guessed_index++;
            } else { 
                printf("Oops! '%c' is not a valid letter.\n: ", tmpInput[0]);
            }
            get_guessed_word(secret, letters_guessed, guessed_word);
            printf("%s\n", guessed_word);
            if (is_word_guessed(secret, guessed_word)) {
            printf("Congratulations! You guessed the word.\n");
            break;
            }
            
            
        } 
        
 
 
        printf("-------------\n");
    }
    if (attempts == 0){
        printf("Better luck next time.\n The word was: %s\n", secret);
        printf("-------------\n");
    }
    
}