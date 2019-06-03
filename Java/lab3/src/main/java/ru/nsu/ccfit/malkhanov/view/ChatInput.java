package ru.nsu.ccfit.malkhanov.view;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ru.nsu.ccfit.malkhanov.model.client.Client;
import ru.nsu.ccfit.malkhanov.model.message.MessageFromClient;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class ChatInput extends JPanel {
    private static final Logger L = LoggerFactory.getLogger(ChatInput.class);

    public ChatInput(Client client) { ;
        this.setLayout(new BorderLayout());
        this.add(message, BorderLayout.CENTER);
        this.add(logout, BorderLayout.EAST);

        message.setFont(new Font("Area", Font.ITALIC, 20));
        logout.setFont(new Font("Area", Font.ITALIC, 20));

        logout.addActionListener((ActionEvent event) -> {
            client.logout();
        });
        message.addActionListener((ActionEvent event) -> {
            client.send(new MessageFromClient(message.getText()));
            message.setText(null);
        });
    }

    public void logout() {
        logout.doClick();
    }

    private JTextField message = new JTextField();
    private JButton logout = new JButton("Log Out");
}