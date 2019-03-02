import report.*;
import stat.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

class Controller {
    Controller(InputStream in, OutputStream out, int sizeTemp) throws IOException {
        WordReader reader = new WordReader(in, sizeTemp);
        TextStatistics stat = new TextStatistics();
        while (true) {
            String word = reader.read();
            if (word == null) {
                break;
            }
            stat.addWord(word);
        }
        reader.close();

        ReportGenerator gen = new ReportGenerator(stat.getStatistics());
        StatisticsReport rep = new StatisticsReport();
        for (WordStat it : gen) {
            rep.addReportRow(Integer.toString(it.getCount()), it.getWord(),
                    String.format("%.3f", 100*(double)it.getCount()/(double)stat.getWordCount()));
        }

        CSVWriter writer = new CSVWriter(out);
        for (ReportRow it : rep) {
            writer.write(it.getReport());
        }
        writer.close();
    }
}
