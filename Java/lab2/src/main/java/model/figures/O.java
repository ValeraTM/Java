package model.figures;

import model.Cell;

public class O extends Shape {
    private final static int[] SIZE = {2};
    private final static String[] FORMS = {"****"};

    public O(int idxForm, Cell color) {
        super.color = color;
        super.width = SIZE[0];
        super.height = SIZE[0];
    }

    @Override
    public boolean getEnd(int x, int y) {
        return FORMS[0].charAt(y*width + x) == BUSY;
    }

    @Override
    public boolean isEmpty(int x, int y) {
        return false;
    }

    @Override
    public void rotateRight() {
    }

    @Override
    public void rotateLeft() {
    }
}
