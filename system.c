#include "system.h"

static Person *Bank_Accounts[MAX_SIZE];

void Load_Accounts(char *file_path)
{
    // Open file
    FILE *f = fopen(file_path, "r");
    if (f == NULL)
    {
        fprintf(stderr,BOLD RED"Failed to open file: %s\n" RESET, file_path);
        exit(EXIT_FAILURE);
    }

    char buffer[256]; // Buffer to hold each line of the CSV
    while (fgets(buffer, sizeof(buffer), f) != NULL)
    {
        // Allocate memory for a new Person
        Person *new_account = malloc(sizeof(Person));
        if (new_account == NULL)
        {
            fclose(f);
            exit(EXIT_FAILURE);
        }

        // Tokenize the CSV line
        char *name = strtok(buffer, ",");
        char *password = strtok(NULL, ",");
        char *age_str = strtok(NULL, ",");
        char *balance_str = strtok(NULL, ",");
        char *account_number_str = strtok(NULL, ",");
        char *gender = strtok(NULL, ",");

        if (name == NULL || password == NULL || age_str == NULL || balance_str == NULL || account_number_str == NULL || gender == NULL)
        {
            free(new_account);
            continue; // Skip malformed lines
        }

        // Fill the new_account structure
        new_account->name = strdup(name);
        new_account->password = strdup(password);
        new_account->age = strtoul(age_str, NULL, 10);
        new_account->balance = strtoul(balance_str, NULL, 10);
        new_account->account_number = strtoul(account_number_str, NULL, 10);
        new_account->gender = strdup(gender);

        // Create the account in the hashtable
        if (!create_account(new_account))
        {
            fprintf(stderr, "Failed to load account: %s\n", name);
            free(new_account->name);
            free(new_account->password);
            free(new_account->gender);
            free(new_account);
        }
    }

	Print_accounts();

    fclose(f);
}


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
	printf(" Gender: %s", Bank_Accounts[index]->gender);
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

	char *password = malloc(sizeof(char) * 6);
	if (password == NULL) 
	{
		return exit(EXIT_FAILURE);
	}

	size_t balance;
	size_t account;
	size_t age;

	printf("Provide Name: ");
	scanf("%s" , name);

	printf("Provide Password: ");
	scanf("%s" , password);

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
		.name = name,
		.password = password,
	};

	printf("\n");
	printf("Creating Account.... \n");

	bool new_account_validity = create_account(&new_account);
	if (!new_account_validity)
	{	
		return exit(EXIT_FAILURE);
	}

	FILE *data = fopen(STREAM , "a");
	fprintf(data , "%s,%s,%zu,%zu,%zu,%s\n", name , password, age , balance , account , gender);

	printf("\n");
	printf("Account Created Successfully\n");

	free(gender);
	free(password);
	fclose(data);
}


bool login(char *name, char *password)
{
	size_t index = hash(name , (size_t) strlen(name));

	if (Bank_Accounts[index] != NULL)
	{
		if (strcmp(name, Bank_Accounts[index]->name) == 0 && strcmp(password, Bank_Accounts[index]->password) == 0)
		{
			return true;
		}
		else
		{
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
	scanf(" %s", from);

	char * to = malloc(sizeof(char) * (LENGTH + 1));
	if (to == NULL)
	{
		return exit(EXIT_FAILURE);
	}
	printf("To: ");
	scanf(" %s", to);

	size_t value;
	printf("amount to transfer: ");
	scanf(" %zu", &value);

	bool transferred = transfer_money(from , to , value);
	if(!transferred)
	{
		return exit(EXIT_FAILURE);
	}

	printf("Funds Transfered to %s successfully\n", to);

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
	scanf(" %s", name);

	size_t check = check_balance(name);
	printf("Your Account Balance is $%.2f\n", (float) check);

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
	scanf(" %s", name);

	Retrieve_Accounts(name);
	free(name);
}


static void information()
{
	printf("\n");
	printf("Option A: LOGIN.\n");
	printf("Option B: CREATE ACCOUNT.\n");
	printf("\n");
}


static void after_information()
{
	printf("\n");
	printf("Option A: TRANSFER FUNDS.\n");
	printf("Option B: Account Balance.\n");
	printf("Option C: Account Details.\n");
	printf("\n");
}


static bool login_user()
{
	char * name = malloc(sizeof(char) * (LENGTH + 1));
	if (name == NULL)
	{
		return false;
	}

	printf("Name: ");
	scanf(" %s", name);

	char * password = malloc(sizeof(char) * (LENGTH + 1));
	if (password == NULL)
	{
		return false;
	}

	printf("Password: ");
	scanf(" %s", password);

	bool login_user_data = login(name , password);
	if (!login_user_data)
	{
		fprintf(stderr, BOLD RED "Failed to Login! Try Again.\n"RESET);
		return false;
	}

	fprintf(stderr, BOLD GREEN "Successfully Logged In.\n"RESET);
	return true;
}

void choose_activity()
{
	information();

	char Alpha;
	printf("Enter Option: ");
	scanf(" %c", &Alpha);

	// Login in to Your Account or Create Account
	switch (Alpha)
	{
		case 'A':
		{
			bool success = login_user();
			if (success)
			{
				after_information();

				char Beta;
				printf("Enter Option: ");
				scanf(" %c", &Beta);

				switch (Beta)
				{
					case 'A':
						transfer_funds();
						break;
					case 'B':
						check_account_balance();
						break;
					case 'C':
						user_information();
						break;
				}
			}
			break;
		}

		case 'B':
			data_from_user();
			break;
		
		default:
			break;
	}
}