import controller.StartMenu;
import model.Game;
import view.NewGame;
import model.Factory;

class Controller {
    Controller(Factory factory) {
        NewGame view = new NewGame(24, 10);
        Game model = new Game(10, 24, factory);
        model.registerObserver(view);
        StartMenu menu = new StartMenu(model, view);
    }
}