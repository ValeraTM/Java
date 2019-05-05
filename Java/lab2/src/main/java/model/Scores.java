package model;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

class Scores implements Serializable {
    private static final Logger logger = LogManager.getLogger(Scores.class);

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

    public static Scores openScores(String fileName) {
        InputStream highScores;
        try {
            highScores = new FileInputStream(fileName);
        }
        catch (IOException ex) {
            logger.error("HighScoreNotFound", ex);
            return new Scores();
        }

        try {
            ObjectInputStream file = new ObjectInputStream(highScores);
            return (Scores)file.readObject();
        }
        catch (Exception ex) {
            logger.warn("Incorrect file. The file will be overwritten", ex);
            return new Scores();
        }
    }

    void addNewRecord(String name, int score) {
        topPlayers.add(new Records(name, score));
        logger.info("Added a new record: " + name + " " + score);
        if (topPlayers.size() > 10) {
            Records last = topPlayers.last();
            topPlayers.remove(last);
            logger.info("Removed record: " + last);
        }
    }

    void saveRecords() {
        try {
            ObjectOutputStream file = new ObjectOutputStream(new FileOutputStream("src/main/resources/HighScores.ser"));
            file.writeObject(this);
            file.close();
        }
        catch (IOException ex) {
            logger.error("High Scores are not saved", ex);
        }
        logger.info("Records saved in " + "src/main/resources/HighScores.ser");
    }
    List getRecords() {
        ArrayList<String> info = new ArrayList<>(topPlayers.size());
        for (Records it : topPlayers) {
            info.add(it.toString());
        }
        return info;
    }

    private TreeSet<Records> topPlayers = new TreeSet<>();
}