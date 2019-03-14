import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class App {
    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Must be two arguments");
            System.out.println("First argument: Input File");
            System.out.println("Second argument: Buffer Size");
        }
        else {
            try {
                int size = Integer.parseUnsignedInt(args[1]);
                if (size == 0) {
                    throw new NumberFormatException("Zero");
                }
                Controller boss = new Controller(new FileInputStream(args[0]), new FileOutputStream("out.csv"), size);
            } catch (NumberFormatException ex) {
                System.err.println(ex.getMessage());
                System.err.println("Second argument must be unsigned integer number > 0");
            } catch (IOException ex) {
                System.err.println(ex.getMessage());
            }
        }
    }
}
