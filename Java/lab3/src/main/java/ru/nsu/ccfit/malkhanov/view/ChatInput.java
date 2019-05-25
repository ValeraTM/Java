package ru.nsu.ccfit.malkhanov.view;

import ru.nsu.ccfit.malkhanov.interfaces.ChatInputReader;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.concurrent.atomic.AtomicBoolean;

public class ChatInput extends JPanel implements ChatInputReader {
    private static final Logger L = LoggerFactory.getLogger(ChatInput.class);

    public ChatInput() {
        this.setLayout(new BorderLayout());
        this.add(message, BorderLayout.CENTER);
        this.add(logout, BorderLayout.EAST);

        message.setFont(new Font("Area", Font.ITALIC, 20));
        logout.setFont(new Font("Area", Font.ITALIC, 20));

        logout.addActionListener((ActionEvent event) -> {
            isInterrupted.set(true);
            message.setText(null);
            synchronized (lock) {
                lock.notifyAll();
            }
        });
        message.addActionListener((ActionEvent event) -> {
            synchronized (lock) {
                lock.notifyAll();
            }
        });
    }

    @Override
    public String readMessage() throws InterruptedException {
        synchronized (lock) {
            lock.wait();
        }
        if (isInterrupted.get()) {
            isInterrupted.set(false);
            Thread.currentThread().interrupt();
            return null;
        }
        String text = message.getText();
        message.setText(null);
        return text;
    }

    public void logout() {
        logout.doClick();
    }

    private AtomicBoolean isInterrupted = new AtomicBoolean();
    private final Object lock = new Object();
    private JTextField message = new JTextField();
    private JButton logout = new JButton("Log Out");
}