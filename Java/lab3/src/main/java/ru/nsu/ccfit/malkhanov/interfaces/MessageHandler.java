package ru.nsu.ccfit.malkhanov.interfaces;

import ru.nsu.ccfit.malkhanov.model.message.*;

import java.io.IOException;

public interface MessageHandler {
    void handle(MessageFromClient message);
    void handle(MessageFromServer message);
    void handle(Logout message) throws IOException;
    void handle(RequestToListUsers message);
    void handle(LogoutResponse message);
}
