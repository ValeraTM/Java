import controller.StartMenu;
import model.Game;
import view.*;

class Controller {
    Controller(int height, int width) {
        Game model = new Game(width, height);

        PlayField field = new PlayField(model);
        NextFigure nextFigure = new NextFigure(model);
        GameScore score = new GameScore(model);
        model.registerObserver(field);
        model.registerObserver(nextFigure);
        model.registerObserver(score);

        NewGame view = new NewGame(field, nextFigure, score);
        StartMenu menu = new StartMenu(model, view);
    }
}
