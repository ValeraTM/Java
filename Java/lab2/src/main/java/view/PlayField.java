package view;

import model.Game;
import model.Glass;
import observer.Observer;

import javax.swing.*;
import java.awt.*;

public class PlayField extends JPanel implements Observer {
    public PlayField(Game model) {
        this.model = model;
        this.setOpaque(false);
    }

    @Override
    public void paint(Graphics g) {
        Graphics2D painter = (Graphics2D)g;
        super.paint(painter);

        Glass field = model.getField();

        int sizeY = this.getHeight()/field.getHeight();
        int sizeX = this.getWidth()/field.getWidth();
        int size = Integer.min(sizeX, sizeY);
        int x = this.getWidth() - field.getWidth()*size;

        painter.setStroke(new BasicStroke(3f));
        for (int i = 0; i < field.getHeight(); i++) {
            for (int j = 0; j < field.getWidth(); j++  ) {
                painter.setColor(field.getColor(j, field.getHeight() - 1 - i));
                painter.fillRect(x + j*size, i*size, size, size);
                painter.setColor(Color.WHITE);
                painter.drawRect(x + j*size, i*size, size, size);

                if (field.getHeight() - i == field.getBorder()) {
                    painter.setColor(Color.RED);
                    painter.drawLine(x,i*size, x + field.getWidth()*size,i*size);
                }
            }
        }
    }

    @Override
    public void updateField() {
        repaint();
    }

    private Game model;
}
