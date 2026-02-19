import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
import java.awt.Component;
import java.awt.Container;

public class SierpinskiTriangleDriver {
	public static void main(String[] args) {
		JFrame frame = new JFrame("Lab 10 – Sierpinski Triangle");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		TrianglePrimaryPanel primaryPanel = new TrianglePrimaryPanel();
		frame.getContentPane().add(primaryPanel);

		frame.pack();
		frame.setVisible(true);

		String phantomMode = System.getenv("PHANTOM_MODE");
		if ("1".equals(phantomMode)) {
			SwingUtilities.invokeLater(() -> {
				try {
					// Find the TrianglePanel and set iteration to 5 for a nice fractal
					TrianglePanel tpanel = findComponent(primaryPanel, TrianglePanel.class);
					if (tpanel != null) {
						tpanel.setIteration(5);
						tpanel.repaint();
					}
					Thread.sleep(500);

					new File("docs/screenshots").mkdirs();
					BufferedImage img = new BufferedImage(
						frame.getContentPane().getWidth(),
						frame.getContentPane().getHeight(),
						BufferedImage.TYPE_INT_RGB);
					frame.getContentPane().paintAll(img.createGraphics());
					ImageIO.write(img, "png", new File("docs/screenshots/sierpinski-triangle.png"));
					System.out.println("PHANTOM_MODE: Captured sierpinski-triangle.png");
					System.exit(0);
				} catch (Exception e) {
					e.printStackTrace();
					System.exit(1);
				}
			});
		}
	}

	@SuppressWarnings("unchecked")
	private static <T> T findComponent(Container container, Class<T> type) {
		for (Component c : container.getComponents()) {
			if (type.isInstance(c)) return (T) c;
			if (c instanceof Container) {
				T found = findComponent((Container) c, type);
				if (found != null) return found;
			}
		}
		return null;
	}
}
