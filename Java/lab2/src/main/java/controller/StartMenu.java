package controller;

import model.Game;
import view.*;

import ch.qos.logback.classic.Logger;
import org.slf4j.LoggerFactory;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.IOException;
import java.io.InputStream;

public class StartMenu extends JFrame implements KeyListener, ActionListener {
    private final static Logger logger = (Logger)LoggerFactory.getLogger(StartMenu.class);

    public StartMenu(Game model, JPanel game){
        this.setLocationRelativeTo(null);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setTitle("Tetris");

        this.model = model;
        model.registerObserver(dialog);
        newGame = game;

        Font mainFont = new Font("Controller", Font.BOLD, 36);
        JButton start = new JButton("New Game");
        JButton scores = new JButton("High Scores");
        JButton about = new JButton("About");
        JButton exit = new JButton("Exit");

        JPanel panel = (JPanel)this.getContentPane();
        menu.setOpaque(false);

        panel.setLayout(new BoxLayout(panel, BoxLayout.PAGE_AXIS));
        panel.add(newGame);
        newGame.setVisible(false);
        panel.add(highScores);
        highScores.setVisible(false);
        panel.add(menu);
        menu.setVisible(true);
        panel.add(aboutInfo);
        aboutInfo.setVisible(false);

        highScores.add(score, BorderLayout.CENTER);

        menu.setLayout(new BoxLayout(menu, BoxLayout.PAGE_AXIS));

        JPanel dialogForScores = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        highScores.add(dialogForScores, BorderLayout.SOUTH);

        JButton backFromScores = new JButton("Back");
        backFromScores.setBackground(Color.BLACK);
        backFromScores.setForeground(Color.WHITE);
        backFromScores.setActionCommand("HighScores");
        backFromScores.setFont(mainFont);
        backFromScores.addActionListener(this);
        dialogForScores.add(backFromScores);

        aboutInfo.add(new About(), BorderLayout.CENTER);
        JPanel dialogForAbout = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        aboutInfo.add(dialogForAbout, BorderLayout.SOUTH);

        JButton backForAbout = new JButton("Back");
        backForAbout.setBackground(Color.BLACK);
        backForAbout.setForeground(Color.WHITE);
        backForAbout.setActionCommand("About");
        backForAbout.setFont(mainFont);
        backForAbout.addActionListener(this);
        dialogForAbout.add(backForAbout);

        start.setAlignmentX(Component.CENTER_ALIGNMENT);
        start.setFont(mainFont);
        start.setActionCommand("NewGame");
        start.addActionListener(this);
        start.setBackground(Color.BLACK);
        start.setForeground(Color.WHITE);

        scores.setAlignmentX(Component.CENTER_ALIGNMENT);
        scores.setFont(mainFont);
        scores.setActionCommand("HighScores");
        scores.addActionListener(this);
        scores.setBackground(Color.BLACK);
        scores.setForeground(Color.WHITE);

        about.setAlignmentX(Component.CENTER_ALIGNMENT);
        about.setFont(mainFont);
        about.setActionCommand("About");
        about.addActionListener(this);
        about.setBackground(Color.BLACK);
        about.setForeground(Color.WHITE);

        exit.setAlignmentX(Component.CENTER_ALIGNMENT);
        exit.setFont(mainFont);
        exit.setActionCommand("Exit");
        exit.addActionListener(this);
        exit.setBackground(Color.BLACK);
        exit.setForeground(Color.WHITE);

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

        this.setMinimumSize(new Dimension(600, 620));
        this.setMaximumSize(new Dimension(620, 1200));
        this.setSize(new Dimension(600, 960));
        this.setVisible(true);

        highScores.setOpaque(false);
        aboutInfo.setOpaque(false);
        dialogForAbout.setOpaque(false);
        dialogForScores.setOpaque(false);
        score.setOpaque(false);
    }

    private void changeVisibleAbout() {
        if (aboutInfo.isVisible()) {
            aboutInfo.setVisible(false);
            menu.setVisible(true);
        }
        else {
            menu.setVisible(false);
            aboutInfo.setVisible(true);
        }
    }
    private void changeVisibleHighScores() {
        if (highScores.isVisible()) {
            highScores.setVisible(false);
            menu.setVisible(true);
        }
        else {
            menu.setVisible(false);
            highScores.setVisible(true);
        }
    }
    private void changeVisibleNewGame() {
        if (newGame.isVisible()) {
            newGame.setVisible(false);
            menu.setVisible(true);
        }
        else {
            menu.setVisible(false);
            newGame.setVisible(true);
        }
    }

    @Override
    public void actionPerformed(ActionEvent event) {
        logger.info("Pressed " + event.getActionCommand());
        switch (event.getActionCommand()) {
            case "Exit":
                dispose();
                break;
            case "About":
                changeVisibleAbout();
                break;
            case "HighScores":
                score.setRecords(model.getRecords());
                changeVisibleHighScores();
                break;
            case "NewGame":
                this.setFocusable(true);
                addKeyListener(this);
                changeVisibleNewGame();
                model.startGame();
                break;
            case "Record":
                removeKeyListener(this);
                String name = dialog.getName();
                if (name.length() != 0) {
                    dialog.setVisible(false);
                    model.addNewRecord(name, model.getScore());
                    model.saveRecords();
                    changeVisibleNewGame();
                }
        }
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    @Override
    public void keyPressed(KeyEvent e) {
        logger.debug("Pressed " + KeyEvent.getKeyText(e.getKeyCode()));
        switch (e.getKeyCode()) {
            case KeyEvent.VK_RIGHT:
                model.moveRight();
                break;
            case KeyEvent.VK_LEFT:
                model.moveLeft();
                break;
            case KeyEvent.VK_DOWN:
                model.setDelay(50);
                break;
            case KeyEvent.VK_UP:
                model.rotateLeft();
                break;
            case KeyEvent.VK_ESCAPE:
                model.clear();
                changeVisibleNewGame();
                removeKeyListener(this);
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_DOWN) {
            model.setInitialDelay();
        }
    }

    private Scores score = new Scores();
    private Name dialog = new Name(this,this);
    private JPanel menu = new JPanel(true) {
        @Override
        public void paint(Graphics g) {
            try {
                InputStream in = this.getClass().getClassLoader().getResourceAsStream("StartMenu.jpg");
                if (in == null) {
                    throw new IOException("StartMenu.jpg not found");
                }
                Image img = ImageIO.read(in);
                g.drawImage(img, 0,0,this);
            } catch (IOException ex) {
                logger.warn("Texture for Main Menu", ex);
            }
            super.paint(g);
        }
    };
    private JPanel highScores = new JPanel(new BorderLayout()) {
        @Override
        public void paint(Graphics g) {
            try {
                InputStream in = this.getClass().getClassLoader().getResourceAsStream("MainTexture.jpg");
                if (in == null) {
                    throw new IOException("MainTexture.jpg not found");
                }
                Image img = ImageIO.read(in);
                g.drawImage(img, 0,0,this);
            } catch (IOException ex) {
                logger.warn("Texture for High Scores", ex);
            }
            super.paint(g);
        }
    };
    private JPanel aboutInfo = new JPanel(new BorderLayout()) {
        @Override
        public void paint(Graphics g) {
            try {
                InputStream in = this.getClass().getClassLoader().getResourceAsStream("MainTexture.jpg");
                if (in == null) {
                    throw new IOException("MainTexture.jpg not found");
                }
                Image img = ImageIO.read(in);
                g.drawImage(img, 0,0,this);
            } catch (IOException ex) {
                logger.warn("Texture for About", ex);
            }
            super.paint(g);
        }
    };
    private JPanel newGame;
    private Game model;
}
