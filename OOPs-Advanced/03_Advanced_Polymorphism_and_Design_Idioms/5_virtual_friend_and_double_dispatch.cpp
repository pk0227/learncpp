/**
 * @file 5_virtual_friend_and_double_dispatch.cpp
 * @brief Demonstrates Virtual Friend Functions and Double Dispatch:
 *        - Non-member friend functions delegating to protected virtual methods.
 *        - Double dispatch resolving interactions between two polymorphic objects without dynamic_cast.
 */

#include <iostream>

// Forward declarations
class SpaceShip;
class Asteroid;

// Base class for game entities
class GameObject {
public:
    virtual ~GameObject() = default;

    // First dispatch level: accept any other GameObject
    virtual void collideWith(GameObject& other) = 0;

    // Second dispatch level: specific overloads for concrete types
    virtual void collideWithShip(SpaceShip& ship) = 0;
    virtual void collideWithAsteroid(Asteroid& asteroid) = 0;

protected:
    virtual void print(std::ostream& os) const = 0;

public:
    // Virtual Friend Idiom: Non-member friend delegates to protected virtual print()
    friend std::ostream& operator<<(std::ostream& os, const GameObject& obj) {
        obj.print(os);
        return os;
    }
};

class SpaceShip : public GameObject {
protected:
    void print(std::ostream& os) const override { os << "SpaceShip [Player 1]"; }

public:
    void collideWith(GameObject& other) override {
        // Double dispatch bounce: passes *this (strongly typed as SpaceShip&)
        other.collideWithShip(*this);
    }

    void collideWithShip(SpaceShip& ship) override {
        std::cout << "  Event: " << *this << " collided with another " << ship << "! (Shields intact)\n";
    }

    void collideWithAsteroid(Asteroid& asteroid) override;
};

class Asteroid : public GameObject {
protected:
    void print(std::ostream& os) const override { os << "Asteroid [Mass 5000t]"; }

public:
    void collideWith(GameObject& other) override {
        // Double dispatch bounce: passes *this (strongly typed as Asteroid&)
        other.collideWithAsteroid(*this);
    }

    void collideWithShip(SpaceShip& ship) override {
        std::cout << "  Event: " << *this << " CRUSHED " << ship << "! (Explosion triggered)\n";
    }

    void collideWithAsteroid(Asteroid& other) override {
        std::cout << "  Event: " << *this << " bounced off another " << other << "! (Elastic collision)\n";
    }
};

void SpaceShip::collideWithAsteroid(Asteroid& asteroid) {
    std::cout << "  Event: " << *this << " was hit by " << asteroid << "!\n";
}

int main() {
    std::cout << "=== 1. Virtual Friend Function Output ===\n";
    SpaceShip ship;
    Asteroid rock;
    std::cout << "Entity 1: " << ship << "\n";
    std::cout << "Entity 2: " << rock << "\n\n";

    std::cout << "=== 2. Double Dispatch Execution (Zero dynamic_cast) ===\n";
    GameObject& obj1 = ship;
    GameObject& obj2 = rock;

    // Both pointers are statically typed as GameObject&.
    // Double dispatch resolves the exact interaction via 2 virtual function calls:
    obj1.collideWith(obj2); // Ship collides with Rock
    obj2.collideWith(obj1); // Rock collides with Ship

    return 0;
}
