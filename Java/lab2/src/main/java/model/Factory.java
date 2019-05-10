package model;

import model.figures.Shape;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import java.util.Random;

public class Factory {
    private final Properties creators = new Properties();
    private final Object[] keys;
    private final Random random = new Random();

    public Factory() throws IOException {
        InputStream in = Factory.class.getClassLoader().getResourceAsStream("FactoryConfig.properties");
        if (in == null) {
            throw new IOException("FactoryConfig.properties not found");
        }
        creators.load(in);
        keys = creators.keySet().toArray();
    }

    public Shape createRandomProduct() throws Exception {
        return createProductById((String)keys[random.nextInt(keys.length)]);
    }

    private Shape createProductById(String id) throws Exception {
        Class<?> product = Class.forName(creators.getProperty(id));
        try {
            return (Shape) product.getConstructor(int.class, Cell.class).newInstance(Math.abs(random.nextInt()), new Cell(random.nextInt()));
        }
        catch (Exception ex) {
            throw new Exception("IncorrectShapeException", ex);
        }
    }
}
