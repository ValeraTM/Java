package model.figures;

import model.Cell;

public class J extends Shape {
    private final static int[] SIZE = {2, 3};
    private final static String[] FORMS = {"_*_***", "***__*", "***_*_", "*__***"};
    private int idxForm;

    public J(int idxForm, Cell color) {
        this.idxForm = idxForm % FORMS.length;
        super.cell = color;
        super.width = SIZE[this.idxForm % 2];
        super.height = SIZE[(this.idxForm + 1) % 2];
    }

    @Override
    public boolean getEnd(int x, int y) {
        return FORMS[idxForm].charAt(y*width + x) == BUSY;
    }

    @Override
    public boolean isEmpty(int x, int y) {
        return (FORMS[idxForm].charAt(y*width + x) == EMPTY);
    }

    @Override
    public void rotateRight() {
        idxForm = (idxForm + FORMS.length - 1) % FORMS.length;
        height = width;
        width = SIZE[idxForm % 2];
    }

    @Override
    public void rotateLeft() {
        idxForm = (idxForm + 1) % FORMS.length;
        height = width;
        width = SIZE[idxForm % 2];
    }
}
