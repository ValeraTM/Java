package ru.nsu.ccfit.malkhanov.model.message;

import ru.nsu.ccfit.malkhanov.interfaces.MessageHandler;

public class MessageFromClient extends Message {
    public MessageFromClient(String message) {
        this.message = message;
    }

    @Override
    public void visit(MessageHandler handler) {
        handler.handle(this);
    }

    public String getMessage() {
        return message;
    }

    private final String message;
}