#include "system.h"

static Person *Bank_Accounts[MAX_SIZE];

// Hash Function
static size_t hash(char *name , size_t length)
{
	size_t hashnumber = 1;
	for (size_t i = 0; i < length; i++)
	{
		hashnumber = (hashnumber * 31) + name[i];
	}

	return hashnumber % MAX_SIZE;
}

size_t hash_function(char *name , size_t length)
{
	return hash(name , length);
}


/*
Ensure all pointers in Bank_Accounts array are initialized to NULL
*/
void init()
{
	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		Bank_Accounts[i] = NULL;
	}
}


/*
Function thatCreates A New Account
Takes in Person Struct holding the information about the person
Returns True if successful 
else False 
*/
bool create_account(Person * new_account)
{
	if (new_account == NULL)
	{
		fprintf(stderr, "Failed to create account (NULL)\n");
		return false;
	}

	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		if(Bank_Accounts[i] != NULL)
		{
			if(Bank_Accounts[i]->account_number == new_account->account_number)
			{
				if(strcmp(Bank_Accounts[i]->name,new_account->name) != 0)
				{
					fprintf(stderr , "Cannot Create New Account (%s). Account Already exists with the same Account Number.\n" ,new_account->name);
					return false;
				}
			}
		}
	}

	size_t index = hash_function(new_account->name, (size_t) strlen(new_account->name));
	// printf("Hash index for %s is %zu.\n", new_account->name, index);

	if(Bank_Accounts[index] == NULL)
	{
		Bank_Accounts[index] = malloc(sizeof(Person));
		if (Bank_Accounts[index] == NULL)
		{
			return false;
		}
		memcpy(Bank_Accounts[index] , new_account , sizeof(Person));
		return true;
	}
	else if (strcmp(Bank_Accounts[index]->name,new_account->name) == 0)
	{
		fprintf(stderr , "Account Already exists with the same name. (%s) collided with (%s)\n",Bank_Accounts[index]->name, new_account->name );
		return false;
	}

	fprintf(stderr ,"Failed to create Account (%s) for Whatever reasons.\n", new_account->name);
	return false;
}


/*
Function Transfers Money From One Account to Another.
Takes in the names of the persons that are sending and 
receiving the money and the value they are receiving
return true if the transfer is successfull
else false.
*/
bool transfer_money(char * from , char * to , size_t value)
{
	size_t index_from = hash_function(from , (size_t) strlen(from));
	size_t index_to   = hash_function(to , (size_t) strlen(to));

	if(Bank_Accounts[index_from] == NULL || strcmp(from , Bank_Accounts[index_from]->name) != 0 )
	{
		fprintf(stderr, "(NULL) 1 from \n");
		return false;
	}
	
	if(Bank_Accounts[index_to] == NULL || strcmp(to , Bank_Accounts[index_to]->name) != 0 )
	{
		fprintf(stderr, "(NULL) 2 to \n");
		return false;
	}

	if(Bank_Accounts[index_from]->balance < value)
	{
		printf("Not Enough Funds in Account to Transfer %zu.\n", value);
		return false;
	}
	Bank_Accounts[index_from]->balance -= value;
	Bank_Accounts[index_to]->balance += value;

	return true;
}


/*
Function that checks the balance of a person and return the balance.
*/
size_t check_balance(char *name)
{
	size_t index = hash_function(name , (size_t) strlen(name));
	if(Bank_Accounts[index] == NULL)
	{
		fprintf(stderr , "Account Does Not Exist (NULL)\n");
		exit(EXIT_FAILURE);
	}
	else if (strcmp(name , Bank_Accounts[index]->name) == 0)
	{
		return Bank_Accounts[index]->balance;
	}

	fprintf(stderr , "Account Not Found\n");
	exit(EXIT_FAILURE);
}

void unload_Hash_table()
{
	for (size_t i = 0; i < MAX_SIZE; i++)
	{
		free(Bank_Accounts[i]);
	}
}


void Retrieve_Accounts(char *name)
{
	size_t index = hash(name , (size_t) strlen(name));
	if (Bank_Accounts[index] == NULL )
	{
		fprintf(stderr , "Account with the name(%s) does Not Exist (NULL)\n", name);
		return exit(EXIT_FAILURE);
	}

	printf(" Name: %s\n", Bank_Accounts[index]->name);
	printf(" Age: %zu\n", Bank_Accounts[index]->age);
	printf(" Account Number: %zu\n", Bank_Accounts[index]->account_number);
	printf(" Gender: %s\n", Bank_Accounts[index]->gender);
	printf(" Balance: $%.2f\n", (float) Bank_Accounts[index]->balance);
	printf("\n");
}


void Print_accounts()
{
	printf("\n");
	printf("Start\n");
	printf("Account(s) Registerd.\n");
	printf("\n");

	for (size_t i = 0 ; i < MAX_SIZE; i++)
	{
		if (Bank_Accounts[i] == NULL)
		{
			// Print Nothing
		}
		else
		{
			Retrieve_Accounts(Bank_Accounts[i]->name);
		}
	}

	printf("\n");
	printf("End\n");
	printf("\n");
}



// Function to get user data 
static void data_from_user()
{
	char name[LENGTH]; 
	char *gender = malloc(sizeof(char) * 6);
	if (gender == NULL) 
	{
		return exit(EXIT_FAILURE);
	}
	size_t balance;
	size_t account;
	size_t age;

	printf("Provide Name: ");
	scanf("%s" , name);

	printf("Provide Age: ");
	scanf("%zu" , &age);

	printf("Provide Balance: ");
	scanf("%zu" , &balance);

	printf("Provide account number: ");
	scanf("%zu" , &account);

	printf("Provide Gender: ");
	scanf("%s" , gender);

	Person new_account = {
		.account_number = account,
		.age = age,
		.balance = balance,
		.gender = gender,
		.name = name
	};

	printf("\n");
	printf("Creating Account.... \n");

	bool new_account_validity = create_account(&new_account);
	if (!new_account_validity)
	{	
		return exit(EXIT_FAILURE);
	}

	FILE *data = fopen("accounts.csv" , "a");
	fprintf(data , "%s, %zu , %zu , %zu , %s", name , age , balance , account , gender);

	printf("\n");
	printf("Account Created Successfully\n");

	free(gender);
	fclose(data);
}


bool login(char *name, char *password)
{
	size_t index = hash(name , (size_t) strlen(name));

	if (Bank_Accounts[index] != NULL)
	{
		if (strcmp(name, Bank_Accounts[index]->name) == 0)
		{
			if(strcmp(password, Bank_Accounts[index]->password) == 0)
			{
				printf("Successfully Logged In.\n");
				return true;
			}
			else
			{
				fprintf(stderr , "Wrong Password Provided. Try Again.\n");
				return false;
			}
		}
		else
		{
			printf("Create New Account.\n");
			data_from_user();
			return false;
		}
	}
	return false;
}


static void transfer_funds()
{
	char * from = malloc(sizeof(char) * (LENGTH + 1));
	if (from == NULL)
	{
		return exit(EXIT_FAILURE);
	}
	printf("From: ");
	scanf("%s", from);

	char * to = malloc(sizeof(char) * (LENGTH + 1));
	if (to == NULL)
	{
		return exit(EXIT_FAILURE);
	}
	printf("To: ");
	scanf("%s", to);

	size_t value;
	printf("amount to transfer: ");
	scanf("%zu", &value);

	bool transferred = transfer_money(from , to , value);
	if(!transferred)
	{
		return exit(EXIT_FAILURE);
	}

	printf("Funds Transfered to %s successfully", to);

	free(from);
	free(to);
}


static void check_account_balance()
{
	char *name= malloc(sizeof(char) * (LENGTH + 1));
	if (name == NULL)
	{
		return exit(EXIT_FAILURE);
	}
	printf("Enter Name: ");
	scanf("%s", name);

	size_t check = check_balance(name);
	printf("Your Account Balance is $%.2f ", (float) check);

	free(name);
}


static void user_information()
{
	char * name = malloc(sizeof(char) * (LENGTH + 1));
	if (name == NULL)
	{
		return exit(EXIT_FAILURE);
	}
	printf("Enter Your Name: ");
	scanf("%s", name);

	Retrieve_Accounts(name);
	free(name);
}


static void information()
{
	printf("\n");
	printf("Option A: Create Account.\n");
	printf("Option B: Transfer Funds.\n");
	printf("Option C: Check Balance.\n");
	printf("Option D: Account Details.\n");
	printf("DEFAULT IS ACCOUNT CREATION\n");
	printf("\n");
}


static void login_user()
{
	char * name = malloc(sizeof(char) * (LENGTH + 1));
	if (name == NULL)
	{
		return exit(EXIT_FAILURE);
	}
	printf("Name: ");
	scanf("%s", name);

	char * password = malloc(sizeof(char) * (LENGTH + 1));
	if (password == NULL)
	{
		return exit(EXIT_FAILURE);
	}
	printf("Password: ");
	scanf("%s", password);
	bool login_user_data = login(name , password);
	if (!login_user_data)
	{
		return exit(EXIT_FAILURE);
	}
}

void choose_activity()
{
	information();

	char Alpha;
	printf("Enter Option: ");
	scanf("%c", &Alpha);

	switch (Alpha)
	{
	case 'A':
		data_from_user();
		break;
	
	case 'B':
		transfer_funds();
		break;
	
	case 'C':
		check_account_balance();
		break;
	
	case 'D':
		user_information();
		break;

	default:
		login_user();
	}
}