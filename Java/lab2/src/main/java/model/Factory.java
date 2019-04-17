package model;

import model.figures.Shape;

import java.awt.Color;
import java.io.IOException;
import java.io.InputStream;
import java.util.*;

class Factory {
    private final Properties creators = new Properties();
    private final Object[] keys;
    private final Random random = new Random();

    Factory(InputStream config) throws IOException {
        creators.load(config);
        keys = creators.keySet().toArray();
    }

    Shape createRandomProduct() throws Exception {
        return createProductById((String)keys[random.nextInt(keys.length)]);
    }

    Shape createProductById(String id) throws Exception {
        Class<?> product = Class.forName(creators.getProperty(id));
        if (null == product) {
            return null;
        }
        return (Shape)product.getConstructor(int.class, Color.class).newInstance(Math.abs(random.nextInt()), new Color(random.nextInt()));
    }
}
