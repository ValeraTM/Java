package stat;

public class WordStat implements Comparable<WordStat> {
    WordStat(String word) {
        this.word = word;
        frequency = 1;
    }

    @Override
    public int compareTo(WordStat it) {
        int cmp = Integer.compare(frequency, it.frequency);
        if (cmp == 0) {
            return word.compareTo(it.word);
        }
        return -1*cmp;
    }

    void addCount() {
        frequency++;
    }
    public String getWord() {
        return word;
    }
    public int getCount() {
        return frequency;
    }

    private final String word;
    private int frequency;
}