package view;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class Scores extends JPanel {
    public Scores() {
        setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
    }

    public void setRecords(List records) {
        removeAll();
        Font font = new Font("Scores", Font.ITALIC, 30);

        add(Box.createVerticalGlue());
        records.add(0, "TOP 10 PLAYERS");
        for (Object it : records) {
            JLabel label = new JLabel(it.toString());
            label.setForeground(Color.WHITE);
            label.setFont(font);
            label.setAlignmentX(Component.CENTER_ALIGNMENT);
            add(label);
            add(Box.createVerticalStrut(10));
        }
        add(Box.createVerticalGlue());
        setVisible(true);
    }
}
