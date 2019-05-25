package ru.nsu.ccfit.malkhanov.server;

import ru.nsu.ccfit.malkhanov.model.message.Message;
import ru.nsu.ccfit.malkhanov.model.message.MessageFromServer;
import ru.nsu.ccfit.malkhanov.model.message.RequestToListUsers;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;

public class ServerFactory implements Runnable, AutoCloseable {
    private static final Logger L = LoggerFactory.getLogger(ServerFactory.class);

    private class BroadcastTransmitter implements Runnable {
        @Override
        public void run() {
            while (true) {
                if (Thread.currentThread().isInterrupted()) {
                    break;
                }
                Message msg = queue.poll();
                if (msg == null) {
                    try {
                        synchronized (lock) {
                            lock.wait();
                        }
                    }
                    catch (InterruptedException ex) {
                        L.warn("Broadcast Transmitter interrupted", ex);
                    }
                    continue;
                }
                for (Server it : clients.values()) {
                    it.send(msg);
                }
            }
        }
    }

    public ServerFactory() throws IOException {
        this(0);
    }

    public ServerFactory(int port) throws IOException {
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
                new Server(socket, this);
            }
            catch (IOException ex) {
                L.error("Server", ex);
            }
        }
    }

    public void disconnectClient(String name) {
        clients.remove(name);
        L.info("Disconnected client: {}", name);
        this.sendAll(new MessageFromServer(null, name + " left to chat"));
        this.sendAll(new RequestToListUsers(this.getParticipantList()));
    }
    public void sendAll(Message message) {
        if (message instanceof MessageFromServer) {
            history.addHistory(message);
        }
        queue.add(message);
        synchronized (lock) {
            lock.notifyAll();
        }
    }
    public List getParticipantList() {
        return Arrays.asList(clients.keySet().toArray());
    }
    public List<Message> getHistory() {
        return history.getHistory();
    }

    public void signUp(String name, Server server) {
        clients.put(name, server);
        L.info("Registered new client: {}", name);
        for (Message it : this.getHistory()) {
            server.send(it);
        }
        this.sendAll(new MessageFromServer(null, name + " joined to chat"));
        this.sendAll(new RequestToListUsers(this.getParticipantList()));
    }

    @Override
    public void close() throws IOException {
        sender.interrupt();
        server.close();
    }

    private final Object lock = new Object();
    private Queue<Message> queue = new LinkedBlockingQueue<>();
    private History history = new History(10);
    private ServerSocket server;
    private Map<String, Server> clients = new ConcurrentHashMap<>();
    private Thread sender;

    public static void main(String[] args) {
        try (ServerFactory server = new ServerFactory()) {
            server.run();
        }
        catch (IOException ex) {
            L.error("ServerSocket", ex);
        }
    }
}