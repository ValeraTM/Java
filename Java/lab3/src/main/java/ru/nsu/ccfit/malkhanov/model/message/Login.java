package ru.nsu.ccfit.malkhanov.model.message;

import java.io.Serializable;

public class Login implements Serializable {
    public Login(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    private final String name;

}