package ru.nsu.ccfit.malkhanov.interfaces;

import ru.nsu.ccfit.malkhanov.model.message.*;

public interface MessageHandler {
    void handle(MessageFromClient message);
    void handle(MessageFromServer message);
    void handle(Logout message);
    void handle(UsersList message);
    void handle(LogoutResponse message);
    void handle(ConnectionVerification message);
}
