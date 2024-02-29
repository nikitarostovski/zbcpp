#pragma once

enum CollisionCategory {
    CategoryTerrain = 0x00000001,
    CategoryShip = 0x00000002,
    CategoryOrb = 0x00000004,
    CategoryShipCollector = 0x00000008,
    CategoryBuilding = 0x00000016
//    PlayerFrame = 0x0001,
//    PlayerEmitter = 0x0002,
//    PlayerCollector = 0x0004,
//    PlayerShield = 0x0008,
//    Asteroid = 0x0010,
//    PlanetCore = 0x0020,
//    CategoryOrb = 0x0040
};
