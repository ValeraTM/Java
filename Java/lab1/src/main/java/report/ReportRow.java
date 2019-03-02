package report;

import java.util.Arrays;
import java.util.Collection;

public class ReportRow {
    ReportRow(String ... info) {
        report = Arrays.asList(info);
    }

    public Collection<String> getReport() {
        return report;
    }

    private final Collection<String> report;
}
