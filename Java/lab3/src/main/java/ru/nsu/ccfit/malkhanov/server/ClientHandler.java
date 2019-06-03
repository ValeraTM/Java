package ru.nsu.ccfit.malkhanov.server;

import ru.nsu.ccfit.malkhanov.interfaces.ChatService;
import ru.nsu.ccfit.malkhanov.interfaces.MessageHandler;
import ru.nsu.ccfit.malkhanov.model.message.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

public class ClientHandler implements AutoCloseable {
    private static final Logger L = LoggerFactory.getLogger(ClientHandler.class);

    private class MessageServerHandler implements MessageHandler {
        @Override
        public void handle(LogoutResponse message) {

        }

        @Override
        public void handle(UsersList message) {
            ClientHandler.this.send(new UsersList(chat.getParticipantList()));
        }

        @Override
        public void handle(Logout message) {
            chat.disconnectClient(nickName);
            queue.add(new LogoutResponse());
            recv.interrupt();
            send.interrupt();
        }

        @Override
        public void handle(MessageFromServer message) {

        }
        @Override
        public void handle(MessageFromClient message) {
            chat.send(message, nickName);
        }

        @Override
        public void handle(ConnectionVerification message) {
            checkConnection.cancel();
        }
    }
    private class ServerTransmitter implements Runnable {
        @Override
        public void run() {
            Timer timer = new Timer();
            L.info("Server Transmitter for {} run", nickName);
            try {
                while (true) {
                    Message message = queue.poll(30, TimeUnit.MINUTES);
                    if (message == null) {
                        out.writeObject(new ConnectionVerification());
                        L.info("{} sent {}", nickName, ConnectionVerification.class.getName());
                        checkConnection = new TimerTask() {
                            @Override
                            public void run() {
                                L.info("{} is not responding", nickName);
                                send.interrupt();
                                recv.interrupt();
                            }
                        };
                        timer.schedule(checkConnection, 1000 * 60 * 2);
                        continue;
                    }
                    out.writeObject(message);
                    L.info("{} sent {}", nickName, message.getClass().getName());
                }
            }
            catch (IOException ex) {
                L.error("Server Transmitter for {}", nickName, ex);
            }
            catch (InterruptedException ex) {
                L.info("Server Transmitter for {} interrupted", nickName);
            }

            try {
                chat.send(new MessageFromClient(nickName + " left to chat"), null);
                ClientHandler.this.close();
            }
            catch (IOException e) {
                L.error("LogoutResponse", e);
            }

            L.info("Server Transmitter for {} died", nickName);
        }

    }
    private class ServerRecipient implements Runnable {
        @Override
        public void run() {
            try {
                Login login = (Login) in.readObject();
                nickName = login.getName();
                if (chat.getParticipantList().contains(nickName)) {
                    out.writeObject(LoginResponse.ALREADY_EXIST);
                    ClientHandler.this.close();
                    return;
                }
                else {
                    out.writeObject(LoginResponse.SUCCESS);
                    chat.signUp(nickName, ClientHandler.this);
                    chat.send(new MessageFromClient(nickName + " joined to chat"), null);
                    send.start();
                }

                L.info("Server Recipient for {} run", nickName);
                while (true) {
                    if (Thread.currentThread().isInterrupted()) {
                        L.info("Server Recipient for {} interrupted", nickName);
                        break;
                    }
                    Message msg = (Message) in.readObject();
                    L.info("{} Received {}", nickName, msg.getClass().getName());
                    msg.visit(visitor);
                }
            }
            catch (ClassCastException ex) {
                L.error("Sign Up failed", ex);
            }
            catch (IOException ex) {
                L.error("Server Recipient {}", nickName, ex);
            }
            catch (ClassNotFoundException ex) {
                L.error("Unexpected message", ex);
            }

            L.info("ServerRecipient for {} died", nickName);
        }
    }

    public ClientHandler(Socket socket, ChatService chat) throws IOException {
        this.socket = socket;
        this.chat = chat;
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

    private LinkedBlockingQueue<Message> queue = new LinkedBlockingQueue<>();
    private String nickName;
    private ChatService chat;
    private Socket socket;
    private ObjectOutputStream out;
    private ObjectInputStream in;
    private MessageHandler visitor = new MessageServerHandler();
    private Thread recv;
    private Thread send;
    private TimerTask checkConnection;
}
