package observer;

public interface Observable {
    enum Update {
        FIELD,
        NEXT_FIGURE,
        SCORE,
        RECORD
    }

    void registerObserver(Observer o);
    void removeObserver(Observer o);
    void notifyObservers(Update it);
}
