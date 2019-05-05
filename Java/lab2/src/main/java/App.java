public class App {
    public static void main(String[] args) {
        int height = 24;
        int width = 10;
        if (args.length == 2) {
            height = Integer.parseUnsignedInt(args[0]);
            width = Integer.parseUnsignedInt(args[1]);
        }
        Controller boss = new Controller(height, width);
    }
}
