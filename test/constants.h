//
//  constants.h
//  test
//
//  Created by Никита Ростовский on 11.12.2023.
//

#ifndef constants_h
#define constants_h

// MARK: - General
#define DEG_PER_RAD 57.2957795F

// MARK: - Particles
#define PARTICLE_LIFETIME 5.0F
#define PARTICLE_RADIUS 0.3F

// MARK: - Chunks
#define CHUNK_SIZE 8.0f
#define CHUNK_ACTIVATION_TIME_LIMIT_MS 4

// MARK: - Materials
#define CELL_SIZE 4.0f

// MARK: - Collisions
#define COLLLISION_THRESHOLD 1.0f
#define COLLLISION_RADIUS 8.0f

// MARK: - Debug
// TODO: remove DEBUG_ constants for release
#define DEBUG_PARTICLES_ENABLED 0
#define DEBUG_RENDER_VIEWPORT 1.0f
#define DEBUG_RESOLUTION_X 1280
#define DEBUG_RESOLUTION_Y 720

// MARK: - Rendering
#define TEXTURE_SCALE 64.0f

#endif /* constants_h */
