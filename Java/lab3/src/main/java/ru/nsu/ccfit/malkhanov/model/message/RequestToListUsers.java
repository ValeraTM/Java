package ru.nsu.ccfit.malkhanov.model.message;

import ru.nsu.ccfit.malkhanov.interfaces.MessageHandler;

import java.util.List;

public class RequestToListUsers extends Message {
    public RequestToListUsers(List users) {
        this.users = users;
    }

    public List getUsers() {
        return users;
    }

    @Override
    public void visit(MessageHandler handler) {
        handler.handle(this);
    }

    private List users;
}
