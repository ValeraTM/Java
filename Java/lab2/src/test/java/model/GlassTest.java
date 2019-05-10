package model;

import model.figures.I;
import model.figures.Shape;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.awt.*;

public class GlassTest {
    private Glass field = new Glass(24, 10);

    @Before
    public void setUp() throws Exception {
        for (int i = 0; i < field.getWidth(); i++) {
            field.setFigure(new I(1, new Cell(Color.RED)), i, 5);
        }
    }

    @Test
    public void destroyFilledRow() {
        Assert.assertEquals(4, field.destroyFilledRow());
        Assert.assertEquals(0, field.destroyFilledRow());
    }

    @Test
    public void updateFigure() {
        Shape figure = new I(1, new Cell(Color.RED));
        field.updateFigure(figure, 5, 0, figure.getCell());
        field.updateFigure(figure, 5, 0, Cell.EMPTY);
        field.updateFigure(figure, 5, 1, figure.getCell());
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (!figure.isEmpty(j, i)) {
                    Assert.assertTrue(field.isEmpty(j, 5 - i));
                }
            }
        }
        for (int i = 0; i < figure.getHeight(); i++) {
            for (int j = 0; j < figure.getWidth(); j++) {
                if (figure.isEmpty(j, i)) {
                    Assert.assertTrue(field.isEmpty(j + 1, 5 - i));
                }
                else {
                    Assert.assertFalse(field.isEmpty(j + 1, 5 - i));
                }
            }
        }
    }

    @Test
    public void clear() {
        field.clear();
        for (int i = 0; i < field.getHeight(); i++) {
            for (int j = 0; j < field.getWidth(); j++) {
                Assert.assertTrue(field.isEmpty(j, i));
            }
        }
    }
}