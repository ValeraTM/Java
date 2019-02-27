import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;

class WordReader {
    WordReader(FileInputStream file, int sizeTemp) {
        reader = new InputStreamReader(file);
        if (sizeTemp > 0) {
            temp = new char[sizeTemp];
        }
        else {
            temp = new char[16];
        }
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
