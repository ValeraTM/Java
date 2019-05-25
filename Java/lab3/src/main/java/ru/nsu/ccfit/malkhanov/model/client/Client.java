package ru.nsu.ccfit.malkhanov.model.client;

import ru.nsu.ccfit.malkhanov.interfaces.MessageHandler;
import ru.nsu.ccfit.malkhanov.model.message.*;
import ru.nsu.ccfit.malkhanov.interfaces.ChatInputReader;
import ru.nsu.ccfit.malkhanov.interfaces.Observable;
import ru.nsu.ccfit.malkhanov.interfaces.ChatObserver;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.util.LinkedList;
import java.util.List;

public class Client implements AutoCloseable, Observable {
    private static final Logger L = LoggerFactory.getLogger(Client.class);

    private class MessageClientHandler implements MessageHandler {
        @Override
        public void handle(LogoutResponse message) {
            recv.interrupt();
        }

        @Override
        public void handle(RequestToListUsers message) {
            users = message.getUsers();
            notifyObservers(Update.USER_LIST);
        }

        @Override
        public void handle(Logout message) {

        }

        @Override
        public void handle(MessageFromServer message) {
            history.add(message);
            notifyObservers(Update.MESSAGE);
        }

        @Override
        public void handle(MessageFromClient message) {

        }
    }

    private class ClientTransmitter implements Runnable {
        @Override
        public void run() {
            L.info("Client Transmitter {} run", userName);
            while (true) {
                try {
                    MessageFromClient message = new MessageFromClient(reader.readMessage());
                    if (Thread.currentThread().isInterrupted()) {
                        L.info("Client Transmitter {} interrupted", userName);
                        break;
                    }
                    out.writeObject(message);
                    out.flush();
                    L.info("Sent {}: {}", message.getClass().getName(), message.getMessage());
                }
                catch (IOException ex) {
                    L.error("Client Transmitter {}", userName, ex);
                }
                catch (InterruptedException ex) {
                    L.info("Client Transmitter {} interrupted", userName, ex);
                    break;
                }
            }

            try {
                out.writeObject(new Logout());
                L.info("Sent {}", Logout.class.getName());
            }
            catch (IOException e) {
                L.error("Close", e);
            }
            L.info("Client Transmitter {} died", userName);
        }
    }

    private class ClientRecipient implements Runnable {
        @Override
        public void run() {
            L.info("Client Recipient {} run", userName);
            while (true) {
                if (Thread.currentThread().isInterrupted()) {
                    L.info("Client Recipient {} interrupted", userName);
                    break;
                }
                try {
                    Message msg = (Message) in.readObject();
                    L.info("Received {}", msg.getClass().getName());
                    msg.visit(visitor);
                }
                catch (IOException ex) {
                    L.error("Client Recipient {}", userName, ex);
                }
                catch (ClassNotFoundException ex) {
                    L.error("Unexpected message", ex);
                }
            }

            try {
                Client.this.close();
            }
            catch (IOException ex) {
                L.error("Close Client", ex);
            }
            L.info("Client Recipient {} died", userName);
        }
    }

    public Client(ChatInputReader reader) {
        this.reader = reader;
    }

    public void joinServer(String hostName, int port, String name) throws IOException {
        socket = new Socket(InetAddress.getByName(hostName), port);
        in = new ObjectInputStream(socket.getInputStream());
        out = new ObjectOutputStream(socket.getOutputStream());
        userName = name;
        out.writeObject(new Login(name));
        try {
            if (in.readObject() == LoginResponse.ALREADY_EXIST) {
                throw new IOException("Sorry, this name already exist");
            }
        }
        catch (ClassNotFoundException ex) {
            L.error("Unexpected message", ex);
        }

        history = new LinkedList<>();
        recv = new Thread(new ClientRecipient());
        send = new Thread(new ClientTransmitter());
        recv.start();
        send.start();
    }

    @Override
    public void registerObserver(ChatObserver o) {
        observers.add(o);
        L.info("Registered observer: {}", o.getClass().getName());
    }

    @Override
    public void removeObserver(ChatObserver o) {
        observers.remove(o);
        L.info("Removed observer: {}", o.getClass().getName());
    }

    @Override
    public void notifyObservers(Update mark) {
        for (ChatObserver it : observers) {
            switch (mark) {
                case LOGOUT:
                    it.logOut();
                    break;
                case MESSAGE:
                    it.updateListMessage();
                    break;
                case USER_LIST:
                    it.updateListUsers();
            }
        }
    }

    @Override
    public void close() throws IOException {
        in.close();
        out.close();
        socket.close();
        notifyObservers(Update.LOGOUT);
    }

    public LinkedList<MessageFromServer> getHistory() {
        return history;
    }
    public List getListUsers() {
        return users;
    }
    public String getUserName() {
        return userName;
    }

    private String userName;
    private List<ChatObserver> observers = new LinkedList<>();
    private LinkedList<MessageFromServer> history;
    private List users;

    private ChatInputReader reader;
    private Socket socket;
    private ObjectInputStream in;
    private ObjectOutputStream out;
    private Thread recv;
    private Thread send;
    private final MessageHandler visitor = new MessageClientHandler();
}
