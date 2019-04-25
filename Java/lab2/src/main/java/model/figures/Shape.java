package model.figures;

import model.Cell;

import java.awt.Color;

public abstract class Shape {
    static final char EMPTY = '_';
    static final char BUSY = '*';

    Cell color;
    int width;
    int height;

    public abstract boolean getEnd(int x, int y);
    public abstract void rotateLeft();
    public abstract void rotateRight();
    public final int getHeight() {
        return height;
    }
    public final int getWidth() {
        return width;
    }
    public Color getColor() {
        return color.getColor();
    }
    public Cell getCell() {
        return color;
    }
    public abstract boolean isEmpty(int x, int y);
}
