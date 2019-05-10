package view;

import model.Cell;
import model.Game;
import model.figures.Shape;
import observer.Observer;

import javax.swing.*;
import java.awt.*;

public class NextFigure extends JPanel implements Observer {
    public NextFigure(Game model) {
        this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

        JLabel label = new JLabel("Next Figure");
        label.setForeground(Color.WHITE);
        label.setFont(new Font("Name", Font.BOLD, 22));
        this.add(label);
        this.add(Box.createVerticalStrut(10));

        JPanel nextFigure = new JPanel() {
            @Override
            public void paint(Graphics g) {
                Shape it = model.getNextFigure();
                int sizeX = this.getWidth()/4;
                int sizeY = this.getHeight()/4;
                int sizeRect = Integer.min(sizeX, sizeY);

                Graphics2D painter = (Graphics2D)g;
                super.paint(painter);
                painter.setStroke(new BasicStroke(3f));

                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (i < it.getHeight() && j < it.getWidth()) {
                            if (it.isEmpty(j, i)) {
                                painter.setColor(Cell.EMPTY.getColor());
                            }
                            else {
                                painter.setColor(it.getColor());
                            }
                        }
                        else {
                            painter.setColor(Cell.EMPTY.getColor());
                        }
                        painter.fillRect(j*sizeRect, i*sizeRect, sizeRect, sizeRect);
                        painter.setColor(Color.WHITE);
                        painter.drawRect(j*sizeRect, i*sizeRect, sizeRect, sizeRect);
                    }
                }
            }
        };
        this.add(nextFigure);
        nextFigure.setOpaque(false);
        this.setOpaque(false);
    }

    @Override
    public void updateNextFigure() {
        repaint();
    }
}
