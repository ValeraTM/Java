package observer;

import model.Glass;
import model.figures.Shape;

public interface Observer {
    void updateField(Glass field);
    void updateNextFigure(Shape figure);
    void updateScore(int score);
    String getRecord();
}
