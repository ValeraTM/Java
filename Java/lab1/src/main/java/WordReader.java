import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;

class WordReader {
    WordReader(InputStream file, int sizeTemp) {
        reader = new InputStreamReader(file);
        temp = new char[sizeTemp];
    }

    String read() throws IOException {
        if (!reader.ready() && pointer == 0) {
            return null;
        }
        StringBuilder str = new StringBuilder();
        int check = 0;
        do {
            while (pointer < count) {
                if (Character.isLetterOrDigit(temp[pointer])) {
                    str.append(temp[pointer]);
                    check++;
                    pointer++;
                }
                else {
                    pointer++;
                    if (check != 0) {
                        return str.toString();
                    }
                }
            }
            pointer = 0;
        } while ((count = reader.read(temp)) != -1);

        if (check == 0) {
            return null;
        }
        return str.toString();
    }
    void close() throws IOException {
        reader.close();
    }

    private int count = 0;
    private int pointer = 0;
    private char[] temp;
    private Reader reader;
}