package model;

import model.figures.Shape;
import observer.Observable;
import observer.Observer;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.swing.Timer;
import java.awt.event.ActionEvent;
import java.util.LinkedList;
import java.util.List;

public class Game implements Observable {
    private static final Logger logger = LogManager.getLogger(Game.class);

    public Game(int width, int height) {
        logger.info("Created Tetris " + height + "x" + width);
        try {
            factory = new Factory();
            logger.info("Created Factory");
        }
        catch (Exception ex) {
            logger.fatal("FactoryConfigNotFoundException", ex);
            System.exit(-1);
        }
        observers = new LinkedList<>();
        field = new Glass(height, width);
        x = width/2;
        y = height - 1;
        records = Scores.openScores("src/main/resources/HighScores.ser");
    }

    public void startGame() {
        logger.info("New Game");
        figure = getNewFigure();
        newFigure = getNewFigure();
        field.updateFigure(figure, y, x, figure.getCell());
        notifyObservers(Update.SCORE);
        notifyObservers(Update.FIELD);
        notifyObservers(Update.NEXT_FIGURE);

        timer = new Timer(delay, (ActionEvent event) -> {
            field.updateFigure(figure, y, x, Cell.EMPTY);
            if (!moveDown()) {
                if (!fall()) {
                    notifyObservers(Update.END_GAME);
                    clear();
                    return;
                }

                timer.setDelay(delay - score/1000*25);
                logger.debug("Current TimerDelay: " + timer.getDelay());

                if (checkFilledRow()) {
                    notifyObservers(Update.SCORE);
                }

                notifyObservers(Update.NEXT_FIGURE);
            }
            field.updateFigure(figure, y, x, figure.getCell());
            notifyObservers(Update.FIELD);
            logger.debug("Current position: " + x + " " + y);
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

        logger.debug("Current position: " + x + " " + y);
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

        logger.debug("Current position: " + x + " " + y);
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
        logger.debug("Turn Right");
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
        logger.debug("Turn Left");
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
        x = field.getWidth()/2;
        y = field.getHeight() - 1;
        field.clear();
        score = 0;
        notifyObservers(Update.FIELD);
        logger.info("End Game");
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
        logger.info("Destroyed " + count + " rows");
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
            Shape it = factory.createRandomProduct();
            logger.info("Created Shape: " + it.getClass().getName() + " " + it.getColor());
            return it;
        }
        catch (Exception ex) {
            logger.fatal("ShapeNotFoundException", ex);
            System.exit(1);
            return null;
        }
    }
    private boolean canRotate() {
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (x + j >= field.getWidth() || y - i < 0) {
                    return false;
                }
                if (!figure.isEmpty(j, i) && !field.isEmpty(x + j, y - i)) {
                    return false;
                }
            }
        }
        return true;
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
                    logger.info("Score Updated: " + score);
                    break;
                case END_GAME:
                    records.addNewRecord(it.getRecord(), score);
            }
        }
    }

    @Override
    public void removeObserver(Observer o) {
        observers.remove(o);
        logger.info("Removed observer: " + o.getClass().getName());
    }

    @Override
    public void registerObserver(Observer o) {
        observers.add(o);
        logger.info("Registered observer: " + o.getClass().getName());
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
