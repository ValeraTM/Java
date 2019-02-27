package report;

import java.util.Iterator;
import java.util.TreeSet;

public class StatisticsReport implements Iterable<ReportRow> {
    public StatisticsReport() {
        set = new TreeSet<>();
    }

    public void addReportRow(String ... data) {
        set.add(new ReportRow(data));
    }

    @Override
    public Iterator<ReportRow> iterator() {
        return set.iterator();
    }

    private TreeSet<ReportRow> set;
}
