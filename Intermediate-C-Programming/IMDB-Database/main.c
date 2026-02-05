/*
 * IMDB DATABASE QUERY TOOL
 *
 * A command-line tool for querying movie databases using SQLite.
 * Supports queries for: movie taglines, MPAA ratings, and actor lists.
 *
 * OPTIMIZATION NOTES (2026):
 * - CRITICAL: Fixed SQL injection vulnerabilities by using parameterized queries
 * - Fixed uninitialized buffer bugs in tag() and actors() functions
 * - Added buffer overflow protection in parse() function
 * - Added proper NULL checks throughout
 * - Original vulnerable code preserved in comments for educational purposes
 */

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

	/*
	 * ORIGINAL CODE (VULNERABLE TO BUFFER OVERFLOW):
	 * ----------------------------------------------
	 *     while(1)
	 *     {
	 *         c = getc(stdin);
	 *         if (c == '\n')
	 *         {
	 *             input[index] = '\0';
	 *             break;
	 *         }
	 *         else
	 *         {
	 *             input[index++] = c;
	 *         }
	 *     }
	 *
	 * SECURITY ISSUE:
	 * No bounds checking on index! If user enters more than MAX_CHARS,
	 * we write past the end of input[], corrupting stack memory.
	 * This could lead to crashes or exploitable buffer overflows.
	 *
	 * FIX: Check index < MAX_CHARS - 1 before writing
	 */

	// Read a line of input from user
	// Save first word as the command string and all others as arguments
	while(1)
	{
		c = getc(stdin);

		if (c == '\n' || c == EOF)
		{
			input[index] = '\0';
			break;
		}
		/* SECURITY FIX: Prevent buffer overflow */
		else if (index < MAX_CHARS - 1)
		{
			input[index++] = c;
		}
		/* Silently truncate input that exceeds buffer */
	}

	if (index > 0)
	{
		buffer = strtok(input, delims);

		/*
		 * OPTIMIZATION: Added NULL check for strtok result.
		 * If input is all whitespace, strtok returns NULL and
		 * strcpy(command, NULL) causes undefined behavior.
		 */
		if (buffer == NULL) {
			command[0] = '\0';
			return 0;
		}
		strcpy(command, buffer);

		/*
		 * OPTIMIZATION: Added bounds check on numArgs to prevent
		 * writing past the arguments array if user provides many arguments.
		 */
		while ((buffer = strtok(NULL, delims)) != NULL && numArgs < MAX_CHARS - 1)
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
	sqlite3_stmt *stmt;  /* NEW: Prepared statement for parameterized query */
	char movie[MAX_CHARS];
	int result;

	/*
	 * CRITICAL SECURITY FIX: SQL INJECTION VULNERABILITY
	 * ==================================================
	 *
	 * ORIGINAL VULNERABLE CODE:
	 * -------------------------
	 *     char sql[MAX_CHARS];
	 *     strcpy(sql, "select year, tag from movie_tags where movie='");
	 *     strcat(sql, movie);
	 *     strcat(sql, "';");
	 *     result = sqlite3_exec(db, sql, callback, (void*)data, &errMsg);
	 *
	 * SECURITY ISSUE:
	 * The movie name was directly concatenated into the SQL query string.
	 * A malicious user could enter something like:
	 *     tag '; DROP TABLE movie_tags; --
	 *
	 * This would create the SQL:
	 *     select year, tag from movie_tags where movie=''; DROP TABLE movie_tags; --'
	 *
	 * The attacker could:
	 * - Delete tables (DROP TABLE)
	 * - Modify data (UPDATE, INSERT, DELETE)
	 * - Extract sensitive data (UNION SELECT)
	 * - Read arbitrary files (depending on SQLite config)
	 *
	 * FIX: Use parameterized queries (prepared statements).
	 * The parameter value is never interpreted as SQL, only as data.
	 */

	// Open database
	result = sqlite3_open("movie_tags.db", &db);
	if (result)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	}

	/*
	 * BUG FIX: UNINITIALIZED BUFFER
	 * =============================
	 *
	 * ORIGINAL BUG:
	 *     char movie[MAX_CHARS];
	 *     strcat(movie, arguments[i]);  // strcat on uninitialized buffer!
	 *
	 * The movie buffer was never initialized before strcat() was called.
	 * strcat() appends to existing string content, so it searches for '\0'.
	 * With uninitialized memory, this could:
	 * - Read garbage data as the "existing" string
	 * - Read past buffer bounds looking for '\0' (buffer overread)
	 * - Produce corrupted movie names
	 *
	 * FIX: Initialize buffer to empty string before using strcat().
	 */
	movie[0] = '\0';  /* CRITICAL: Initialize buffer before strcat! */

	// Form entire movie title from arguments
	for (int i = 0; i < numArgs - 1; ++i)
	{
		strcat(movie, arguments[i]);
		strcat(movie, " ");
	}
	strcat(movie, arguments[numArgs - 1]);

	/* SECURE: Use parameterized query instead of string concatenation */
	const char *sql = "SELECT year, tag FROM movie_tags WHERE movie = ?;";

	result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (result != SQLITE_OK)
	{
		fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	/* Bind the movie name as a parameter - safely escaped automatically */
	sqlite3_bind_text(stmt, 1, movie, -1, SQLITE_STATIC);

	/* Execute and print results */
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		const char *year = (const char *)sqlite3_column_text(stmt, 0);
		const char *tagline = (const char *)sqlite3_column_text(stmt, 1);
		printf("%s: %s\n", year ? year : "N/A", tagline ? tagline : "N/A");
	}

	if (result != SQLITE_DONE)
	{
		fprintf(stderr, "SQL execution error: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

// Prints all movies with a given MPAA rating from a given year and reason
void movies()
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

	/*
	 * SECURITY FIX: SQL INJECTION
	 * ===========================
	 *
	 * ORIGINAL VULNERABLE CODE:
	 *     strcpy(sql, "select movie, rating, reason from mpaa_ratings where (rating='");
	 *     strcat(sql, arguments[0]);  // Rating directly concatenated!
	 *     strcat(sql, "' AND year='");
	 *     strcat(sql, arguments[1]);  // Year directly concatenated!
	 *     strcat(sql, "') order by reason;");
	 *
	 * Even though isRating() and isYear() validate the format, this pattern
	 * is still dangerous because:
	 * 1. Validation could be bypassed if code is modified
	 * 2. Sets a bad example/pattern for other developers
	 * 3. Defense in depth - parameterized queries are always safer
	 *
	 * FIX: Use parameterized queries for all user input.
	 */

	// Open database
	result = sqlite3_open("mpaa_ratings.db", &db);
	if (result)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	}

	/* SECURE: Parameterized query with placeholders */
	const char *sql = "SELECT movie, rating, reason FROM mpaa_ratings "
	                  "WHERE rating = ? AND year = ? ORDER BY reason;";

	result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (result != SQLITE_OK)
	{
		fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	/* Bind parameters safely */
	sqlite3_bind_text(stmt, 1, arguments[0], -1, SQLITE_STATIC);  /* rating */
	sqlite3_bind_text(stmt, 2, arguments[1], -1, SQLITE_STATIC);  /* year */

	/* Execute and print results */
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		const char *movie = (const char *)sqlite3_column_text(stmt, 0);
		const char *rating = (const char *)sqlite3_column_text(stmt, 1);
		const char *reason = (const char *)sqlite3_column_text(stmt, 2);
		printf("%s (%s) - %s\n",
		       movie ? movie : "N/A",
		       rating ? rating : "N/A",
		       reason ? reason : "N/A");
	}

	if (result != SQLITE_DONE)
	{
		fprintf(stderr, "SQL execution error: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

// Prints all actors in movies of a given name sorted by year then actor name
void actors(int numArgs)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char movie[MAX_CHARS];
	int result;

	/*
	 * SECURITY + BUG FIXES (same issues as tag() function):
	 * 1. SQL injection via string concatenation -> parameterized query
	 * 2. Uninitialized movie buffer -> initialize to empty string
	 *
	 * See tag() function comments for detailed explanation.
	 */

	// Open database
	result = sqlite3_open("movie_actors.db", &db);
	if (result)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	}

	/* BUG FIX: Initialize buffer before strcat */
	movie[0] = '\0';

	// Form entire movie title from arguments
	for (int i = 0; i < numArgs - 1; ++i)
	{
		strcat(movie, arguments[i]);
		strcat(movie, " ");
	}
	strcat(movie, arguments[numArgs - 1]);

	/* SECURE: Parameterized query */
	const char *sql = "SELECT year, actor FROM movie_actors "
	                  "WHERE movie = ? ORDER BY year, actor;";

	result = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (result != SQLITE_OK)
	{
		fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	/* Bind movie name parameter safely */
	sqlite3_bind_text(stmt, 1, movie, -1, SQLITE_STATIC);

	/* Execute and print results */
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		const char *year = (const char *)sqlite3_column_text(stmt, 0);
		const char *actor = (const char *)sqlite3_column_text(stmt, 1);
		printf("%s: %s\n", year ? year : "N/A", actor ? actor : "N/A");
	}

	if (result != SQLITE_DONE)
	{
		fprintf(stderr, "SQL execution error: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

/*
 * NOTE: The callback function below is NO LONGER USED after the security fixes.
 *
 * ORIGINAL PURPOSE:
 * The callback was used with sqlite3_exec() to process query results.
 * Each row would trigger this callback with the column values.
 *
 * WHY IT'S NO LONGER NEEDED:
 * The new implementation uses sqlite3_prepare_v2() + sqlite3_step() which
 * provides direct access to results without a callback. This approach:
 * 1. Allows parameterized queries (preventing SQL injection)
 * 2. Gives better control over result processing
 * 3. Makes error handling clearer
 *
 * Kept here for educational reference showing the old pattern.
 */
static int callback(void *data, int argc, char **argv, char **azColName)
{
   (void)data;       /* Suppress unused parameter warning */
   (void)argc;       /* Suppress unused parameter warning */
   (void)azColName;  /* Suppress unused parameter warning */
   (void)argv;       /* Suppress unused parameter warning */

   /* This function is no longer called - results are handled inline */
   /* See each query function for the new result processing code */

   /*
    * ORIGINAL CODE (for reference):
    *
    * // Tag command
    * if (strcmp(command, "tag") == 0)
    * {
    *     printf("%s: %s\n", argv[0], argv[1]);
    *     return 0;
    * }
    * // Movies command
    * if (strcmp(command, "movies") == 0)
    * {
    *     printf("%s (%s) - %s\n", argv[0], argv[1], argv[2]);
    *     return 0;
    * }
    * // Actors command
    * if (strcmp(command, "actors") == 0)
    * {
    *     printf("%s: %s\n", argv[0], argv[1]);
    *     return 0;
    * }
    */
   return 0;
}