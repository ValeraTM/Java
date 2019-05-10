package observer;

public interface Observer {
    default void updateField() {

    }
    default void updateNextFigure() {

    }
    default void updateScore() {

    }
    default void endGame() {

    }
}
