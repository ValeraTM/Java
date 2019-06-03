package ru.nsu.ccfit.malkhanov.controller;

import ru.nsu.ccfit.malkhanov.model.client.Client;
import ru.nsu.ccfit.malkhanov.interfaces.ChatObserver;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ru.nsu.ccfit.malkhanov.view.Chat;
import ru.nsu.ccfit.malkhanov.view.ChatInput;
import ru.nsu.ccfit.malkhanov.view.ChatOutput;

import javax.swing.*;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

public class Controller extends JFrame implements ActionListener, ChatObserver {
    private static final Logger L = LoggerFactory.getLogger(Controller.class);

    private JTextField inetAddress;
    private JPanel panel;
    private JButton join;
    private JTextField portNumber;
    private JButton exit;
    private JTextField nickName;

    public Controller(Client client, ChatInput input, ChatOutput output) {
        this.client = client;
        this.chat = new Chat(input, output);
        this.input = input;

        this.setLocationRelativeTo(null);
        this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        this.setTitle("Chat");

        JPanel main = (JPanel) this.getContentPane();
        main.setLayout(new BoxLayout(main, BoxLayout.Y_AXIS));
        main.add(panel);
        panel.setVisible(true);
        main.add(chat);
        chat.setVisible(false);

        inetAddress.setDocument(new PlainDocument() {
            @Override
            public void insertString(int offs, String str, AttributeSet a) throws BadLocationException {
                if (Character.isSpaceChar(str.charAt(0))) {
                    return;
                }
                super.insertString(offs, str, a);
            }
        });

        portNumber.setDocument(new PlainDocument() {
            @Override
            public void insertString(int offs, String str, AttributeSet a) throws BadLocationException {
                if (!Character.isDigit(str.charAt(0))) {
                    return;
                }
                if (str.length() + offs > 5) {
                    return;
                }
                super.insertString(offs, str, a);
            }
        });

        nickName.setDocument(new PlainDocument() {
            @Override
            public void insertString(int offs, String str, AttributeSet a) throws BadLocationException {
                if (Character.isSpaceChar(str.charAt(0))) {
                    return;
                }
                if (str.length() + offs > 10) {
                    return;
                }
                super.insertString(offs, str, a);
            }
        });

        exit.addActionListener((ActionEvent e) -> dispose());
        join.addActionListener(this);

        this.pack();
        this.setMinimumSize(new Dimension(this.getWidth() + 450, this.getHeight() + 200));
        this.setResizable(false);
        this.setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent event) {
        try {
            client.joinServer(inetAddress.getText(), Integer.parseInt(portNumber.getText()), nickName.getText());
            chat.setVisible(true);
            panel.setVisible(false);
        }
        catch (IOException ex) {
            JOptionPane.showMessageDialog(this, ex.getMessage());
            L.error("Join", ex);
        }
    }

    @Override
    public void logOut() {
        chat.setVisible(false);
        panel.setVisible(true);
    }

    @Override
    public void dispose() {
        if (chat.isVisible()) {
            input.logout();
        }
        super.dispose();
    }

    private ChatInput input;
    private JPanel chat;
    private Client client;

    public static void main(String[] args) {
        Client client = new Client();
        ChatInput in = new ChatInput(client);
        ChatOutput out = new ChatOutput(client);
        client.registerObserver(out);

        Controller it = new Controller(client, in, out);
        client.registerObserver(it);
    }
}
