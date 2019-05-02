package model;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

class Scores implements Serializable {
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

    void addNewRecord(String name, int score) {
        topPlayers.add(new Records(name, score));
        if (topPlayers.size() > 10) {
            topPlayers.remove(topPlayers.last());
        }
    }

    void saveRecords() {
        try {
            ObjectOutputStream file = new ObjectOutputStream(new FileOutputStream("src/main/resources/HighScores.ser"));
            file.writeObject(this);
            file.close();
        }
        catch (IOException ex) {
            ex.printStackTrace();
        }
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