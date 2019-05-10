package model;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.util.Iterator;

public class ScoresTest {
    private Scores records = new Scores();

    @Before
    public void setUp() throws Exception {
        records.addNewRecord("A", 1);
        records.addNewRecord("B", 2);
        records.addNewRecord("C", 2);
        records.addNewRecord("A", 3);
        records.addNewRecord("B", 2);
    }

    @Test
    public void addNewRecords() {
        Iterator it = records.getRecords().iterator();
        Assert.assertEquals("A 3", it.next().toString());
        Assert.assertEquals("B 2", it.next().toString());
        Assert.assertEquals("C 2", it.next().toString());
        Assert.assertEquals("A 1", it.next().toString());
        Assert.assertFalse(it.hasNext());
    }

}