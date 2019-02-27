package stat;

import java.util.Collection;
import java.util.HashMap;

public class TextStatistics {
    public TextStatistics() {
        map = new HashMap<>();
    }
    public void addWord(String word) {
        String it = word.toLowerCase();
        if (map.containsKey(it)) {
            map.get(it).addCount();
        }
        else {
            map.put(it, new WordStat(it));
        }
        wordCount++;
    }
    public Collection<WordStat> getStatistics() {
        return map.values();
    }
    public int getWordCount() {
        return wordCount;
    }
    private int wordCount = 0;
    private HashMap<String, WordStat> map;
}
