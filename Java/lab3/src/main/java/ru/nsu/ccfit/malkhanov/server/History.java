package ru.nsu.ccfit.malkhanov.server;

import ru.nsu.ccfit.malkhanov.model.message.Message;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.List;

public class History {
    private static final Logger L = LoggerFactory.getLogger(History.class);

    public History(int capacity) {
        this.capacity = capacity;
        story = new ArrayList<>(capacity);
        L.info("Created History with capacity {}", capacity);
    }

    public void addHistory(Message message) {
        if (story.size() == capacity) {
            story.remove(0);
            L.info("Removed old record");
        }
        story.add(message);
        L.info("Added new record");
    }

    public List<Message> getHistory() {
        return story;
    }

    private int capacity;
    private ArrayList<Message> story;
}
