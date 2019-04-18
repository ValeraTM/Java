package view;

import controller.Controller;

import javax.swing.*;
import java.awt.*;

public class StartMenu extends JFrame {
    public StartMenu(NewGame game, Scores records, Controller controller){
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setTitle("Tetris");
        this.setFocusable(true);

        newGame = game;
        newGame.setParent(this);

        Font mainFont = new Font("Controller", Font.PLAIN, 20);
        JButton start = new JButton("New Game");
        JButton scores = new JButton("High Scores");
        JButton about = new JButton("About");
        JButton exit = new JButton("Exit");

        JPanel panel = (JPanel)this.getContentPane();
        panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
        panel.add(newGame);
        newGame.setVisible(false);
        panel.add(highScores);
        highScores.setVisible(false);
        panel.add(menu);
        menu.setVisible(true);
        panel.add(aboutInfo);
        aboutInfo.setVisible(false);

        highScores.add(records, BorderLayout.CENTER);

        menu.setLayout(new BoxLayout(menu, BoxLayout.PAGE_AXIS));

        JPanel dialogForScores = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        highScores.add(dialogForScores, BorderLayout.SOUTH);

        JButton backFromScores = new JButton("Back");
        backFromScores.setActionCommand("HighScores");
        backFromScores.setFont(mainFont);
        backFromScores.addActionListener(controller);
        dialogForScores.add(backFromScores);


        JPanel dialogForAbout = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        aboutInfo.add(dialogForAbout, BorderLayout.SOUTH);

        JButton backForAbout = new JButton("Back");
        backForAbout.setActionCommand("About");
        backForAbout.setFont(mainFont);
        backForAbout.addActionListener(controller);
        dialogForAbout.add(backForAbout);

        start.setAlignmentX(Component.CENTER_ALIGNMENT);
        start.setFont(mainFont);
        start.setActionCommand("NewGame");
        start.addActionListener(controller);

        scores.setAlignmentX(Component.CENTER_ALIGNMENT);
        scores.setFont(mainFont);
        scores.setActionCommand("HighScores");
        scores.addActionListener(controller);

        about.setAlignmentX(Component.CENTER_ALIGNMENT);
        about.setFont(mainFont);
        about.setActionCommand("About");
        about.addActionListener(controller);

        exit.setAlignmentX(Component.CENTER_ALIGNMENT);
        exit.setFont(mainFont);
        exit.setActionCommand("Exit");
        exit.addActionListener(controller);

        menu.add(Box.createVerticalGlue());
        menu.add(Box.createVerticalStrut(20));
        menu.add(start);
        menu.add(Box.createVerticalStrut(20));
        menu.add(scores);
        menu.add(Box.createVerticalStrut(20));
        menu.add(about);
        menu.add(Box.createVerticalStrut(20));
        menu.add(exit);
        menu.add(Box.createVerticalStrut(20));
        menu.add(Box.createVerticalGlue());

        this.pack();
        this.setMinimumSize(this.getSize());
        this.setVisible(true);
    }

    public void changeVisibleAbout() {
        if (aboutInfo.isVisible()) {
            aboutInfo.setVisible(false);
            menu.setVisible(true);
        }
        else {
            menu.setVisible(false);
            aboutInfo.setVisible(true);
        }
    }

    public void changeVisibleHighScores() {
        if (highScores.isVisible()) {
            highScores.setVisible(false);
            menu.setVisible(true);
        }
        else {
            menu.setVisible(false);
            highScores.setVisible(true);
        }
    }

    public void changeVisibleNewGame() {
        if (newGame.isVisible()) {
            newGame.setVisible(false);
            menu.setVisible(true);
        }
        else {
            menu.setVisible(false);
            newGame.setVisible(true);
        }
    }

    private JPanel menu = new JPanel();
    private JPanel highScores = new JPanel(new BorderLayout());
    private JPanel aboutInfo = new JPanel(new BorderLayout());
    private NewGame newGame;
}
