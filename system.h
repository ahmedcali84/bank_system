#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


// Define escape codes for text formatting
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

#define MAX_SIZE 100 //Buckets
#define LENGTH 45 //MAX NAME SIZE
#define BUFFER_LENGTH 256
#ifndef STREAM
#define STREAM "accounts.csv"
#endif  // STREAM

struct Person 
{
	char *name;
	char *password;
	char * gender;
	size_t age;
	size_t account_number;
	size_t balance;
};

typedef struct Person Person;

void Load_Accounts(char *file_path);
void init();
size_t hash_function(char *name , size_t length);
bool create_account(Person *new_account);
bool transfer_money(char * from , char * to , size_t value);
size_t check_balance(char *name);
void unload_Hash_table();
void choose_activity();
void Retrieve_Accounts( char *name);
void Print_accounts();
bool login(char *name, char *password);

#endif //BANK_SYSTEM_H