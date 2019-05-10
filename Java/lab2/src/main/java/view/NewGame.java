package view;

import ch.qos.logback.classic.Logger;
import org.slf4j.LoggerFactory;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.io.InputStream;

public class NewGame extends JPanel {
    private final static Logger logger = (Logger)LoggerFactory.getLogger(NewGame.class);

    public NewGame(JPanel field, JPanel nextFigure, JPanel scores) {
        this.setLayout(new BoxLayout(this, BoxLayout.X_AXIS));

        JPanel forField = new JPanel();
        forField.setLayout(new BoxLayout(forField, BoxLayout.Y_AXIS));
        forField.add(Box.createVerticalStrut(10));
        forField.add(field);
        forField.add(Box.createVerticalStrut(10));

        this.add(forField);
        this.add(Box.createHorizontalStrut(25));

        JPanel info = new JPanel();
        info.setLayout(new BoxLayout(info, BoxLayout.Y_AXIS));
        this.add(info);

        info.setMaximumSize(new Dimension(200, 1200));
        info.setMinimumSize(new Dimension(120, 720));

        info.add(Box.createVerticalStrut(70));
        info.add(nextFigure);
        info.add(Box.createVerticalStrut(50));
        info.add(scores);
        info.add(Box.createVerticalStrut(80));

        this.add(Box.createHorizontalStrut(20));

        this.setOpaque(false);
        forField.setOpaque(false);
        info.setOpaque(false);
    }

    @Override
    public void paint(Graphics g) {
        try {
            InputStream texture = NewGame.class.getClassLoader().getResourceAsStream("MainTexture.jpg");
            if (texture == null) {
                throw new IOException("MainTexture.jpg not found");
            }
            Image img = ImageIO.read(texture);
            g.drawImage(img, 0,0,null);
        } catch (IOException ex) {
            logger.error("MainTexture", ex);
        }
        super.paint(g);
    }
}