package ru.nsu.ccfit.malkhanov;

import ru.nsu.ccfit.malkhanov.model.client.Client;
import ru.nsu.ccfit.malkhanov.view.ChatInput;
import ru.nsu.ccfit.malkhanov.view.ChatOutput;
import ru.nsu.ccfit.malkhanov.controller.Controller;

public class App {
    public static void main(String[] args) {
        ChatInput in = new ChatInput();
        Client client = new Client(in);
        ChatOutput out = new ChatOutput(client);
        client.registerObserver(out);

        Controller it = new Controller(client, in, out);
        client.registerObserver(it);
    }
}
