package model;

import model.figures.Shape;

import java.awt.Color;
import java.util.LinkedList;

public class Glass {
    public static final Color EMPTY = Color.BLACK;
    Glass(int height, int width) {
        this.height = height;
        this.border = height - 4;
        this.width = width;
        field = new LinkedList<>();
        for (int i = 0; i < height; i++) {
            field.add(new Color[width]);
            for (int j = 0; j < width; j++) {
                field.get(i)[j] = EMPTY;
            }
        }
    }
    void setFigure(Shape figure, int x, int y) {
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (figure.getEnd(j, i)) {
                    field.get(y - i)[x + j] = figure.getColor();
                }
            }
        }
    }
    int destroyFilledRow() {
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
        Color[] line = new Color[width];
        for (int i = 0; i < width; i++) {
            line[i] = EMPTY;
        }
        field.addLast(line);
    }

    public Color getColor(int x, int y) {
        return field.get(y)[x];
    }
    public boolean isEmpty(int x, int y) {
        return field.get(y)[x] == EMPTY;
    }
    public int getHeight() {
        return height;
    }
    public int getWidth() {
        return width;
    }
    int getBorder() {
        return border;
    }

    void clear() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                field.get(i)[j] = EMPTY;
            }
        }
    }

    private LinkedList<Color[]> field;
    private int height;
    private int border;
    private int width;
}
