import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.ArrayList;
import java.util.Iterator;

class CSVWriter {
    CSVWriter(FileOutputStream file) {
        writer = new OutputStreamWriter(file);
    }
    void write(ArrayList<String> rep) throws IOException {
        StringBuilder res = new StringBuilder();
        Iterator<String> it = rep.iterator();
        if (isEmpty) {
            isEmpty = false;
        }
        else {
            res.append('\n');
        }
        while (it.hasNext()) {
            res.append('\"');
            res.append(it.next());
            res.append('\"');
            if (it.hasNext()) {
                res.append(',');
            }
        }
        writer.write(res.toString());
    }

    void close() throws IOException {
        writer.close();
    }

    private boolean isEmpty = true;
    private Writer writer;
}