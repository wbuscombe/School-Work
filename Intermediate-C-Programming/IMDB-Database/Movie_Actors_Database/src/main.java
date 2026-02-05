import java.io.File;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class main {
	public static void main(String[] args) {
		try {
			File file = new File("actors.txt");
			// Scanner scan = new Scanner(file, "UTF-8");
			Scanner scan = new Scanner(file);

			Connection c = null;
			Statement stmt = null;

			String actor, movie, year;
			String actor_fName, actor_lName;
			String buffer = "", actorBuffer;

			int endOfActor;
			int endOfLName, indexOfFName, endOfFName;
			int indexOfMovie;
			int indexOfYear;

			Class.forName("org.sqlite.JDBC");
			c = DriverManager.getConnection("jdbc:sqlite:movie_actors.db");
			c.setAutoCommit(false);

			System.out.println("Opened database successfully");

			stmt = c.createStatement();
			String sql = "CREATE TABLE MOVIE_ACTORS "
					+ "(ENTRYID INTEGER PRIMARY KEY," + " MOVIE BLOB NOT NULL,"
					+ " ACTOR BLOB NOT NULL," + " YEAR BLOB NOT NULL)";
			stmt.execute(sql);

			while (scan.hasNext()) {
				actor = movie = year = actor_fName = actor_lName = "";
				actorBuffer = "";
				endOfActor = 0;
				endOfLName = indexOfFName = endOfFName = 0;
				indexOfMovie = 0;
				indexOfYear = 0;

				// First Line For Actor
				buffer = scan.nextLine();

				// Pattern {word} \t movie
				// Find End Of Actor Name And Beginning Of Movie
				Pattern pattern = Pattern.compile("\\w{0,}\\t{1,4}");
				Matcher matcher = pattern.matcher(buffer);

				// Find End Of Actor And Beginning Of Movie
				while (matcher.find()) {
					endOfActor = matcher.end() - 1;
					indexOfMovie = matcher.end();
				}

				actorBuffer = buffer.substring(0, endOfActor);

				// If Given lName, fName
				if (actorBuffer.contains(",")) {
					// Pattern for lName,
					pattern = Pattern.compile("[\\S ]*,");
					matcher = pattern.matcher(actorBuffer);

					// Find End Of lName
					while (matcher.find()) {
						endOfLName = matcher.end() - 1;
					}

					// Pattern For fName
					pattern = Pattern.compile(", [\\S ]*");
					matcher = pattern.matcher(actorBuffer);

					// Find Indices For fName
					while (matcher.find()) {
						indexOfFName = matcher.start() + 2;
						endOfFName = matcher.end();
					}

					actor_fName = actorBuffer.substring(indexOfFName,
							endOfFName);
					actor_lName = actorBuffer.substring(0, endOfLName);

					actor_fName = actor_fName.trim();
					actor_lName = actor_lName.trim();

					actor = actor_fName + " " + actor_lName;

					// SQLite Will Not Handle The Character ' Well So We Must
					// Replace It With " If It Exists With A String
					for (int i = 0; i < actor.length(); ++i)
						if (actor.charAt(i) == '\'') {
							StringBuilder actorSB = new StringBuilder(actor);
							actorSB.setCharAt(i, '"');
							actor = actorSB.toString();
						}
				}
				// If Only Given One Name
				else {
					// Pattern For Name
					pattern = Pattern.compile("[\\S ]*");
					matcher = pattern.matcher(actorBuffer);

					while (matcher.find()) {
						endOfActor = matcher.end();
					}

					actor = actorBuffer.substring(0, endOfActor);

					actor = actor.trim();

					// SQLite Will Not Handle The Character ' Well So We Must
					// Replace It With " If It Exists With A String
					for (int i = 0; i < actor.length(); ++i)
						if (actor.charAt(i) == '\'') {
							StringBuilder actorSB = new StringBuilder(actor);
							actorSB.setCharAt(i, '"');
							actor = actorSB.toString();
						}
				}

				// Pattern Of (YYYY) Or (????)
				pattern = Pattern
						.compile("\\([0123456789?]{4}\\/{0,1}[IVXLCDM]*\\)");
				matcher = pattern.matcher(buffer);

				// Find (YYYY)
				while (matcher.find()) {
					indexOfYear = matcher.start();
				}

				// Pull Movie Out Of Line
				movie = buffer.substring(actorBuffer.length(), indexOfYear - 1);

				movie = movie.trim();

				// SQLite Will Not Handle The Character ' Well So We Must
				// Replace It With " If It Exists With A String
				for (int i = 0; i < movie.length(); ++i)
					if (movie.charAt(i) == '\'') {
						StringBuilder movieSB = new StringBuilder(movie);
						movieSB.setCharAt(i, '"');
						movie = movieSB.toString();
					}

				// Pull Year Out Of Line
				year = buffer.substring(indexOfYear + 1, indexOfYear + 5);

				year = year.trim();

				// SQLite Will Not Handle The Character ' Well So We Must
				// Replace It With " If It Exists With A String
				for (int i = 0; i < year.length(); ++i)
					if (year.charAt(i) == '\'') {
						StringBuilder yearSB = new StringBuilder(year);
						yearSB.setCharAt(i, '"');
						year = yearSB.toString();
					}

				// Testing
				/*
				 * System.out.printf( "Movie: %s\nYear: %s\nActor: %s\n\n",
				 * movie.trim(), year.trim(), actor.trim());
				 */

				// Testing
				System.out
						.printf("INSERT INTO MOVIE_ACTORS (MOVIE, ACTOR, YEAR) VALUES('%s', '%s', '%s');\n",
								movie.trim(), actor.trim(), year.trim());

				sql = "INSERT INTO MOVIE_ACTORS (MOVIE, ACTOR, YEAR) "
						+ "VALUES('" + movie.trim() + "', '" + actor.trim()
						+ "', '" + year.trim() + "');";
				stmt.execute(sql);

				// While Actor Has More Movies
				while (scan.hasNext()
						&& (!((buffer = scan.nextLine()).isEmpty()))) {
					// Pattern Of (YYYY) Or (????)
					pattern = Pattern
							.compile("\\([0123456789?]{4}\\/{0,1}[IVXLCDM]*\\)");
					matcher = pattern.matcher(buffer);

					// Find (YYYY)
					while (matcher.find()) {
						indexOfYear = matcher.start();
					}

					// Pull Movie Out Of Line
					movie = buffer.substring(0, indexOfYear - 1);

					movie = movie.trim();

					// SQLite Will Not Handle The Character ' Well So We Must
					// Replace It With " If It Exists With A String
					for (int i = 0; i < movie.length(); ++i)
						if (movie.charAt(i) == '\'') {
							StringBuilder movieSB = new StringBuilder(movie);
							movieSB.setCharAt(i, '"');
							movie = movieSB.toString();
						}

					// Pull Year Out Of Line
					year = buffer.substring(indexOfYear + 1, indexOfYear + 5);

					year = year.trim();

					// SQLite Will Not Handle The Character ' Well So We Must
					// Replace It With " If It Exists With A String
					for (int i = 0; i < year.length(); ++i)
						if (year.charAt(i) == '\'') {
							StringBuilder yearSB = new StringBuilder(year);
							yearSB.setCharAt(i, '"');
							year = yearSB.toString();
						}

					// Testing
					/*
					 * System.out.printf( "Movie: %s\nYear: %s\nActor: %s\n\n",
					 * movie.trim(), year.trim(), actor.trim());
					 */

					// Testing
					System.out
							.printf("INSERT INTO MOVIE_ACTORS (MOVIE, ACTOR, YEAR) VALUES('%s', '%s', '%s');\n",
									movie.trim(), actor.trim(), year.trim());

					sql = "INSERT INTO MOVIE_ACTORS (MOVIE, ACTOR, YEAR) "
							+ "VALUES('" + movie.trim() + "', '" + actor.trim()
							+ "', '" + year.trim() + "');";
					stmt.execute(sql);
				}
			}
			stmt.close();
			c.commit();
			c.close();
		} catch (Exception e) {
			System.err.println(e.getClass().getName() + ": " + e.getMessage());
			System.exit(0);
		}
		System.out.println("Records created successfully");
	}
}
