package controller;

import model.Game;
import view.NewGame;
import view.Scores;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.InputStream;

public class StartMenu extends JFrame implements ActionListener, KeyListener {
    public StartMenu(InputStream config, int height, int width) throws Exception {
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setTitle("Tetris");
        this.setFocusable(true);

        newGame = new NewGame(height, width);
        tetris = new Game(width, height, config);

        Font mainFont = new Font("StartMenu", Font.PLAIN, 20);
        JButton start = new JButton("New Game");
        JButton scores = new JButton("High Scores");
        JButton about = new JButton("About");
        JButton exit = new JButton("Exit");

        JPanel panel = (JPanel)this.getContentPane();
        panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
        panel.add(newGame);

        JPanel highScores = new JPanel(new BorderLayout());
        highScores.setVisible(false);
        panel.add(highScores);

        records = new Scores();
        highScores.add(records, BorderLayout.CENTER);

        menu.setLayout(new BoxLayout(menu, BoxLayout.PAGE_AXIS));
        panel.add(menu);

        JPanel dialogForScores = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        highScores.add(dialogForScores, BorderLayout.SOUTH);
        JButton backFromScores = new JButton("Back");
        backFromScores.setFont(mainFont);
        backFromScores.addActionListener((ActionEvent event) -> {highScores.setVisible(false); menu.setVisible(true);});
        dialogForScores.add(backFromScores);

        JPanel aboutInfo = new JPanel(new BorderLayout());
        aboutInfo.setVisible(false);
        panel.add(aboutInfo);

        JPanel dialog = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        aboutInfo.add(dialog, BorderLayout.SOUTH);

        JButton back = new JButton("Back");
        back.setFont(mainFont);
        back.setFocusable(false);
        back.addActionListener((ActionEvent event) -> {aboutInfo.setVisible(false); menu.setVisible(true);});
        dialog.add(back);

        JLabel info = new JLabel("<html><center>Created by<br />Malkhanov Valery</center></html>");
        info.setFont(mainFont);
        info.setHorizontalAlignment(SwingConstants.CENTER);
        aboutInfo.add(info, BorderLayout.CENTER);

        start.setAlignmentX(Component.CENTER_ALIGNMENT);
        start.setFont(mainFont);
        start.addActionListener((ActionEvent event) -> {newGame.setVisible(true); menu.setVisible(false); startPlay();});

        scores.setAlignmentX(Component.CENTER_ALIGNMENT);
        scores.setFont(mainFont);
        scores.addActionListener((ActionEvent event) -> {menu.setVisible(false); highScores.setVisible(true);});

        about.setAlignmentX(Component.CENTER_ALIGNMENT);
        about.setFont(mainFont);
        about.addActionListener((ActionEvent event) -> {aboutInfo.setVisible(true); menu.setVisible(false);});

        exit.setAlignmentX(Component.CENTER_ALIGNMENT);
        exit.setFont(mainFont);
        exit.addActionListener((ActionEvent event) -> dispose());

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

    private void startPlay() {
        this.addKeyListener(this);
        newGame.repaintFigure(tetris.getFigure(), tetris.getX(), tetris.getY());
        newGame.showNextFigure(tetris.getNextFigure());

        timer.start();
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    @Override
    public void keyPressed(KeyEvent e) {
        switch (e.getKeyCode()) {
            case KeyEvent.VK_RIGHT:
                newGame.clearFigure(tetris.getFigure(), tetris.getX(), tetris.getY());
                tetris.moveRight();
                newGame.repaintFigure(tetris.getFigure(), tetris.getX(), tetris.getY());
                break;
            case KeyEvent.VK_LEFT:
                newGame.clearFigure(tetris.getFigure(), tetris.getX(), tetris.getY());
                tetris.moveLeft();
                newGame.repaintFigure(tetris.getFigure(), tetris.getX(), tetris.getY());
                break;
            case KeyEvent.VK_DOWN:
                timer.setDelay(50);
                break;
            case KeyEvent.VK_UP:
                newGame.clearFigure(tetris.getFigure(), tetris.getX(), tetris.getY());
                tetris.rotateLeft();
                newGame.repaintFigure(tetris.getFigure(), tetris.getX(), tetris.getY());
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_DOWN) {
            int count = tetris.getScore()/1000;
            timer.setDelay(delay - count*25);
        }
    }

    @Override
    public void actionPerformed(ActionEvent event) {
        newGame.clearFigure(tetris.getFigure(), tetris.getX(), tetris.getY());
        if (!tetris.moveDown()) {
            if (!tetris.fall()) {
                timer.stop();
                this.removeKeyListener(this);
                String result = JOptionPane.showInputDialog(this, "Enter the name", "Player");
                records.addNewRecord(result, tetris.getScore());
                records.saveRecords();
                menu.setVisible(true);
                newGame.setVisible(false);
                return;
            }

            int count = tetris.getScore()/1000;
            timer.setDelay(delay - count*25);

            if (tetris.checkFilledRow()) {
                newGame.setScores(tetris.getScore());
            }
            newGame.repaintField(tetris.getField());
            newGame.showNextFigure(tetris.getNextFigure());
        }
        newGame.repaintFigure(tetris.getFigure(), tetris.getX(), tetris.getY());
    }

    private JPanel menu = new JPanel();
    private int delay = 500;
    private Scores records;
    private Timer timer = new Timer(delay, this);
    private NewGame newGame;
    private Game tetris;
}