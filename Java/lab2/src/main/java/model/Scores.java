package model;

import ch.qos.logback.classic.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

public class Scores implements Serializable {
    private static final Logger logger = (Logger)LoggerFactory.getLogger(Scores.class);

    private class Records implements Comparable<Records>, Serializable {
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
            return name + " " + score;
        }

        String name;
        int score;
    }

    public static Scores openScores(InputStream highScores) {
        if (highScores == null) {
            logger.error("HighScoreNotFound. The NEW HighScores will be saved in " + "HighScores.ser");
            return new Scores();
        }

        try {
            ObjectInputStream file = new ObjectInputStream(highScores);
            return (Scores)file.readObject();
        }
        catch (IOException | ClassNotFoundException ex) {
            logger.warn("Incorrect HighScores file. The file will be overwritten", ex);
            return new Scores();
        }
    }

    public void addNewRecord(String name, int score) {
        topPlayers.add(new Records(name, score));
        logger.info("Added a new record: " + name + " " + score);
        if (topPlayers.size() > 10) {
            Records last = topPlayers.last();
            topPlayers.remove(last);
            logger.info("Removed record: " + last);
        }
    }

    public void saveRecords() {
        try {
            URL resourceUrl = this.getClass().getClassLoader().getResource("HighScores.ser");
            if (resourceUrl == null) {
                throw new IOException("HighScores.ser not found");
            }
            File saved = new File(resourceUrl.toURI());
            OutputStream out = new FileOutputStream(saved);
            ObjectOutputStream file = new ObjectOutputStream(out);
            file.writeObject(this);
            file.close();
            logger.info("Records saved in " + "HighScores.ser");
        }
        catch (URISyntaxException ex) {
            logger.error("New Records not saved", ex);
        }
        catch (IOException ex) {
            logger.error("High Scores are not saved", ex);
        }
    }
    public List getRecords() {
        ArrayList<String> info = new ArrayList<>(topPlayers.size());
        for (Records it : topPlayers) {
            info.add(it.toString());
        }
        return info;
    }

    private TreeSet<Records> topPlayers = new TreeSet<>();
}