package model;

import java.awt.Color;

public class Cell {
    public static final Cell EMPTY = new Cell(Color.BLACK, true);

    public Cell(Color color, boolean isEmpty) {
        this.color = color;
        this.isEmpty = isEmpty;
    }
    public Cell(int rgb) {
        color = new Color(rgb);
        isEmpty = false;
    }
    public Cell(Color color) {
        this.color = color;
        this.isEmpty = false;
    }

    public boolean isEmpty() {
        return isEmpty;
    }
    public Color getColor() {
        return color;
    }

    private final Color color;
    private final boolean isEmpty;
}