import java.awt.Component;
import java.awt.Graphics;
import java.awt.Image;

import javax.swing.ImageIcon;

public class Knight extends ChessPiece {
	public Knight() {
		super();

		this.icon = new ImageIcon(this.getClass().getResource("knight.png"));

		circles = new Circle[8];
	}

	public void draw(Component c, Graphics g) {
		Image image = icon.getImage().getScaledInstance(width, height,
				Image.SCALE_SMOOTH);
		icon.setImage(image);

		icon.paintIcon(c, g, x, y);
	}

	public void drawMoves(Graphics g) {
		int circle_width = (this.width / 2);
		int circle_height = (this.height / 2);

		circles[0] = new Circle((int) (width * 1.25), (int) (height * 0.25),
				circle_width, circle_height);
		circles[1] = new Circle((int) (width * 0.25), (int) (height * 1.25),
				circle_width, circle_height);
		circles[2] = new Circle((int) (width * 0.25), (int) (height * 3.25),
				circle_width, circle_height);
		circles[3] = new Circle((int) (width * 1.25), (int) (height * 4.25),
				circle_width, circle_height);
		circles[4] = new Circle((int) (width * 3.25), (int) (height * 4.25),
				circle_width, circle_height);
		circles[5] = new Circle((int) (width * 4.25), (int) (height * 3.25),
				circle_width, circle_height);
		circles[6] = new Circle((int) (width * 4.25), (int) (height * 1.25),
				circle_width, circle_height);
		circles[7] = new Circle((int) (width * 3.25), (int) (height * 0.25),
				circle_width, circle_height);

		for (int i = 0; i < circles.length; ++i) {
			circles[i].draw(g);
		}
	}
}
