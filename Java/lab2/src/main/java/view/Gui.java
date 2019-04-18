package view;

import model.Glass;
import model.figures.Shape;

public interface Gui {
    void showNextFigure(Shape figure);
    void repaintField(Glass field);
    void clearFigure(Shape figure, int x, int y);
    void paintFigure(Shape figure, int x, int y);
    void setScores(int newScores);
    void saveRecord(int score);
}
