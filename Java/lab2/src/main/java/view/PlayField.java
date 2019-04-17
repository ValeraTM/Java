package view;

import javax.swing.*;
import java.awt.*;
import java.util.Arrays;

public class PlayField extends JPanel {
    static final Color EMPTY = Color.BLACK;

    PlayField(int height, int width) {
        this.height = height;
        this.width = width;
        colors = new Color[height*width];
        Arrays.fill(colors, EMPTY);
    }

    @Override
    public void paint(Graphics g) {
        Graphics2D painter = (Graphics2D)g;
        super.paint(painter);

        int sizeY = this.getHeight()/height;
        int sizeX = this.getWidth()/width;

        painter.setStroke(new BasicStroke(2.5f));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++  ) {
                painter.setColor(colors[i*width + j]);
                painter.fillRect(j*sizeX, i*sizeY, sizeX, sizeY);
                painter.setColor(Color.WHITE);
                painter.drawRect(j*sizeX, i*sizeY, sizeX, sizeY);
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
