package stat;

import java.util.Collection;
import java.util.Iterator;
import java.util.TreeSet;

public class ReportGenerator implements Iterable<WordStat> {
    public ReportGenerator(Collection<WordStat> values) {
        set = new TreeSet<>(values);
    }

    @Override
    public Iterator<WordStat> iterator() {
        return set.iterator();
    }

    private final TreeSet<WordStat> set;
}
