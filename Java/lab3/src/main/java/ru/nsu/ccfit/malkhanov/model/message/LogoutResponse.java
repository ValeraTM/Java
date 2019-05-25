package ru.nsu.ccfit.malkhanov.model.message;

import ru.nsu.ccfit.malkhanov.interfaces.MessageHandler;

import java.io.IOException;

public class LogoutResponse extends Message {
    @Override
    public void visit(MessageHandler handler) throws IOException {
        handler.handle(this);
    }
}
