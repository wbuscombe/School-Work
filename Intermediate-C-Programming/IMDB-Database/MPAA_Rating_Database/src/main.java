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
			File file = new File("mpaa.txt");
			Scanner scan = new Scanner(file);

			Connection c = null;
			Statement stmt = null;

			String movie, year, rating, reason;
			String buffer = "";

			int indexOfYear, indexOfReason;

			Class.forName("org.sqlite.JDBC");
			c = DriverManager.getConnection("jdbc:sqlite:mpaa_ratings.db");
			c.setAutoCommit(false);

			System.out.println("Opened database successfully");

			stmt = c.createStatement();
			String sql = "CREATE TABLE MPAA_RATINGS "
					+ "(ENTRYID INTEGER PRIMARY KEY," + " MOVIE BLOB NOT NULL,"
					+ " RATING BLOB NOT NULL," + " REASON BLOB NOT NULL,"
					+ " YEAR BLOB NOT NULL)";
			stmt.execute(sql);

			if (scan.hasNext())
				buffer = scan.nextLine();

			while (scan.hasNext()) {
				movie = year = rating = reason = "";
				indexOfYear = indexOfReason = 0;

				// Pattern Of (YYYY) Or (????)
				Pattern pattern = Pattern
						.compile("\\([0123456789?]{4}\\/{0,1}[IVXLCDM]*\\)");
				Matcher matcher = pattern.matcher(buffer);

				// Find (YYYY)
				while (matcher.find()) {
					indexOfYear = matcher.start();
				}

				// Pull Movie Title Out Of Line
				movie = buffer.substring(4, indexOfYear);

				movie = movie.trim();

				// SQLite Will Not Handle The Character ' Well So We Must
				// Replace It With " If It Exists With A String
				for (int i = 0; i < movie.length(); ++i)
					if (movie.charAt(i) == '\'') {
						StringBuilder movieSB = new StringBuilder(movie);
						movieSB.setCharAt(i, '"');
						movie = movieSB.toString();
					}

				// Pull Movie Year Out Of Line
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

				// Find Rating
				buffer = scan.nextLine();

				pattern = Pattern.compile("\\bG \\b");
				matcher = pattern.matcher(buffer);

				if (matcher.find()) {
					rating = "G";
					indexOfReason = matcher.end() + 4;
				}

				pattern = Pattern.compile("\\bPG \\b");
				matcher = pattern.matcher(buffer);

				if (matcher.find()) {
					rating = "PG";
					indexOfReason = matcher.end() + 4;
				}

				pattern = Pattern.compile("\\bPG-13 \\b");
				matcher = pattern.matcher(buffer);

				if (matcher.find()) {
					rating = "PG-13";
					indexOfReason = matcher.end() + 4;
				}

				pattern = Pattern.compile("\\bR \\b");
				matcher = pattern.matcher(buffer);

				if (matcher.find()) {
					rating = "R";
					indexOfReason = matcher.end() + 4;
				}

				pattern = Pattern.compile("\\bNC-17 \\b");
				matcher = pattern.matcher(buffer);

				if (matcher.find()) {
					rating = "NC-17";
					indexOfReason = matcher.end() + 4;
				}

				// Pull Reason Out Of Line
				reason = buffer.substring(indexOfReason);

				while (scan.hasNextLine()
						&& (buffer = scan.nextLine()).contains("RE: ")) {
					reason = reason.concat(buffer.substring(4));
				}

				reason = reason.trim();

				// SQLite Will Not Handle The Character ' Well So We Must
				// Replace It With " If It Exists With A String
				for (int i = 0; i < reason.length(); ++i)
					if (reason.charAt(i) == '\'') {
						StringBuilder reasonSB = new StringBuilder(reason);
						reasonSB.setCharAt(i, '"');
						reason = reasonSB.toString();
					}

				// Testing
				/*
				 * System.out.printf(
				 * "Movie: %s\nYear: %s\nRating: %s\nReason: %s\n\n",
				 * movie.trim(), year.trim(), rating.trim(), reason.trim());
				 */

				// Testing
				System.out
						.printf("INSERT INTO MPAA_RATINGS (MOVIE, RATING, REASON, YEAR) VALUES('%s', '%s', '%s', '%s');\n",
								movie.trim(), rating.trim(), reason.trim(),
								year.trim());

				sql = "INSERT INTO MPAA_RATINGS (MOVIE, RATING, REASON, YEAR) "
						+ "VALUES('" + movie.trim() + "', '" + rating.trim()
						+ "', '" + reason.trim() + "', '" + year.trim() + "');";
				stmt.execute(sql);
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
