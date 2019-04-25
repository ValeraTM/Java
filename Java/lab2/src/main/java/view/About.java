package view;

import javax.swing.*;
import java.awt.*;

public class About extends JLabel {
    public About() {
        setText("<html><center>Created by<br />Malkhanov Valery</center></html>");
        setFont(new Font("About", Font.ITALIC, 30));
        setHorizontalAlignment(SwingConstants.CENTER);
        setForeground(Color.WHITE);
    }
}
