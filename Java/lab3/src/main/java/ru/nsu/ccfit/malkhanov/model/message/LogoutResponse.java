package ru.nsu.ccfit.malkhanov.model.message;

import ru.nsu.ccfit.malkhanov.interfaces.MessageHandler;

public class LogoutResponse extends Message {
    @Override
    public void visit(MessageHandler handler) {
        handler.handle(this);
    }
}
