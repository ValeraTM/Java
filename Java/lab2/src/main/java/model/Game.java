package model;

import model.figures.Shape;
import view.Gui;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.io.InputStream;

public class Game {
    public Game(int width, int height, InputStream config, Gui observer) throws Exception {
        this.observer = observer;
        factory = new Factory(config);
        field = new Glass(height, width);
        figure = getNewFigure();
        newFigure = getNewFigure();
        x = width/2;
        y = height - 1;
    }
    public void startGame() {
        observer.repaintField(field);
        observer.paintFigure(figure, x ,y);
        observer.showNextFigure(newFigure);

        timer = new Timer(delay, (ActionEvent event) -> {
            observer.clearFigure(figure, x, y);
            if (!moveDown()) {
                if (!fall()) {
                    timer.stop();
                    clear();
                    observer.saveRecord(score);
                    return;
                }

                timer.setDelay(delay - score/1000*25);

                if (checkFilledRow()) {
                    observer.setScores(score);
                }
                observer.repaintField(field);
                observer.showNextFigure(newFigure);
            }
            observer.paintFigure(figure, x, y);
        });

        timer.start();
    }

    public void moveLeft() {
        observer.clearFigure(figure, x ,y);
        if (x != 0) {
            boolean canMove = true;
            for (int i = 0; i < figure.getHeight(); i++) {
                if (y - i >= field.getHeight()) {
                    continue;
                }
                if (!field.isEmpty(x - 1,y - i) && !figure.isEmpty(0, i)) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                x--;
            }
        }
        observer.paintFigure(figure, x ,y);
    }
    public void moveRight() {
        observer.clearFigure(figure, x ,y);
        if (x + figure.getWidth() != field.getWidth()) {
            boolean canMove = true;
            for (int i = 0; i < figure.getHeight(); i++) {
                if (y - i >= field.getHeight()) {
                    continue;
                }
                if (!field.isEmpty(x + figure.getWidth(), y - i) && !figure.isEmpty(figure.getWidth() - 1, i)) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                x++;
            }
        }
        observer.paintFigure(figure, x ,y);
    }
    private boolean moveDown() {
        if (y - figure.getHeight() + 1 != 0) {
            boolean canMove = true;
            for (int j = figure.getHeight() - 1; j >= 0; j--) {
                for (int i = 0; i < figure.getWidth(); i++) {
                    if (!field.isEmpty(x + i, y - j - 1) && !figure.isEmpty(i, j)) {
                        canMove = false;
                        break;
                    }
                }
            }
            if (canMove) {
                y--;
                return true;
            }
        }
        return false;
    }
    public boolean rotateRight() {
        figure.rotateRight();
        if (!canRotate()) {
            figure.rotateLeft();
            return false;
        }
        return true;
    }
    public boolean rotateLeft() {
        observer.clearFigure(figure, x, y);
        figure.rotateLeft();
        if (!canRotate()) {
            figure.rotateRight();
            observer.paintFigure(figure, x ,y);
            return false;
        }
        observer.paintFigure(figure, x, y);
        return true;
    }
    private boolean fall() {
        if (y > field.getBorder()) {
            return false;
        }
        field.setFigure(figure, x, y);
        return true;
    }
    private boolean checkFilledRow() {
        int count = field.destroyFilledRow();

        switch (count) {
            case 1:
                score += 100;
                break;
            case 2:
                score += 300;
                break;
            case 3:
                score += 700;
                break;
            case 4:
                score += 1500;
        }

        figure = newFigure;
        newFigure = getNewFigure();
        x = field.getWidth()/2;
        y = field.getHeight() - 1;

        return count != 0;
    }
    private void clear() {
        figure = getNewFigure();
        newFigure = getNewFigure();
        x = field.getWidth()/2;
        y = field.getHeight() - 1;
        field.clear();
    }
    public void setDelay(int delay) {
        timer.setDelay(delay);
    }
    public void setInitialDelay() {
        timer.setDelay(delay - score/1000*25);
    }

    private Shape getNewFigure() {
        try {
            return factory.createRandomProduct();
        }
        catch (Exception ex) {
            return null;
        }
    }
    private boolean canRotate() {
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (x + j >= field.getWidth()) {
                    return false;
                }
                if (!figure.isEmpty(j, i) && !field.isEmpty(x + j, y - i)) {
                    return false;
                }
            }
        }
        return true;
    }

    private int x;
    private int y;
    private Shape figure;
    private Shape newFigure;
    private int score = 0;
    private Factory factory;
    private Glass field;
    private Gui observer;
    private int delay = 500;
    private Timer timer;
}
