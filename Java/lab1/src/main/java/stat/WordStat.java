package stat;

public class WordStat {
    WordStat(String word) {
        this.word = word;
        frequency = 1;
    }

    @Override
    public int hashCode() {
        return word.hashCode();
    }

    @Override
    public boolean equals(Object obj) {
        if (this.getClass() != obj.getClass()) {
            return false;
        }
        WordStat it = (WordStat) obj;
        return word.equals(it.word);
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

    private String word;
    private int frequency;
}