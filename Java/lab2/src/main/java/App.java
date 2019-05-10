import ch.qos.logback.classic.Logger;
import org.slf4j.LoggerFactory;

public class App {
    private final static Logger logger = (Logger)LoggerFactory.getLogger(App.class);

    public static void main(String[] args) {
        int height = 24;
        int width = 10;
        if (args.length == 2) {
            try {
                height = Integer.parseUnsignedInt(args[0]);
                width = Integer.parseUnsignedInt(args[1]);
            }
            catch (NumberFormatException ex) {
                logger.warn("Incorrect size of field", ex);
            }
            height = Integer.max(24, height);
            width = Integer.max(10, width);
        }
        logger.info("Started Application " + height + "x" + width);

        Controller boss = new Controller(height, width);
    }
}
