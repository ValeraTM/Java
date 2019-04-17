package view;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.util.*;

public class Scores extends JPanel {
    private class Records implements Comparable<Records> {
        @Override
        public int compareTo(Records it) {
            int cmp = Integer.compare(score, it.score);
            if (cmp == 0) {
                return name.compareTo(it.name);
            }
            return -1*cmp;
        }

        Records(String name, int score) {
            this.name = name;
            this.score = score;
        }

        @Override
        public String toString() {
            return name + "   " + score;
        }

        String name;
        int score;
    }

    @Override
    public void paint(Graphics g) {
        Graphics2D painter = (Graphics2D)g;
        super.paint(painter);
        int offsetX = this.getWidth()/5*2;
        int offsetY = this.getHeight()/5*2;

        painter.setFont(new Font("Font", Font.ITALIC, 26));
        int i = 0;
        painter.setColor(Color.BLACK);
        for (Records it : topPlayers) {
            painter.drawString(it.toString(), offsetX, offsetY + i*40);
            i++;
        }
    }

    public Scores() throws IOException {
        Scanner scanner = new Scanner(new FileInputStream("src/main/resources/HighScores.txt"));

        while (scanner.hasNext()) {
            topPlayers.add(new Records(scanner.next(), Integer.parseInt(scanner.next())));
        }
    }

    public boolean addNewRecord(String name, int score) {
        topPlayers.add(new Records(name, score));
        if (topPlayers.size() > 10) {
            topPlayers.remove(topPlayers.last());
        }
        return true;
    }

    public void saveRecords() {
        try {
            Writer writer = new OutputStreamWriter(new FileOutputStream("src/main/resources/HighScores.txt"));
            for (Records it : topPlayers) {
                writer.write(it.name + " " + it.score + '\n');
            }
            writer.close();
        }
        catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    private TreeSet<Records> topPlayers = new TreeSet<>();
}
