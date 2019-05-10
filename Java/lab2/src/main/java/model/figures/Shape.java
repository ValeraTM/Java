package model.figures;

import model.Cell;

import java.awt.Color;

public abstract class Shape {
    protected static final char EMPTY = '_';
    protected static final char BUSY = '*';

    protected Cell cell;
    protected int width;
    protected int height;

    public int getOffsetX() {
        return 0;
    }
    public int getOffsetY() {
        return 0;
    }
    public int getEndY() {
        return height;
    }
    public int getEndX() {
        return width;
    }
    public abstract boolean getEnd(int x, int y);
    public abstract void rotateLeft();
    public abstract void rotateRight();
    public final int getHeight() {
        return height;
    }
    public final int getWidth() {
        return width;
    }
    public final Color getColor() {
        return cell.getColor();
    }
    public final Cell getCell() {
        return cell;
    }
    public abstract boolean isEmpty(int x, int y);
}
