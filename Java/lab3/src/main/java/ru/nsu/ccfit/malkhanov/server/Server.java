package ru.nsu.ccfit.malkhanov.server;

import ru.nsu.ccfit.malkhanov.interfaces.ChatService;
import ru.nsu.ccfit.malkhanov.model.message.Message;
import ru.nsu.ccfit.malkhanov.model.message.MessageFromClient;
import ru.nsu.ccfit.malkhanov.model.message.MessageFromServer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ru.nsu.ccfit.malkhanov.model.message.UsersList;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;

public class Server implements Runnable, AutoCloseable, ChatService {
    private static final Logger L = LoggerFactory.getLogger(Server.class);

    private class BroadcastTransmitter implements Runnable {
        @Override
        public void run() {
            try {
                while (true) {
                    Message msg = queue.take();
                    for (ClientHandler it : clients.values()) {
                        it.send(msg);
                    }
                }
            }
            catch (InterruptedException ex) {
                L.info("Broadcast Transmitter interrupted");
            }
        }
    }

    public Server() throws IOException {
        this(0);
    }

    public Server(int port) throws IOException {
        server = new ServerSocket(port);
        L.info("Server port {} {}", server.getInetAddress().getHostName(), server.getLocalPort());
        sender = new Thread(new BroadcastTransmitter());
        sender.start();
    }

    @Override
    public void run() {
        while (true) {
            try {
                Socket socket = server.accept();
                new ClientHandler(socket, this);
            }
            catch (IOException ex) {
                L.error("Server", ex);
            }
        }
    }

    @Override
    public void disconnectClient(String name) {
        if (clients.remove(name) == null) {
            return;
        }
        L.info("Disconnected client: {}", name);
        queue.add(new UsersList(this.getParticipantList()));
    }

    @Override
    public void send(MessageFromClient message, String name) {
        MessageFromServer msg = new MessageFromServer(name, message.getMessage());
        history.addHistory(msg);
        queue.add(msg);
    }

    @Override
    public void signUp(String name, ClientHandler server) {
        clients.put(name, server);
        L.info("Registered new client: {}", name);
        for (Message it : history.getHistory()) {
            server.send(it);
        }
        queue.add(new UsersList(this.getParticipantList()));
    }

    @Override
    public List getParticipantList() {
        return Arrays.asList(clients.keySet().toArray());
    }

    @Override
    public void close() throws IOException {
        sender.interrupt();
        server.close();
    }

    private LinkedBlockingQueue<Message> queue = new LinkedBlockingQueue<>();
    private History history = new History(10);
    private ServerSocket server;
    private Map<String, ClientHandler> clients = new ConcurrentHashMap<>();
    private Thread sender;

    public static void main(String[] args) {
        try (Server server = new Server()) {
            server.run();
        }
        catch (IOException ex) {
            L.error("ServerSocket", ex);
        }
    }
}