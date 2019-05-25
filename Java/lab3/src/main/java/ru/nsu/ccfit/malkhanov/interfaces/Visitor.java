package ru.nsu.ccfit.malkhanov.interfaces;

import java.io.IOException;

public interface Visitor {
    void visit(MessageHandler handler) throws IOException;
}
