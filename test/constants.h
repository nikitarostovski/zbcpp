//
//  constants.h
//  test
//
//  Created by Никита Ростовский on 11.12.2023.
//

#ifndef constants_h
#define constants_h

// SFML uses degrees for angles while Box2D uses radians
#define DEG_PER_RAD 57.2957795F

#define PARTICLE_LIFETIME 5.0F
#define PARTICLE_RADIUS 0.3F

#define CHUNK_SIZE 8.0f
#define CHUNK_ACTIVATION_TIME_LIMIT_MS 4

#define COLLLISION_THRESHOLD 100.0f
#define COLLLISION_RADIUS 8.0f

// TODO: remove DEBUG_ constants for release
#define DEBUG_RENDER_VIEWPORT 1.0f
#define DEBUG_RESOLUTION_X 1280
#define DEBUG_RESOLUTION_Y 720

#endif /* constants_h */
