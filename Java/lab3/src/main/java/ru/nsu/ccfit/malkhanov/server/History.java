package ru.nsu.ccfit.malkhanov.server;

import ru.nsu.ccfit.malkhanov.model.message.Message;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Deque;
import java.util.concurrent.ConcurrentLinkedDeque;

public class History {
    private static final Logger L = LoggerFactory.getLogger(History.class);

    public History(int capacity) {
        this.capacity = capacity;
        story = new ConcurrentLinkedDeque<>();
        L.info("Created History with capacity {}", capacity);
    }

    public void addHistory(Message message) {
        if (story.size() == capacity) {
            story.removeFirst();
            L.info("Removed old record");
        }
        story.addLast(message);
        L.info("Added new record");
    }

    public Deque<Message> getHistory() {
        return story;
    }

    private int capacity;
    private ConcurrentLinkedDeque<Message> story;
}
