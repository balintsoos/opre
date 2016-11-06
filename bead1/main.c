#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Events
struct Event
{
	int id;
	int guestCount;
	char name[100];
};

struct Event events[100];
int last_event_index = 0;
int next_event_id = 0;

void load_events()
{
	last_event_index = 0;

	FILE* file;
	file = fopen("events", "r");

	if (file)
	{
		while (!feof(file))
		{
			if (fread(&(events[last_event_index]), sizeof(struct Event), 1, file))
			{
				if (events[last_event_index].id >= next_event_id)
				{
					next_event_id = events[last_event_index].id + 1;
				}

				++last_event_index;
			}
		}

		fclose(file);
	}
}

void list_events()
{
	load_events();

	printf("\nID\tName\tNumber of guests\n");
	printf("------------------------------------\n");

	for (int i = 0; i < last_event_index; ++i)
	{
		printf("%d\t%s\t%d\n", events[i].id, events[i].name, events[i].guestCount);
	}
}

void create_event()
{
	load_events();

	struct Event new_event;
	char new_name[100];

	printf("\nEvent name: ");
	scanf(" %[^\n]%*c", new_name);

	new_event.id = next_event_id;
	new_event.guestCount = 0;
	strcpy(new_event.name, new_name);

	// append to events file
	FILE* file;
	file = fopen("events", "a");
	fwrite(&new_event, sizeof(struct Event), 1, file);
	fclose(file);
}

void delete_event()
{
	
}

// Guests
struct Guest
{
	int id;
	char name[100];
	char email[100];
	int event_id;
	int event_queue;
	time_t signup_time;
};

void list_guests()
{
	
}

void add_guest()
{
	
}

void edit_guest()
{
	
}

void remove_guest()
{
	
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
		
		switch (selected_option) {
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