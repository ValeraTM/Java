package observer;

public interface Observable {
    enum Update {
        FIELD,
        NEXT_FIGURE,
        SCORE,
        END_GAME
    }

    void registerObserver(Observer o);
    void removeObserver(Observer o);
    void notifyObservers(Update it);
}
