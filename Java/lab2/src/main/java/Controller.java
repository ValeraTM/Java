import controller.StartMenu;
import model.Game;
import view.NewGame;

class Controller {
    Controller(int height, int width) {
        NewGame view = new NewGame(height, width);
        Game model = new Game(width, height);
        model.registerObserver(view);
        StartMenu menu = new StartMenu(model, view);
    }
}
