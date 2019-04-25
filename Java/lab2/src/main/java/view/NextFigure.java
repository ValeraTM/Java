package view;

import javax.swing.*;
import java.awt.*;
import java.util.Arrays;

class NextFigure extends JPanel {
    NextFigure(int height, int width) {
        this.setMaximumSize(new Dimension(width*60, 10000));
        this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

        this.height = height;
        this.width = width;
        colors = new Color[height*width];

        JLabel label = new JLabel("Next Figure");
        label.setForeground(Color.WHITE);
        label.setFont(new Font("Name", Font.BOLD, 22));
        this.add(label);
        this.add(Box.createVerticalStrut(10));

        JPanel nextFigure = new JPanel() {
            @Override
            public void paint(Graphics g) {
                int sizeX = this.getWidth()/width;
                int sizeY = this.getHeight()/height;
                int sizeRect = Integer.min(sizeX, sizeY);
                Graphics2D painter = (Graphics2D)g;
                super.paint(painter);
                painter.setStroke(new BasicStroke(3f));
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        painter.setColor(colors[i*width + j]);
                        painter.fillRect(j*sizeRect, i*sizeRect, sizeRect, sizeRect);
                        painter.setColor(Color.WHITE);
                        painter.drawRect(j*sizeRect, i*sizeRect, sizeRect, sizeRect);
                    }
                }
            }
        };
        this.add(nextFigure);
        nextFigure.setOpaque(false);
    }

    void fillRect(int y, int x, Color color) {
        colors[y*width + x] = color;
    }

    void clear(Color color) {
        Arrays.fill(colors, color);
    }

    private Color[] colors;
    private int height;
    private int width;
}
