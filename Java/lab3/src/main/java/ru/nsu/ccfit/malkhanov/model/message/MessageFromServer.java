package ru.nsu.ccfit.malkhanov.model.message;

import ru.nsu.ccfit.malkhanov.interfaces.MessageHandler;

import java.text.SimpleDateFormat;
import java.util.Date;

public class MessageFromServer extends Message {
    public MessageFromServer(String name, String message) {
        this.message = message;
        this.name = name;
        clock = date.format(new Date());
    }

    public String getTime() {
        return clock;
    }

    public String getName() {
        return name;
    }

    public String getMessage() {
        return message;
    }

    @Override
    public void visit(MessageHandler handler) {
        handler.handle(this);
    }

    private final String clock;
    private final String name;
    private final String message;
    private final static SimpleDateFormat date = new SimpleDateFormat("HH:mm:ss");
}