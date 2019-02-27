import report.ReportRow;
import report.StatisticsReport;
import stat.TextStatistics;
import stat.WordStat;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class Controller {
    public static void main(String[] args) {
        try {
            WordReader reader = new WordReader(new FileInputStream(args[0]), Integer.parseInt(args[1]));
            TextStatistics stat = new TextStatistics();
            while (true) {
                String word = reader.read();
                if (word == null) {
                    break;
                }
                stat.addWord(word);
            }
            reader.close();

            StatisticsReport rep = new StatisticsReport();
            for (WordStat it : stat.getStatistics()) {
                rep.addReportRow(Integer.toString(it.getCount()), it.getWord(),
                        String.format("%.2f", 100*(double)it.getCount()/(double)stat.getWordCount()));
            }

            CSVWriter writer = new CSVWriter(new FileOutputStream("out.csv"));
            for (ReportRow it : rep) {
                writer.write(it.getReport());
            }
            writer.close();
        }
        catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}
