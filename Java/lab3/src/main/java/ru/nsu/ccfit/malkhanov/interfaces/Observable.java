package ru.nsu.ccfit.malkhanov.interfaces;

public interface Observable {
    enum Update {
        MESSAGE,
        LOGOUT,
        USER_LIST
    }
    void registerObserver(ChatObserver o);
    void removeObserver(ChatObserver o);
    void notifyObservers(Update it);
}
