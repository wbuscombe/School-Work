import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

public class Brute {
	private static final boolean PHANTOM = "1".equals(System.getenv("PHANTOM_MODE"));

	public static void main(String[] args) {
		// rescale coordinates and turn on animation mode
		StdDraw.setXscale(0, 32768);
		StdDraw.setYscale(0, 32768);
		StdDraw.show(0);

		// read in the input (from file in phantom mode, stdin otherwise)
		In input = PHANTOM ? new In("input10.txt") : new In();
		int N = input.readInt();
		Point points[] = new Point[N];
		for (int i = 0; i < N; ++i) {
			int x = input.readInt();
			int y = input.readInt();
			points[i] = new Point(x, y);

			// Draw all of the points we read in
			StdDraw.setPenRadius(0.015);
			StdDraw.setPenColor(StdDraw.BLACK);
			points[i].draw();
		}

		// For each set of four points in the set
		for (int i = 0; i < N; ++i)
			for (int j = (i + 1); j < N; ++j)
				for (int k = (j + 1); k < N; ++k)
					for (int l = (k + 1); l < N; ++l) {
						// If the four points are collinear
						if (points[i].areCollinear(points[j], points[k],
								points[l])) {
							// Draw lines connecting each point
							StdDraw.setPenRadius(0.005);
							StdDraw.setPenColor(StdDraw.RED);
							points[i].drawTo(points[j]);
							points[i].drawTo(points[k]);
							points[i].drawTo(points[l]);

							// Display all of the collinear points
							StdOut.println(displayCollinearPoints(points[i],
									points[j], points[k], points[l]));
						}
					}

		// display to screen all at once
		StdDraw.show(0);

		if (PHANTOM) {
			new java.io.File("docs/screenshots").mkdirs();
			StdDraw.save("docs/screenshots/collinear-points.png");
			System.out.println("PHANTOM_MODE: Captured collinear-points.png");
			System.exit(0);
		}
	}

	public static String displayCollinearPoints(Point a, Point b, Point c,
			Point d) {
		return ("4: " + a + " -> " + b + " -> " + c + " -> " + d);
	}
}
