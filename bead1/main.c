#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum { false, true } bool;

// Events
struct Event
{
	int id;
	int guestCount;
	char name[100];
};

struct Event events[100];

int next_event_index = 0;
int next_event_id = 0;

void read_events()
{
	next_event_index = 0;

	FILE* file;
	file = fopen("events", "r");

	if (file)
	{
		while (!feof(file))
		{
			if (fread(&(events[next_event_index]), sizeof(struct Event), 1, file))
			{
				if (events[next_event_index].id >= next_event_id)
				{
					next_event_id = events[next_event_index].id + 1;
				}

				++next_event_index;
			}
		}

		fclose(file);
	}
}

void write_events(int from, int to)
{
	FILE* file;
	file = fopen("events", "w");

	for (int i = from; i < to; ++i)
	{
		fwrite(&events[i], sizeof(struct Event), 1, file);
	}
	
	fclose(file);
}

void append_event(struct Event new_event)
{
	FILE* file;
	file = fopen("events", "a");

	fwrite(&new_event, sizeof(struct Event), 1, file);
	
	fclose(file);
}

void list_events()
{
	read_events();

	printf("\nID\tName\tNumber of guests\n");
	printf("------------------------------------\n");

	for (int i = 0; i < next_event_index; ++i)
	{
		printf("%d\t%s\t%d\n", events[i].id, events[i].name, events[i].guestCount);
	}
}

void create_event()
{
	read_events();

	struct Event new_event;
	char new_name[100];

	printf("\nEvent name: ");
	scanf(" %[^\n]%*c", new_name);

	new_event.id = next_event_id;
	new_event.guestCount = 0;
	strcpy(new_event.name, new_name);

	append_event(new_event);
}

void delete_event()
{
	read_events();

	int id;

	printf("\nDelete event by ID: ");
	scanf("%d", &id);

	int delete_position = -1;

	for (int i = 0; i < next_event_index; ++i)
	{
		if (id == events[i].id)
		{
			delete_position = i;
			break;
		}
	}

	if (delete_position == -1)
	{
		printf("\nEvent ID not found\n");
		return;
	}

	for (delete_position; delete_position < next_event_index; ++delete_position)
	{
		events[delete_position] = events[delete_position + 1];
	}

	write_events(0, next_event_index - 1);
}

// Guests
struct Guest
{
	int id;
	char name[100];
	char email[100];
	int event_id;
	int event_queue;
	time_t signup;
};

struct Guest guests[100];

int next_guest_index = 0;
int next_guest_id = 0;

void read_guests()
{
	next_guest_index = 0;

	FILE* file;
	file = fopen("guests", "r");

	if (file)
	{
		while (!feof(file))
		{
			if (fread(&(guests[next_guest_index]), sizeof(struct Guest), 1, file))
			{
				if (guests[next_guest_index].id >= next_guest_id)
				{
					next_guest_id = guests[next_guest_index].id + 1;
				}

				++next_guest_index;
			}
		}

		fclose(file);
	}
}

void write_guests(int from, int to)
{
	FILE* file;
	file = fopen("guests", "w");

	for (int i = from; i < to; ++i)
	{
		fwrite(&guests[i], sizeof(struct Guest), 1, file);
	}
	
	fclose(file);
}

void append_guest(struct Guest new_guest)
{
	FILE* file;
	file = fopen("guests", "a");

	fwrite(&new_guest, sizeof(struct Guest), 1, file);
	
	fclose(file);
}

void list_guests()
{
	read_guests();

	printf("\nID\tName\tE-mail\tEvent ID\tSign up\n");
	printf("----------------------------------------------------------------\n");

	for (int i = 0; i < next_guest_index; ++i)
	{
		printf("%d\t%s\t%s\t%d\t%s\n", guests[i].id, guests[i].name, guests[i].email, guests[i].event_id, ctime(&guests[i].signup));
	}
}

void add_guest()
{
	read_events();
	read_guests();

	struct Guest new_guest;

	new_guest.signup = time(NULL);
	new_guest.id = next_guest_id;

	char new_name[100];
	printf("\nName: ");
	scanf (" %[^\n]%*c", new_name);
	strcpy(new_guest.name, new_name);

	char new_email[100];
	printf("\nE-mail address: ");
	scanf (" %[^\n]%*c", new_email);
	strcpy(new_guest.email, new_email);

	int new_event_id;
	printf("\nEvent ID: ");
	scanf (" %d", &new_event_id);

	bool isValidEventId = false;

	for (int i = 0; i < next_event_index; ++i)
	{
		if (events[i].id == new_event_id)
		{
			isValidEventId = true;
			events[i].guestCount += 1;
			new_guest.event_queue = events[i].guestCount;
			break;
		}
	}

	if (!isValidEventId)
	{
		printf("\nEvent ID not found\n");
		return;
	}

	new_guest.event_id = new_event_id;

	write_events(0, next_event_index);
	append_guest(new_guest);

	printf("\nGuest placed in position %d on Event %d\n", new_guest.event_queue, new_guest.event_id);
}

void edit_guest()
{
	read_guests();

	int id;
	printf("\nEdit guest by ID: ");
	scanf("%d", &id);

	bool isValidGuestId = false;

	for (int i = 0; i < next_guest_index; ++i)
	{
		if (guests[i].id == id)
		{
			isValidGuestId = true;

			printf("\nNew name: ");
			scanf (" %[^\n]%*c", guests[i].name);

			printf("\nNew e-mail address: ");
			scanf (" %[^\n]%*c", guests[i].email);

			break;
		}
	}

	if (!isValidGuestId)
	{
		printf("\nGuest ID not found\n");
		return;
	}

	write_guests(0, next_guest_index);
}

void remove_guest()
{
	read_events();
	read_guests();

	int id;
	printf("\nRemove guest by ID: ");
	scanf("%d", &id);

	int delete_position = -1;

	for (int i = 0; i < next_guest_index; ++i)
	{
		if (guests[i].id == id)
		{
			delete_position = i;

			for (int j = 0; j < next_event_index; ++j)
			{
				if (events[j].id == guests[i].event_id)
				{
					events[j].guestCount -= 1;
					break;
				}
			}

			break;
		}
	}

	if (delete_position == -1)
	{
		printf("\nGuest ID not found\n");
		return;
	}

	for (delete_position; delete_position < next_guest_index; ++delete_position)
	{
		guests[delete_position] = guests[delete_position + 1];
	}

	write_events(0, next_event_index);
	write_guests(0, next_guest_index - 1);
}

void menu()
{
	int selected_option;
	
	do {
		printf("\n1 - List events\n");
		printf("2 - Create new event\n");
		printf("3 - Delete event\n");
		printf("4 - List guests\n");
		printf("5 - Sign up new guest\n");
		printf("6 - Edit guest\n");
		printf("7 - Remove guest\n");
		printf("0 - Exit\n");
		printf("\nChoose an option: ");
		
		scanf("%d", &selected_option);
		
		switch (selected_option)
		{
			case 1: list_events();
			break;
			case 2: create_event();
			break;
			case 3: delete_event();
			break;
			case 4: list_guests();
			break;
			case 5: add_guest();
			break;
			case 6: edit_guest();
			break;
			case 7: remove_guest();
			break;
		}
	} while (selected_option != 0);
}

void main(int argc, char** argv)
{
	menu();
}