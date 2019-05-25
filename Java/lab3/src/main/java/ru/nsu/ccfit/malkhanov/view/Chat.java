package ru.nsu.ccfit.malkhanov.view;

import javax.swing.*;
import java.awt.*;

public class Chat extends JPanel {
    public Chat(JPanel in, JPanel out) {
        this.setLayout(new BorderLayout());

        this.add(out, BorderLayout.CENTER);
        this.add(in, BorderLayout.SOUTH);
        in.setAlignmentX(Component.CENTER_ALIGNMENT);
        out.setAlignmentX(Component.CENTER_ALIGNMENT);
    }
}