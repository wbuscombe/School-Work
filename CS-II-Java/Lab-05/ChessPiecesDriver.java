import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;

public class ChessPiecesDriver {
	public static void main(String[] args) {
		JFrame frame = new JFrame("Lab 5 - Chess Pieces");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		ChessPiecesPrimaryPanel panel = new ChessPiecesPrimaryPanel();
		frame.getContentPane().add(panel);

		frame.pack();
		frame.setVisible(true);

		String phantomMode = System.getenv("PHANTOM_MODE");
		if ("1".equals(phantomMode)) {
			SwingUtilities.invokeLater(() -> {
				try {
					Thread.sleep(500);
					new File("docs/screenshots").mkdirs();
					BufferedImage img = new BufferedImage(
						frame.getContentPane().getWidth(),
						frame.getContentPane().getHeight(),
						BufferedImage.TYPE_INT_RGB);
					frame.getContentPane().paintAll(img.createGraphics());
					ImageIO.write(img, "png", new File("docs/screenshots/chess-board.png"));
					System.out.println("PHANTOM_MODE: Captured chess-board.png");
					System.exit(0);
				} catch (Exception e) {
					e.printStackTrace();
					System.exit(1);
				}
			});
		}
	}
}
