import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdRandom;

@SuppressWarnings("unchecked")
public class GraphicalInsertion {
	private static final boolean PHANTOM = "1".equals(System.getenv("PHANTOM_MODE"));
	private static final int DELAY = PHANTOM ? 10 : 1000;

	public static void sort(Comparable[] a) {
		// Length of comparable array
		int N = a.length;

		// Loop through entire array
		for (int i = 0; i < N; i++) {
			// Check array for consistency
			assert isSorted(a, 0, i - 1);

			// Draw current array, sorted elements in gray, elements still
			// to be worked on in black
			for (int k = 0; k < N; k++) {
				Double x = 1.0 * k / N;
				Double y = (Double) a[k] / 2.0;
				Double rw = 0.5 / N;
				Double rh = (Double) a[k] / 2.0;
				if (k < i) {
					StdDraw.setPenColor(StdDraw.GRAY);
				} else {
					StdDraw.setPenColor(StdDraw.BLACK);
				}
				StdDraw.filledRectangle(x, y, rw, rh);
			}

			// Draw for one second
			StdDraw.show(DELAY);

			// While element after sorted section is still larger than elements
			// to its left, continue swapping it until it is in its rightful
			// place
			for (int j = i; j > 0 && less(a[j], a[j - 1]); j--) {
				// Clear canvas for next graph
				StdDraw.clear();

				// Draw current array, sorted elements in gray, elements still
				// to be worked on in black and the selected element in red
				for (int k = 0; k < N; k++) {
					Double x = 1.0 * k / N;
					Double y = (Double) a[k] / 2.0;
					Double rw = 0.5 / N;
					Double rh = (Double) a[k] / 2.0;

					if (k < i) {
						StdDraw.setPenColor(StdDraw.GRAY);
					} else if (k == j) {
						StdDraw.setPenColor(StdDraw.RED);
					} else {
						StdDraw.setPenColor(StdDraw.BLACK);
					}
					StdDraw.filledRectangle(x, y, rw, rh);
				}

				// Draw for one second
				StdDraw.show(DELAY);

				// Clear canvas for next graph
				StdDraw.clear();

				// Draw current array, sorted elements in gray, elements still
				// to be worked on in black and the two elements to swap in red
				for (int k = 0; k < N; k++) {
					Double x = 1.0 * k / N;
					Double y = (Double) a[k] / 2.0;
					Double rw = 0.5 / N;
					Double rh = (Double) a[k] / 2.0;

					if (k == j || k == (j - 1)) {
						StdDraw.setPenColor(StdDraw.RED);
					} else if (k < i) {
						StdDraw.setPenColor(StdDraw.GRAY);
					} else {
						StdDraw.setPenColor(StdDraw.BLACK);
					}
					StdDraw.filledRectangle(x, y, rw, rh);
				}

				// Draw for one second
				StdDraw.show(DELAY);

				// Swap two selected elements
				exch(a, j, j - 1);

				// Check array for consistency
				assert isSorted(a);

				// Clear canvas for next graph
				StdDraw.clear();
			}
			// Check array for consistency
			assert isSorted(a, 0, i);
		}
		// Check array for consistency
		assert isSorted(a);

		// Clear canvas for next graph
		StdDraw.clear();

		// Draw final, sorted array. All elements in gray
		for (int k = 0; k < N; k++) {
			Double x = 1.0 * k / N;
			Double y = (Double) a[k] / 2.0;
			Double rw = 0.5 / N;
			Double rh = (Double) a[k] / 2.0;
			StdDraw.setPenColor(StdDraw.GRAY);
			StdDraw.filledRectangle(x, y, rw, rh);
		}

		// Draw for one second
		StdDraw.show(DELAY);
	}

	// is v < w ?
	private static boolean less(Comparable v, Comparable w) {
		return (v.compareTo(w) < 0);
	}

	// exchange a[i] and a[j]
	private static void exch(Object[] a, int i, int j) {
		Object swap = a[i];
		a[i] = a[j];
		a[j] = swap;
	}

	// is the array a[] sorted?
	private static boolean isSorted(Comparable[] a) {
		return isSorted(a, 0, a.length - 1);
	}

	// is the array sorted from a[lo] to a[hi]
	private static boolean isSorted(Comparable[] a, int lo, int hi) {
		for (int i = lo + 1; i <= hi; i++)
			if (less(a[i], a[i - 1]))
				return false;
		return true;
	}

	public static void main(String[] args) {
		int N = (args.length > 0) ? Integer.parseInt(args[0]) : 20;

		Double[] a = new Double[N];

		// Fill a with doubles from 1/N to N
		for (int i = 0; i < N; ++i) {
			a[i] = (1.0 / N) * i;
		}

		// Shuffle a
		StdRandom.shuffle(a);

		// Run insertion sort method
		GraphicalInsertion.sort(a);

		if (PHANTOM) {
			new java.io.File("docs/screenshots").mkdirs();
			StdDraw.save("docs/screenshots/insertion-sort.png");
			System.out.println("PHANTOM_MODE: Captured insertion-sort.png");
			System.exit(0);
		}
	}

}
