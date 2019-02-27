package report;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;

public class ReportRow implements Comparable<ReportRow>, Iterable<String> {
    ReportRow(String ... info) {
        report = new ArrayList<>();
        report.addAll(Arrays.asList(info));
    }

    public ArrayList<String> getReport() {
        return report;
    }

    @Override
    public Iterator<String> iterator() {
        return report.iterator();
    }

    @Override
    public int compareTo(ReportRow it) {
        int cmp = report.get(0).compareTo(it.report.get(0));
        if (cmp == 0) {
            return report.get(1).compareTo(it.report.get(1));
        }
        return -1*cmp;
    }

    private ArrayList<String> report;
}
