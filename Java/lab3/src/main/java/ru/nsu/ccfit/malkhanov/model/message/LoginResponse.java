package ru.nsu.ccfit.malkhanov.model.message;

import java.io.Serializable;

public enum LoginResponse implements Serializable {
    SUCCESS,
    ALREADY_EXIST
}