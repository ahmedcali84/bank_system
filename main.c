#include "system.h"

int main(void)
{
	init();

	Person ahmed = {
		.account_number = 2234567,
		.age = 20,
		.balance = 2346347,
		.password = "ahmed123",
		.name = "ahmed",
		.gender = "M",
	};

	bool created = create_account(&ahmed);
	if(!created)
	{
		return 1;
	}

	Person shay = {
		.account_number = 234567,
		.age = 22,
		.balance = 2343747,
		.password = "shay123",
		.name = "shay",
		.gender = "F",
	};

	bool created2 = create_account(&shay);
	if(!created2)
	{
		return 1;
	}	

	choose_activity();

	Print_accounts();

	unload_Hash_table();

	return 0;
}