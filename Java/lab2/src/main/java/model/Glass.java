package model;

import model.figures.Shape;

import java.awt.Color;
import java.util.LinkedList;

public class Glass {
    public Glass(int height, int width) {
        this.height = height;
        this.border = height - 4;
        this.width = width;
        field = new LinkedList<>();
        for (int i = 0; i < height; i++) {
            field.add(new Cell[width]);
            for (int j = 0; j < width; j++) {
                field.get(i)[j] = Cell.EMPTY;
            }
        }
    }

    public void setFigure(Shape figure, int x, int y) {
        for (int i = 0; i < figure.getEndY(); i++) {
            for (int j = 0; j < figure.getEndX(); j++) {
                if (y - figure.getOffsetY() -i >= 0 && x + j + figure.getOffsetX() >= 0 && x + j + figure.getOffsetX() < width) {
                    if (figure.getEnd(j, i)) {
                        field.get(y - i - figure.getOffsetY())[x + j + figure.getOffsetX()] = figure.getCell();
                    }
                }
            }
        }
    }
    public int destroyFilledRow() {
        int count = 0;
        for (int j = 0; j < border; j++) {
            boolean isFilled = true;
            for (int i = 0; i < width; i++) {
                if (isEmpty(i, j)) {
                    isFilled = false;
                    break;
                }
            }
            if (isFilled) {
                destroyRow(j);
                j--;
                count++;
            }
        }
        return count;
    }

    private void destroyRow(int row) {
        field.remove(row);
        Cell[] line = new Cell[width];
        for (int i = 0; i < width; i++) {
            line[i] = Cell.EMPTY;
        }
        field.addLast(line);
    }

    public void updateFigure(Shape figure, int y, int x, Cell color) {
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (!figure.isEmpty(j, i)) {
                    field.get(y - i)[x + j] = color;
                }
            }
        }
    }

    public Color getColor(int x, int y) {
        return field.get(y)[x].getColor();
    }
    public boolean isEmpty(int x, int y) {
        return field.get(y)[x].isEmpty();
    }
    public int getHeight() {
        return height;
    }
    public int getWidth() {
        return width;
    }
    public int getBorder() {
        return border;
    }

    public void clear() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                field.get(i)[j] = Cell.EMPTY;
            }
        }
    }

    private LinkedList<Cell[]> field;
    private int height;
    private int border;
    private int width;
}
