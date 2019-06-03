package ru.nsu.ccfit.malkhanov.model.message;

import ru.nsu.ccfit.malkhanov.interfaces.MessageHandler;

public class ConnectionVerification extends Message {
    @Override
    public void visit(MessageHandler handler) {
        handler.handle(this);
    }
}
