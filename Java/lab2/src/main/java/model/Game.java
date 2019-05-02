package model;

import model.figures.Shape;
import observer.Observable;
import observer.Observer;

import javax.swing.Timer;
import java.awt.event.ActionEvent;
import java.io.*;
import java.util.LinkedList;
import java.util.List;

public class Game implements Observable {
    public Game(int width, int height, Factory factory) {
        observers = new LinkedList<>();
        this.factory = factory;
        field = new Glass(height, width);
        figure = getNewFigure();
        newFigure = getNewFigure();
        x = width/2;
        y = height - 1;

        try {
            ObjectInputStream file = new ObjectInputStream(new FileInputStream("src/main/resources/HighScores.ser"));
            records = (Scores)file.readObject();
        }
        catch (Exception ex) {
            records = new Scores();
        }
    }

    @Override
    public void notifyObservers(Update mark) {
        for (Observer it : observers) {
            switch (mark) {
                case FIELD:
                    it.updateField(field);
                    break;
                case NEXT_FIGURE:
                    it.updateNextFigure(newFigure);
                    break;
                case SCORE:
                    it.updateScore(score);
                    break;
                case RECORD:
                    records.addNewRecord(it.getRecord(), score);
            }
        }
    }

    @Override
    public void removeObserver(Observer o) {
        observers.remove(o);
    }

    @Override
    public void registerObserver(Observer o) {
        observers.add(o);
    }

    public void startGame() {
        field.updateFigure(figure, y, x, figure.getCell());
        notifyObservers(Update.SCORE);
        notifyObservers(Update.FIELD);
        notifyObservers(Update.NEXT_FIGURE);

        timer = new Timer(delay, (ActionEvent event) -> {
            field.updateFigure(figure, y, x, Cell.EMPTY);
            if (!moveDown()) {
                if (!fall()) {
                    notifyObservers(Update.RECORD);
                    clear();
                    return;
                }

                timer.setDelay(delay - score/1000*25);

                if (checkFilledRow()) {
                    notifyObservers(Update.SCORE);
                }

                notifyObservers(Update.NEXT_FIGURE);
            }
            field.updateFigure(figure, y, x, figure.getCell());
            notifyObservers(Update.FIELD);
        });

        timer.setInitialDelay(delay*3);
        timer.start();
    }

    public void moveLeft() {
        field.updateFigure(figure, y, x, Cell.EMPTY);
        if (x != 0) {
            boolean canMove = true;
            for (int i = 0; i < figure.getHeight(); i++) {
                if (y - i >= field.getHeight()) {
                    continue;
                }
                for (int j = 0; j < figure.getWidth(); j++) {
                    if (!field.isEmpty(x + j - 1, y - i) && !figure.isEmpty(j, i)) {
                        canMove = false;
                        break;
                    }
                }
                if (!canMove) {
                    break;
                }
            }
            if (canMove) {
                x--;
            }
        }
        field.updateFigure(figure, y, x, figure.getCell());
        notifyObservers(Update.FIELD);
    }
    public void moveRight() {
        field.updateFigure(figure, y, x, Cell.EMPTY);
        if (x + figure.getWidth() != field.getWidth()) {
            boolean canMove = true;
            for (int i = 0; i < figure.getHeight(); i++) {
                if (y - i >= field.getHeight()) {
                    continue;
                }
                for (int j = figure.getWidth() - 1; j >= 0; j--) {
                    if (!field.isEmpty(x + j + 1, y - i) && !figure.isEmpty(j, i)) {
                        canMove = false;
                        break;
                    }
                }
                if (!canMove) {
                    break;
                }
            }
            if (canMove) {
                x++;
            }
        }
        field.updateFigure(figure, y, x, figure.getCell());
        notifyObservers(Update.FIELD);
    }
    public boolean rotateRight() {
        field.updateFigure(figure, y, x, Cell.EMPTY);
        figure.rotateRight();
        if (!canRotate()) {
            figure.rotateLeft();
            field.updateFigure(figure, y, x, figure.getCell());
            return false;
        }
        field.updateFigure(figure, y, x, figure.getCell());
        notifyObservers(Update.FIELD);
        return true;
    }
    public boolean rotateLeft() {
        field.updateFigure(figure, y, x, Cell.EMPTY);
        figure.rotateLeft();
        if (!canRotate()) {
            figure.rotateRight();
            field.updateFigure(figure, y, x, figure.getCell());
            return false;
        }
        field.updateFigure(figure, y, x, figure.getCell());
        notifyObservers(Update.FIELD);
        return true;
    }
    public void setDelay(int delay) {
        timer.setDelay(delay);
    }
    public void setInitialDelay() {
        timer.setDelay(delay - score/1000*25);
    }
    public void clear() {
        timer.stop();
        figure = getNewFigure();
        newFigure = getNewFigure();
        x = field.getWidth()/2;
        y = field.getHeight() - 1;
        field.clear();
        score = 0;
        notifyObservers(Update.FIELD);
    }
    public void saveRecords() {
        records.saveRecords();
    }
    public List getRecords() {
        return records.getRecords();
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
                if (!canMove) {
                    break;
                }
            }
            if (canMove) {
                y--;
                return true;
            }
        }
        return false;
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
    private List<Observer> observers;
    private int delay = 500;
    private Timer timer;
    private Scores records;
}
