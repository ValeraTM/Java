package model.figures;

import model.Cell;

public class Bomb extends Shape {
    private final static int[] SIZE = {1, 1};
    private final static String[] FORMS = {"*"};
    private final static int RADIUS = 3;

    public Bomb(int idx, Cell color) {
        super.cell = color;
        super.width = SIZE[0];
        super.height = SIZE[1];
    }

    @Override
    public boolean getEnd(int x, int y) {
        cell = Cell.EMPTY;
        return (x >= 0 && x < RADIUS && y >= 0 && y < RADIUS);
    }

    @Override
    public int getEndY() {
        return RADIUS;
    }

    @Override
    public int getEndX() {
        return RADIUS;
    }

    @Override
    public int getOffsetX() {
        return -1;
    }

    @Override
    public int getOffsetY() {
        return -1;
    }

    @Override
    public boolean isEmpty(int x, int y) {
        return (FORMS[0].charAt(y*width + x) == EMPTY);
    }

    @Override
    public void rotateRight() {
    }

    @Override
    public void rotateLeft() {
    }
}
