#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <raylib.h>

#define MAX_SIZE 100 //Buckets
#define LENGTH 45 //MAX NAME SIZE

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