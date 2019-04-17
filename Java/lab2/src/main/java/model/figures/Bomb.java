package model.figures;

import model.Glass;

import java.awt.Color;

public class Bomb extends Shape {
    private final static int[] SIZE = {3, 3};
    private final static String[] FORMS = {"____*____"};
    private final static String[] ACTION = {"*********"};
    private final static int RADIUS = 3;

    public Bomb(int idxForm, Color color) {
        super.color = color;
        super.width = SIZE[0];
        super.height = SIZE[1];
    }

    @Override
    public boolean getEnd(int x, int y) {
        color = Glass.EMPTY;
        return ACTION[0].charAt(y*RADIUS + x) == BUSY;
    }

    @Override
    public boolean isEmpty(int x, int y) {
        return (FORMS[0].charAt(y*width + x) == EMPTY);
    }

    /*@Override
    public Color getColor() {
        return Glass.EMPTY;
    }*/

    @Override
    public void rotateRight() {
    }

    @Override
    public void rotateLeft() {
    }
}
