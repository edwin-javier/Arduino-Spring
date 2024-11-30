package com.parqueo.contador_de_parqueo.controllers;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.HashMap;
import java.util.Map;

@RestController
@RequestMapping("/parkingData")
public class ParkingDataController {

    private Map<String, Integer[]> parkingState = new HashMap<>();

    public ParkingDataController() {
        // Inicializa el estado del parqueo en el constructor
        parkingState.put("disponibles", new Integer[]{1, 1, 1});   // Estado de los espacios regulares
        parkingState.put("disponiblesR", new Integer[]{1, 1, 1});  // Estado de los espacios reservados
    }

    @PostMapping
    public ResponseEntity<String> receiveParkingData(@RequestBody Map<String, Integer[]> parkingData) {
        // Actualiza el estado del parqueo con los datos recibidos del sensor
        if (parkingData.containsKey("disponibles")) {
            parkingState.put("disponibles", parkingData.get("disponibles"));
        }
        if (parkingData.containsKey("disponiblesR")) {
            parkingState.put("disponiblesR", parkingData.get("disponiblesR"));
        }
        return ResponseEntity.ok("Datos recibidos correctamente");
    }

    @GetMapping
    public ResponseEntity<Map<String, Integer[]>> getParkingData() {
        return ResponseEntity.ok(parkingState);
    }
}

