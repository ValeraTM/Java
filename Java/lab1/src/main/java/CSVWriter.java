import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.nio.charset.Charset;
import java.util.Collection;
import java.util.Iterator;

class CSVWriter {
    CSVWriter(OutputStream file) {
        writer = new OutputStreamWriter(file, Charset.forName("UTF-8"));
    }
    void write(Collection<String> rep) throws IOException {
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