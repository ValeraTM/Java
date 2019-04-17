import controller.StartMenu;

import java.io.FileInputStream;

public class App {
    private static void usage() {
        System.out.println("Must be one argument");
        System.out.println("First argument must be config file for factory");
    }

    public static void main(String[] args) {
        if (args.length != 1) {
            usage();
            return;
        }
        try {
            String configFactory = args[0];
            StartMenu app = new StartMenu(new FileInputStream(configFactory), 24, 10);
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
