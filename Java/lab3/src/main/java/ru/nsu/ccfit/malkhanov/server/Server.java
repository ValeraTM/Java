package ru.nsu.ccfit.malkhanov.server;

import ru.nsu.ccfit.malkhanov.interfaces.MessageHandler;
import ru.nsu.ccfit.malkhanov.model.message.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class Server implements AutoCloseable {
    private class MessageServerHandler implements MessageHandler {
        @Override
        public void handle(LogoutResponse message) {

        }

        @Override
        public void handle(RequestToListUsers message) {
            Server.this.send(new RequestToListUsers(factory.getParticipantList()));
        }

        @Override
        public void handle(Logout message) throws IOException {
            Server.this.send(new LogoutResponse());
            factory.disconnectClient(nickName);
            recv.interrupt();
            send.interrupt();
        }

        @Override
        public void handle(MessageFromServer message) {

        }

        @Override
        public void handle(MessageFromClient message) {
            factory.sendAll(new MessageFromServer(nickName, message.getMessage()));
        }
    }

    private class ServerTransmitter implements Runnable {
        @Override
        public void run() {
            L.info("Server Transmitter for {} run", nickName);
            while (true) {
                Message message = queue.poll();
                if (message == null) {
                    if (Thread.currentThread().isInterrupted()) {
                        break;
                    }
                    continue;
                }
                try {
                    out.writeObject(message);
                    out.flush();
                    L.info("Sent {}", message.getClass().getName());
                }
                catch (IOException ex) {
                    L.error("ServerTransmitter for {}", nickName, ex);
                }
            }

            try {
                close();
            }
            catch (IOException ex) {
                L.error("Close Socket", ex);
            }

            L.info("ServerTransmitter for {} died", nickName);
        }
    }

    private class ServerRecipient implements Runnable {
        @Override
        public void run() {
            try {
                Login login = (Login) in.readObject();
                nickName = login.getName();
                if (factory.getParticipantList().contains(nickName)) {
                    out.writeObject(LoginResponse.ALREADY_EXIST);
                    close();
                    return;
                }
                else {
                    out.writeObject(LoginResponse.SUCCESS);
                    factory.signUp(nickName, Server.this);
                    send.start();
                }

                L.info("Server Recipient for {} run", nickName);
                while (true) {
                    if (Thread.currentThread().isInterrupted()) {
                        L.info("Server for {} interrupted", nickName);
                        break;
                    }
                    Message msg = (Message) in.readObject();
                    L.info("Received {}", msg.getClass().getName());
                    msg.visit(visitor);
                }
            }
            catch (ClassCastException ex) {
                L.error("Sign Up failed", ex);
            }
            catch (IOException ex) {
                L.error("Login", ex);
            }
            catch (ClassNotFoundException ex) {
                L.error("Unexpected message", ex);
            }

            L.info("ServerRecipient for {} died", nickName);
        }
    }

    private static final Logger L = LoggerFactory.getLogger(Server.class);

    public Server(Socket socket, ServerFactory factory) throws IOException {
        this.socket = socket;
        this.factory = factory;
        out = new ObjectOutputStream(socket.getOutputStream());
        in = new ObjectInputStream(socket.getInputStream());

        recv = new Thread(new ServerRecipient());
        send = new Thread(new ServerTransmitter());
        recv.start();
    }

    public void send(Message message) {
        queue.add(message);
    }

    @Override
    public void close() throws IOException {
        out.close();
        in.close();
        socket.close();
    }

    private Queue<Message> queue = new LinkedBlockingQueue<>();
    private String nickName;
    private ServerFactory factory;
    private Socket socket;
    private ObjectOutputStream out;
    private ObjectInputStream in;
    private MessageHandler visitor = new MessageServerHandler();
    private Thread recv;
    private Thread send;
}
