package view;

import javax.swing.*;
import java.awt.*;

public class PlayField extends JPanel {
    PlayField(int height, int width) {
        this.height = height;
        this.width = width;
        colors = new Color[height*width];
    }

    @Override
    public void paint(Graphics g) {
        Graphics2D painter = (Graphics2D)g;
        super.paint(painter);

        int sizeY = this.getHeight()/height;
        int sizeX = this.getWidth()/width;
        int size = Integer.min(sizeX, sizeY);
        int x = this.getWidth() - width*size;

        painter.setStroke(new BasicStroke(3f));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++  ) {
                painter.setColor(colors[i*width + j]);
                painter.fillRect(x + j*size, i*size, size, size);
                if (colors[i*width + j] != Color.BLACK) {
                    painter.setColor(Color.BLACK);
                    painter.fillRect(x + j*size + 12, i*size + 12, size - 24, size - 24);
                    painter.setColor(Color.WHITE);
                    painter.drawRect(x + j*size + 12, i*size + 12, size - 24, size - 24);
                }
                painter.setColor(Color.WHITE);
                painter.drawRect(x + j*size, i*size, size, size);
            }
        }
    }

    void fillRect(int idx, Color color) {
        colors[idx] = color;
    }

    private int height;
    private int width;
    private Color[] colors;
}
