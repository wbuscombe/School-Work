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
			File file = new File("movies.txt");
			Scanner scan = new Scanner(file);

			Connection c = null;
			Statement stmt = null;

			String movie, year, tag;
			String buffer = "";

			int indexOfTag, endOfTag;
			int indexOfYear;
			int indexOfMovie, endOfMovie;

			Class.forName("org.sqlite.JDBC");
			c = DriverManager.getConnection("jdbc:sqlite:movie_tags.db");
			c.setAutoCommit(false);

			System.out.println("Opened database successfully");

			stmt = c.createStatement();
			String sql = "CREATE TABLE MOVIE_TAGS "
					+ "(ENTRYID INTEGER PRIMARY KEY," + " MOVIE BLOB,"
					+ " TAG BLOB NOT NULL," + " YEAR BLOB NOT NULL)";
			stmt.execute(sql);

			while (scan.hasNext()) {
				movie = year = tag = "";
				indexOfTag = endOfTag = 0;
				indexOfYear = 0;
				indexOfMovie = endOfMovie = 0;

				buffer = scan.nextLine();

				// Pattern "......" For Tag
				Pattern pattern = Pattern.compile("\\\".*\\\"");
				Matcher matcher = pattern.matcher(buffer);

				// Find Tag
				while (matcher.find()) {
					indexOfTag = matcher.start();
					endOfTag = matcher.end();
				}

				tag = buffer.substring(indexOfTag, endOfTag);

				tag = tag.trim();

				// SQLite Will Not Handle The Character ' Well So We Must
				// Replace It With " If It Exists With A String
				for (int i = 0; i < tag.length(); ++i)
					if (tag.charAt(i) == '\'') {
						StringBuilder tagSB = new StringBuilder(tag);
						tagSB.setCharAt(i, '"');
						tag = tagSB.toString();
					}

				// Pattern Of (YYYY) Or (????)
				pattern = Pattern
						.compile("\\([0123456789?]{4}\\/{0,1}[IVXLCDM]*\\)");
				matcher = pattern.matcher(buffer);

				// Find (YYYY)
				while (matcher.find()) {
					indexOfYear = matcher.start();
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

				// Pattern {.......} for movie
				pattern = Pattern.compile("\\{.*\\}");
				matcher = pattern.matcher(buffer);

				// Find Movie
				while (matcher.find()) {
					indexOfMovie = matcher.start() + 1;
					endOfMovie = matcher.end() - 1;
				}

				// Check For Junk {..... (#...)}
				pattern = Pattern.compile("\\(\\#\\d*[.]{0,1}\\d*\\)");
				matcher = pattern.matcher(buffer);

				// If Junk Exists
				if (matcher.find()) {
					endOfMovie = matcher.start() - 1;
				}

				// Edge Case Of {(#...)}
				if (endOfMovie < indexOfMovie)
					indexOfMovie = endOfMovie;

				// Pull Movie Out Of Line
				movie = buffer.substring(indexOfMovie, endOfMovie);

				movie = movie.trim();

				// SQLite Will Not Handle The Character ' Well So We Must
				// Replace It With " If It Exists With A String
				for (int i = 0; i < movie.length(); ++i)
					if (movie.charAt(i) == '\'') {
						StringBuilder movieSB = new StringBuilder(movie);
						movieSB.setCharAt(i, '"');
						movie = movieSB.toString();
					}

				// Testing
				/*
				 * System.out.printf("Movie: %s\nYear: %s\nTag: %s\n\n",
				 * movie.trim(), year.trim(), tag.trim());
				 */

				// Testing
				System.out
						.printf("INSERT INTO MOVIE_TAGS (MOVIE, TAG, YEAR) VALUES('%s', '%s', '%s');\n",
								movie.trim(), tag.trim(), year.trim());

				sql = "INSERT INTO MOVIE_TAGS (MOVIE, TAG, YEAR) " + "VALUES('"
						+ movie.trim() + "', '" + tag.trim() + "', '"
						+ year.trim() + "');";
				stmt.execute(sql);
			}

			stmt.close();
			c.commit();
			c.close();
		} catch (Exception e) {
			System.err.println(e.getClass().getName() + ": " + e.getMessage());
			System.exit(0);
		}
	}
}
