#include "Sector.h"

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code
    distance_from_earth = sqrt((x * x) + (y * y) + (z * z));
    std::stringstream ss;

    ss << static_cast<int>(distance_from_earth) << (x > 0 ? "R" :(x<0 ? "L" : "S")) << (y > 0 ? "U" :(y<0 ? "D" : "S")) << (z > 0 ? "F" :(z<0 ? "B" : "S"));
    sector_code=ss.str();
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}