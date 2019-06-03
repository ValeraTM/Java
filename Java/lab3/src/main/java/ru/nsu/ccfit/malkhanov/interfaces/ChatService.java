package ru.nsu.ccfit.malkhanov.interfaces;

import ru.nsu.ccfit.malkhanov.model.message.MessageFromClient;
import ru.nsu.ccfit.malkhanov.server.ClientHandler;

import java.util.List;

public interface ChatService {
    void disconnectClient(String name);
    void signUp(String name, ClientHandler client);
    void send(MessageFromClient message, String name);
    List getParticipantList();
}
