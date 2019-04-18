import controller.Controller;
import model.Game;
import view.NewGame;
import view.Scores;
import view.StartMenu;

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

            Scores records = new Scores();
            NewGame newGame = new NewGame(24, 10, records);
            Controller app = new Controller(new Game(10, 24, new FileInputStream(configFactory), newGame));
            StartMenu menu = new StartMenu(newGame, records, app);
            app.setFrame(menu);
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
