#pragma once

enum CollisionCategory {
    PlayerFrame = 0x0001,
    PlayerEmitter = 0x0002,
    PlayerCollector = 0x0004,
    PlayerShield = 0x0008,
    Asteroid = 0x0010,
    PlanetCore = 0x0020
};
