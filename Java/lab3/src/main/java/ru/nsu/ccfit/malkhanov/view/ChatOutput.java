package ru.nsu.ccfit.malkhanov.view;

import ru.nsu.ccfit.malkhanov.model.client.Client;
import ru.nsu.ccfit.malkhanov.interfaces.ChatObserver;
import ru.nsu.ccfit.malkhanov.model.message.MessageFromServer;

import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;

public class ChatOutput extends JPanel implements ChatObserver {
    public ChatOutput(Client client) {
        this.client = client;
        this.setLayout(new BorderLayout());

        JScrollPane scrollForView = new JScrollPane(view);
        scrollForView.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
        scrollForView.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);

        view.setEditable(false);

        JScrollPane scrollForList = new JScrollPane(listUsers);
        scrollForList.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
        scrollForList.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);

        scrollForList.setPreferredSize(new Dimension(150, 500));

        listUsers.setEditable(false);

        this.add(scrollForView, BorderLayout.CENTER);
        this.add(scrollForList, BorderLayout.EAST);

        StyledDocument doc = new DefaultStyledDocument() {
            @Override
            public void insertString(int offs, String str, AttributeSet a) throws BadLocationException {
                if (str == null) {
                    return;
                }
                int size = 38;
                StringBuilder it = new StringBuilder(str);
                int rows = str.length() / size;
                int idx = size;
                for (int i = 0; i < rows; i++) {
                    it.insert(idx, '\n');
                    idx += size + 1;
                }
                super.insertString(offs, it.toString(), a);
            }
        };
        view.setDocument(doc);

        StyleConstants.setForeground(forName, Color.BLACK);
        StyleConstants.setBold(forName, true);
        StyleConstants.setFontSize(forName, 22);

        StyleConstants.setForeground(forDate, Color.GRAY);
        StyleConstants.setItalic(forDate, true);
        StyleConstants.setFontSize(forDate, 18);

        StyleConstants.setForeground(forMessage, Color.BLACK);
        StyleConstants.setFontSize(forMessage, 20);

        StyleConstants.setFontSize(forSpace, 3);
    }

    @Override
    public void updateListMessage() {
        MessageFromServer message = client.getHistory().getLast();
        StyledDocument doc = view.getStyledDocument();

        try {
            if (message.getName() == null) {
                doc.insertString(doc.getLength(), "\n", forSpace);
                doc.insertString(doc.getLength(), "\n", forSpace);
                doc.insertString(doc.getLength(), "              " + message.getTime() + "  ", forDate);
                doc.insertString(doc.getLength(), message.getMessage() + '\n', forDate);
                doc.insertString(doc.getLength(), "\n", forSpace);
                doc.insertString(doc.getLength(), "\n", forSpace);
                return;
            }
            doc.insertString(doc.getLength(), message.getName(), forName);
            doc.insertString(doc.getLength(), " " + message.getTime() + '\n', forDate);
            doc.insertString(doc.getLength(), "\n", forSpace);
            doc.insertString(doc.getLength(), message.getMessage() + '\n', forMessage);
            doc.insertString(doc.getLength(), "\n", forSpace);
            doc.insertString(doc.getLength(), "\n", forSpace);
        }
        catch (BadLocationException ex) {
            ex.printStackTrace();
        }
        view.setCaretPosition(view.getDocument().getLength());
    }

    @Override
    public void updateListUsers() {
        listUsers.setText(null);
        StyledDocument doc = listUsers.getStyledDocument();
        for (Object name : client.getListUsers()) {
            try {
                if (name.equals(client.getUserName())) {
                    doc.insertString(doc.getLength(), name + "\n", forName);
                }
                else {
                    doc.insertString(doc.getLength(), name + "\n", forMessage);
                }
                doc.insertString(doc.getLength(), "\n", forSpace);
            }
            catch (BadLocationException ex) {
                ex.printStackTrace();
            }
        }
    }

    @Override
    public void logOut() {
        view.setText(null);
        listUsers.setText(null);
    }

    private SimpleAttributeSet forName = new SimpleAttributeSet();
    private SimpleAttributeSet forDate = new SimpleAttributeSet();
    private SimpleAttributeSet forMessage = new SimpleAttributeSet();
    private SimpleAttributeSet forSpace = new SimpleAttributeSet();

    private Client client;
    private JTextPane listUsers = new JTextPane();
    private JTextPane view = new JTextPane();
}
