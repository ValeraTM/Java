package view;

import model.Cell;
import model.Glass;
import model.figures.Shape;
import observer.Observer;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;

public class NewGame extends JPanel implements Observer {
    public NewGame(int height, int width) {
        this.height = height;
        this.width = width;
        field = new PlayField(height, width);
        this.setLayout(new BoxLayout(this, BoxLayout.X_AXIS));

        JPanel forField = new JPanel();
        forField.setLayout(new BoxLayout(forField, BoxLayout.Y_AXIS));
        forField.add(Box.createVerticalStrut(10));
        forField.add(field);
        forField.add(Box.createVerticalStrut(10));

        this.add(forField);
        this.add(Box.createHorizontalStrut(25));

        JPanel info = new JPanel();
        info.setLayout(new BoxLayout(info, BoxLayout.Y_AXIS));
        this.add(info);
        info.setMaximumSize(new Dimension(200, 1200));
        info.setMinimumSize(new Dimension(120, 720));

        nextFigure = new NextFigure(4, 4);
        info.add(Box.createVerticalStrut(70));
        info.add(nextFigure);
        info.add(Box.createVerticalStrut(50));

        JPanel infoScores = new JPanel();
        infoScores.setLayout(new BoxLayout(infoScores, BoxLayout.Y_AXIS));
        Font font = new Font("Scores", Font.BOLD, 30);
        JLabel header = new JLabel(" Scores");
        header.setForeground(Color.WHITE);
        scores.setForeground(Color.WHITE);
        level.setForeground(Color.WHITE);
        header.setFont(font);
        scores.setFont(font);
        level.setFont(font);
        infoScores.add(header);
        infoScores.add(Box.createVerticalStrut(10));
        infoScores.add(scores);

        info.add(infoScores);
        info.add(Box.createVerticalGlue());
        info.add(level);
        info.add(Box.createVerticalStrut(80));

        this.add(Box.createHorizontalStrut(20));

        this.setOpaque(false);
        forField.setOpaque(false);
        field.setOpaque(false);
        info.setOpaque(false);
        infoScores.setOpaque(false);
        nextFigure.setOpaque(false);
    }

    public void setObserverForScores(Name name) {
        dialog = name;
    }

    @Override
    public void updateField(Glass field) {
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                this.field.fillRect((height - 1 - i)*width + j, field.getColor(j, i));
            }
        }

        this.field.repaint();
    }

    @Override
    public void updateNextFigure(Shape figure) {
        nextFigure.clear(Cell.EMPTY.getColor());
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (!figure.isEmpty(j, i)) {
                    nextFigure.fillRect(i, j, figure.getColor());
                }
            }
        }
        nextFigure.repaint();
    }

    @Override
    public void updateScore(int score) {
        scores.setText("    " + score);
        level.setText(" Level " + (score/1000 + 1));
    }

    @Override
    public String getRecord() {
        dialog.clear();
        dialog.setVisible(true);
        while (dialog.isVisible());
        return dialog.getName();
    }

    @Override
    public void paint(Graphics g) {
        try {
            Image img = ImageIO.read(new File("src/main/resources/3.jpg"));
            g.drawImage(img, 0,0,null);
        } catch (IOException e) {
            e.printStackTrace();
        }
        super.paint(g);
    }

    private Name dialog;
    private int height;
    private int width;
    private JLabel level = new JLabel(" Level 1");
    private JLabel scores = new JLabel("    0");
    private PlayField field;
    private NextFigure nextFigure;
}
