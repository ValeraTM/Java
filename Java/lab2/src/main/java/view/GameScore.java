package view;

import model.Game;
import observer.Observer;

import javax.swing.*;
import java.awt.*;

public class GameScore extends JPanel implements Observer {
    public GameScore(Game model) {
        this.model = model;
        this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));

        Font font = new Font("Scores", Font.BOLD, 30);
        JLabel header = new JLabel(" Scores");
        header.setForeground(Color.WHITE);
        scores.setForeground(Color.WHITE);
        level.setForeground(Color.WHITE);
        header.setFont(font);
        scores.setFont(font);
        level.setFont(font);

        this.add(header);
        this.add(Box.createVerticalStrut(10));
        this.add(scores);
        this.add(Box.createVerticalGlue());
        this.add(level);

        this.setOpaque(false);
    }

    @Override
    public void updateScore() {
        scores.setText("    " + model.getScore());
        level.setText(" Level " + (model.getScore()/1000 + 1));
    }

    private Game model;
    private JLabel level = new JLabel(" Level 1");
    private JLabel scores = new JLabel("    0");
}
