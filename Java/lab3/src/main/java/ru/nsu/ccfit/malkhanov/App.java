package ru.nsu.ccfit.malkhanov;

import ru.nsu.ccfit.malkhanov.controller.Controller;
import ru.nsu.ccfit.malkhanov.model.client.Client;
import ru.nsu.ccfit.malkhanov.view.ChatInput;
import ru.nsu.ccfit.malkhanov.view.ChatOutput;

public class App {
    public static void main(String[] args) {
        Client client = new Client();
        ChatInput in = new ChatInput(client);
        ChatOutput out = new ChatOutput(client);
        client.registerObserver(out);

        Controller it = new Controller(client, in, out);
        client.registerObserver(it);
    }
}
