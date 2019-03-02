package report;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class StatisticsReport implements Iterable<ReportRow> {
    public StatisticsReport() {
        set = new ArrayList<>();
    }

    public void addReportRow(String ... data) {
        set.add(new ReportRow(data));
    }

    @Override
    public Iterator<ReportRow> iterator() {
        return set.iterator();
    }

    private List<ReportRow> set;
}
