package controller;

import model.Game;
import view.StartMenu;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class Controller implements ActionListener, KeyListener {
    public Controller(Game tetris) {
        this.tetris = tetris;
    }

    public void setFrame(StartMenu menu) {
        frame = menu;
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    @Override
    public void keyPressed(KeyEvent e) {
        switch (e.getKeyCode()) {
            case KeyEvent.VK_RIGHT:
                tetris.moveRight();
                break;
            case KeyEvent.VK_LEFT:
                tetris.moveLeft();
                break;
            case KeyEvent.VK_DOWN:
                tetris.setDelay(50);
                break;
            case KeyEvent.VK_UP:
                tetris.rotateLeft();
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_DOWN) {
            tetris.setInitialDelay();
        }
    }

    @Override
    public void actionPerformed(ActionEvent event) {
        switch (event.getActionCommand()) {
            case "Exit":
                frame.dispose();
                break;
            case "About":
                frame.changeVisibleAbout();
                break;
            case "HighScores":
                frame.changeVisibleHighScores();
                break;
            case "NewGame":
                frame.addKeyListener(this);
                frame.changeVisibleNewGame();
                tetris.startGame();
                break;
        }
    }

    private StartMenu frame;
    private Game tetris;
}