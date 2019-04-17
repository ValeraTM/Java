package view;

import javax.swing.*;
import java.awt.*;
import java.util.Arrays;

public class NextFigure extends JPanel {
    public static final Color EMPTY = Color.BLACK;

    NextFigure(int height, int width) {
        this.setMaximumSize(new Dimension(width*60, 10000));
        this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

        this.height = height;
        this.width = width;
        colors = new Color[height*width];
        Arrays.fill(colors, EMPTY);

        JLabel label = new JLabel("Next Figure");
        label.setFont(new Font("Name", Font.BOLD, 23));
        this.add(label);
        this.add(Box.createVerticalStrut(10));

        this.add(new JPanel() {
            @Override
            public void paint(Graphics g) {
                int sizeX = this.getWidth()/width;
                int sizeY = this.getHeight()/height;
                int sizeRect = Integer.min(sizeX, sizeY);
                Graphics2D painter = (Graphics2D)g;
                super.paint(painter);
                painter.setStroke(new BasicStroke(2));
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        painter.setColor(colors[i*width + j]);
                        painter.fillRect(j*sizeRect, i*sizeRect, sizeRect, sizeRect);
                        painter.setColor(Color.LIGHT_GRAY);
                        painter.drawRect(j*sizeRect, i*sizeRect, sizeRect, sizeRect);
                    }
                }
            }

        });
    }

    void fillRect(int y, int x, Color color) {
        colors[y*width + x] = color;
    }

    void clear() {
        Arrays.fill(colors, EMPTY);
    }

    private Color[] colors;
    private int height;
    private int width;
}
