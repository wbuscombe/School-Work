#include <ctype.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 256
#define delims " \t\r\n"

char command[MAX_CHARS];
char arguments[MAX_CHARS][MAX_CHARS];

// Read and parse input, return number of arguments
int parse();

// Clear command string and argument vector
void flush();

// Print command menu
void printMenu();

// Helper functions to check arguments for valid input
bool isRating(char arguments[MAX_CHARS]);
bool isYear(char arguments[MAX_CHARS]);

// Prints all the tag lines used for movies with a given name
void tag(int numArgs);

// Prints all movies with a given MPAA rating from a given year and reason
void movies();

// Prints all actors in movies of a given name sorted by year then actor name
void actors(int numArgs);

static int callback(void *data, int argc, char **argv, char **azColName);

int main()
{
	// Primary program loop
	while (1)
	{
		flush();

		// Prompt for command
		printf("Enter ? for command menu\n");
		printf("> ");

		// parse() will fill the command and argument strings with input
		// from the command line. It will also return the number of arugments
		// for further testing purposes, which will be stored in numArgs.
		int numArgs = parse(command, arguments);

		// Interpret and execute command
		//
		// ? menu command
		if ((strcmp(command, "?") == 0) && (numArgs == 0))
			printMenu();
		// tag movie_name command
		else if (strcmp(command, "tag") == 0 && (numArgs > 0))
		{
			// Do tag movie_name
			tag(numArgs);
		}
		// movies rating year command
		else if ((strcmp(command, "movies") == 0) && (numArgs ==  2) && isRating(arguments[0]) && isYear(arguments[1]))
		{
			// Do movies rating year
			movies();
		}
		// actors movie_name command
		else if (strcmp(command, "actors") == 0 && (numArgs > 0))
		{
			// Do actors movie_name
			actors(numArgs);
		}
		// exit command
		else if ((strcmp(command, "exit") == 0) && (numArgs == 0))
			break;
		else
			printf("Invalid Input\n");

		printf("\n");
	}

	return 0;
}

// Read and parse input, return number of arguments
int parse()
{
	char c = 0, *buffer, input[MAX_CHARS];
	int index = 0, numArgs = 0;

	// Read a line of input from user
	// Save first word as the command string and all others
	//  as arguments
	while(1)
	{
		c = getc(stdin);

		if (c == '\n')
		{
			input[index] = '\0';
			break;
		}
		else
		{
			input[index++] = c;
		}
	}

	if (index > 0)
	{
		buffer = strtok(input, delims);
		strcpy(command, buffer);

		while ((buffer = strtok(NULL, delims)) != NULL)
		{
			strcpy(arguments[numArgs++], buffer);
		}
	}

	return numArgs;
}

// Clear command string and argument vector
void flush()
{
	command[0] = '\0';
	
	for (int i = 0; i < MAX_CHARS; ++i)
			arguments[i][0] = '\0';
}

// Print command menu
void printMenu()
{
	printf("--Commands\n");
	printf("\n");
	printf("\ttag movie_name: prints all of tag lines used for all of the movies with the given name. Tag lines are printed one per line and are prefixed with the year of the movie.\n");
	printf("\n");
	printf("\tmovies rating year: prints all of the movies with the given MPAA rating in the given year along with the reasons for the rating in alphabetic order.\n");
	printf("\n");
	printf("\tactors movie_name: prints all of the actors in all of the movies with the given name. Output is sorted by year then actor name.\n");
	printf("\n");
	printf("exit - quit and exit the program.\n");
	printf("\n");
}

// Argument validation helper functions
//
// Check is variable rating an appropriate MPAA rating
bool isRating(char arguments[MAX_CHARS])
{
	// If rating is G, PG, PG-13, R or NC-17
	if (strcmp(arguments, "G") == 0)
		return true;
	if (strcmp(arguments, "PG") == 0)
		return true;
	if (strcmp(arguments, "PG-13") == 0)
		return true;
	if (strcmp(arguments, "R") == 0)
		return true;
	if (strcmp(arguments, "NC-17") == 0)
		return true;

	return false;
}
// Check is variable an appropriate year
bool isYear(char arguments[MAX_CHARS])
{
	// If 4 characters long
	if (strlen(arguments) != 4)
		return false;

	// If every character is an integer
	for (int i = 0; i < strlen(arguments); ++i)
		if (!(isdigit(arguments[i])))
			return false;

	return true;
}

// Prints all the tag lines used for movies with a given name
void tag(int numArgs)
{
	sqlite3 *db;
	char *errMsg = 0;
	char movie[MAX_CHARS];
	char sql[MAX_CHARS];
   	const char* data = "Callback function called";
	int result;

	// Open database
	result = sqlite3_open("movie_tags.db", &db);
	if (result)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	}

	// Form entire movie title
	for (int i = 0; i < numArgs - 1; ++i)
	{
		strcat(movie, arguments[i]);
		strcat(movie, " ");
	}
	strcat(movie, arguments[numArgs - 1]);

	// Form SQLite statement
	strcpy(sql, "select year, tag from movie_tags where movie='");
	strcat(sql, movie);
	strcat(sql, "';");

	// Execute statement
	result = sqlite3_exec(db, sql, callback, (void*)data, &errMsg);
	if( result != SQLITE_OK )
	{
      	fprintf(stderr, "SQL error: %s\n", errMsg);
      	sqlite3_free(errMsg);
   }
   sqlite3_close(db);
}

// Prints all movies with a given MPAA rating from a given year and reason
void movies()
{
	sqlite3 *db;
	char *errMsg = 0;
	char sql[MAX_CHARS];
   	const char* data = "Callback function called";
	int result;

	// Open database
	result = sqlite3_open("mpaa_ratings.db", &db);
	if (result)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	}

	// Form SQLite statement
	strcpy(sql, "select movie, rating, reason from mpaa_ratings where (rating='");
	strcat(sql, arguments[0]);
	strcat(sql, "' AND year='");
	strcat(sql, arguments[1]);
	strcat(sql, "') order by reason;");

	// Execute statement
	result = sqlite3_exec(db, sql, callback, (void*)data, &errMsg);
	if( result != SQLITE_OK )
	{
      	fprintf(stderr, "SQL error: %s\n", errMsg);
      	sqlite3_free(errMsg);
   }
   sqlite3_close(db);
}

// Prints all actors in movies of a given name sorted by year then actor name
void actors(int numArgs)
{
	sqlite3 *db;
	char *errMsg = 0;
	char movie[MAX_CHARS];
	char sql[MAX_CHARS];
   	const char* data = "Callback function called";
	int result;

	// Open database
	result = sqlite3_open("movie_actors.db", &db);
	if (result)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	}

	// Form entire movie title
	for (int i = 0; i < numArgs - 1; ++i)
	{
		strcat(movie, arguments[i]);
		strcat(movie, " ");
	}
	strcat(movie, arguments[numArgs - 1]);

	// Form SQLite statement
	strcpy(sql, "select year, actor from movie_actors where movie='");
	strcat(sql, movie);
	strcat(sql, "' order by year, actor;");

	// Execute statement
	result = sqlite3_exec(db, sql, callback, (void*)data, &errMsg);
	if( result != SQLITE_OK )
	{
      	fprintf(stderr, "SQL error: %s\n", errMsg);
      	sqlite3_free(errMsg);
   }
   sqlite3_close(db);
}

static int callback(void *data, int argc, char **argv, char **azColName)
{
   // Tag command
   if (strcmp(command, "tag") == 0)
   {
   		printf("%s: %s\n", argv[0], argv[1]);
   		return 0;
   }
   // Movies command
   if (strcmp(command, "movies") == 0)
   {
   		printf("%s (%s) - %s\n", argv[0], argv[1], argv[2]);
   		return 0;
   }
   // Actors command
   if (strcmp(command, "actors") == 0)
   {
   		printf("%s: %s\n", argv[0], argv[1]);
   		return 0;
   }
   return 0;
}