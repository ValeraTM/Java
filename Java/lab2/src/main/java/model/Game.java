package model;

import model.figures.Shape;

import java.io.InputStream;

public class Game {
    public Game(int width, int height, InputStream config) throws Exception {
        factory = new Factory(config);
        field = new Glass(height, width);
        figure = getNewFigure();
        newFigure = getNewFigure();
        x = width/2;
        y = height - 1;
    }

    public boolean moveLeft() {
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
                return true;
            }
        }
        return false;
    }
    public boolean moveRight() {
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
                return true;
            }
        }
        return false;
    }
    public boolean moveDown() {
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
        figure.rotateLeft();
        if (!canRotate()) {
            figure.rotateRight();
            return false;
        }
        return true;
    }
    public boolean fall() {
        if (y > field.getBorder()) {
            return false;
        }
        field.setFigure(figure, x, y);
        return true;
    }
    public boolean checkFilledRow() {
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

    public int getScore() {
        return score;
    }
    public Glass getField() {
        return field;
    }
    public Shape getNextFigure() {
        return newFigure;
    }
    public Shape getFigure() {
        return figure;
    }
    public int getX() {
        return x;
    }
    public int getY() {
        return y;
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
}
