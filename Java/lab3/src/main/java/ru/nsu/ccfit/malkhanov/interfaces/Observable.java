package ru.nsu.ccfit.malkhanov.interfaces;

import java.util.function.Consumer;

public interface Observable {
    void registerObserver(ChatObserver o);
    void removeObserver(ChatObserver o);
    void notifyObservers(Consumer<ChatObserver> it);
}