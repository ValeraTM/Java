package view;

import model.Glass;
import model.figures.Shape;

import javax.swing.*;
import java.awt.*;

public class NewGame extends JPanel implements Gui {
    public NewGame(int height, int width, Scores records) {
        this.records = records;
        this.height = height;
        this.width = width;
        field = new PlayField(height, width);

        this.setLayout(new BoxLayout(this, BoxLayout.X_AXIS));

        this.add(Box.createHorizontalGlue());

        JPanel forField = new JPanel();
        forField.setLayout(new BoxLayout(forField, BoxLayout.Y_AXIS));
        forField.add(Box.createVerticalStrut(30));
        forField.add(field);
        forField.add(Box.createVerticalStrut(30));

        this.add(forField);
        this.add(Box.createHorizontalStrut(30));

        JPanel info = new JPanel();
        info.setLayout(new BoxLayout(info, BoxLayout.Y_AXIS));
        this.add(info);

        nextFigure = new NextFigure(4, 4);
        info.add(Box.createVerticalStrut(70));
        info.add(nextFigure);
        info.add(Box.createVerticalStrut(50));

        JPanel infoScores = new JPanel();
        infoScores.setLayout(new BoxLayout(infoScores, BoxLayout.Y_AXIS));
        Font font = new Font("Scores", Font.ITALIC, 30);
        JLabel header = new JLabel("Scores:");
        header.setFont(font);
        scores.setFont(font);
        infoScores.add(header);
        infoScores.add(scores);

        info.add(infoScores);
        info.add(Box.createVerticalGlue());

        this.add(Box.createHorizontalGlue());
    }
    public void setParent(StartMenu frame) {
        this.frame = frame;
    }

    public void showNextFigure(Shape figure) {
        nextFigure.clear();
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (!figure.isEmpty(j, i)) {
                    nextFigure.fillRect(i, j, figure.getColor());
                }
            }
        }
        nextFigure.repaint();
    }
    public void repaintField(Glass field) {
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                if (!field.isEmpty(j, i)) {
                    this.field.fillRect((height - 1 - i)*width + j, field.getColor(j, i));
                }
                else {
                    this.field.fillRect((height - 1 - i)*width + j, Glass.EMPTY);
                }
            }
        }

        this.field.repaint();
    }
    public void clearFigure(Shape figure, int x, int y) {
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (!figure.isEmpty(j, i)) {
                    field.fillRect((height - 1 - (y - i))*width + x + j, Glass.EMPTY);
                }
            }
        }
    }
    public void paintFigure(Shape figure, int x, int y) {
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (!figure.isEmpty(j, i)) {
                    field.fillRect((height - 1 - (y - i))*width + x + j, figure.getColor());
                }
            }
        }

        field.repaint();
    }
    public void setScores(int newScores) {
        scores.setText(Integer.toString(newScores));
    }

    @Override
    public void saveRecord(int score) {
        this.removeAll();
        String result = JOptionPane.showInputDialog(this, "Enter the name", "Player");
        records.addNewRecord(result, score);
        records.saveRecords();
        frame.changeVisibleNewGame();
    }

    private int height;
    private int width;
    private JLabel scores = new JLabel("0");
    private Scores records;
    private PlayField field;
    private NextFigure nextFigure;
    private StartMenu frame;
}
