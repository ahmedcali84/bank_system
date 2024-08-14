#include "system.h"

int main(void)
{
	Load_Accounts(STREAM);

	choose_activity();

	unload_Hash_table();

	return 0;
}