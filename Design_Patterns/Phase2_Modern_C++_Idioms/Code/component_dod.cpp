/**
 * @file component_dod.cpp
 * @brief Component Pattern & Data-Oriented Design (ECS Architecture)
 *
 * THE PROBLEM:
 * A deep game object inheritance tree becomes fragile and unscalable:
 * GameObject → PhysicsObject → CollidableObject → RenderableCollidableObject...
 * Adding new behavior forces changes across the hierarchy.
 * OOP object graphs cause cache misses: each entity update chases pointers
 * across RAM, causing the CPU to stall waiting for cache fills.
 *
 * THE SOLUTION:
 * Entity-Component-System (ECS):
 * - ENTITY    = just an integer ID. Nothing more.
 * - COMPONENT = plain data struct. No methods. No vtables.
 * - SYSTEM    = pure function that processes arrays of specific components.
 *
 * PERFORMANCE KEY — SOA (Struct of Arrays):
 * All HealthComponents stored in one contiguous vector. CPU prefetcher loads
 * the entire array before the loop. No pointer chasing. SIMD-friendly.
 *
 * MULTICORE SCALING:
 * Independent systems run on separate threads with ZERO data sharing.
 * PhysicsSystem and RenderSystem touch different component arrays.
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra -O2 component_dod.cpp -o component_dod
 * RUN: ./component_dod
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

// ============================================================================
// PART A: Classic OOP Component (with pointer graph, for comparison)
// ============================================================================

struct OOPHealthComp  {
    float hp = 100.f;
    void update(float) { /* regenerate */ hp = std::min(hp + 0.1f, 100.f); }
};

struct OOPPhysicsComp {
    float x=0, y=0, vx=0, vy=0;
    void update(float dt) { x += vx * dt; y += vy * dt; }
};

struct OOPRenderComp  {
    std::string sprite;
    bool visible = true;
};

// Classic: Each entity owns its components via pointers
class OOPEntity {
public:
    size_t id;
    std::unique_ptr<OOPHealthComp>  health;
    std::unique_ptr<OOPPhysicsComp> physics;
    std::unique_ptr<OOPRenderComp>  render;

    OOPEntity(size_t id, bool hasHealth, bool hasPhysics, bool hasRender)
        : id(id)
    {
        if (hasHealth)  health  = std::make_unique<OOPHealthComp>();
        if (hasPhysics) physics = std::make_unique<OOPPhysicsComp>();
        if (hasRender)  render  = std::make_unique<OOPRenderComp>();
    }

    void update(float dt) {
        if (health)  health->update(dt);
        if (physics) physics->update(dt);
        // Cache miss here: health ptr → physics ptr → render ptr
        // All allocated separately on the heap, scattered in memory
    }
};

// ============================================================================
// PART B: Data-Oriented ECS
// ============================================================================

using EntityID = uint32_t;

// --- Components: Plain data structs. No methods. No vtable. ---

struct HealthComponent {
    float hp    = 100.f;
    float maxHp = 100.f;
    bool  alive = true;
};

struct PhysicsComponent {
    float x  = 0.f, y  = 0.f;  // Position
    float vx = 0.f, vy = 0.f;  // Velocity
    float mass = 1.f;
};

struct RenderComponent {
    std::string sprite;
    float r = 1.f, g = 1.f, b = 1.f;  // Color (RGB)
    bool visible = true;
};

struct AIComponent {
    enum class State { Idle, Patrol, Chase, Attack };
    State   state    = State::Idle;
    float   sightRange = 100.f;
    EntityID target  = 0;
};

// --- World: Owns ALL component arrays (SOA layout) ---
// Index = EntityID. Optional<> marks "entity has this component".
struct World {
    size_t entityCount = 0;

    // SOA: each component type in its OWN contiguous array
    std::vector<std::optional<HealthComponent>>  healths;
    std::vector<std::optional<PhysicsComponent>> physics;
    std::vector<std::optional<RenderComponent>>  renders;
    std::vector<std::optional<AIComponent>>      ais;

    EntityID createEntity() {
        EntityID id = entityCount++;
        healths.emplace_back(std::nullopt);
        physics.emplace_back(std::nullopt);
        renders.emplace_back(std::nullopt);
        ais.emplace_back(std::nullopt);
        return id;
    }

    void addHealth (EntityID id, HealthComponent  c) { healths[id] = c; }
    void addPhysics(EntityID id, PhysicsComponent c) { physics[id] = c; }
    void addRender (EntityID id, RenderComponent  c) { renders[id] = c; }
    void addAI     (EntityID id, AIComponent      c) { ais[id]     = c; }

    HealthComponent*  getHealth (EntityID id) { return healths[id] ? &*healths[id] : nullptr; }
    PhysicsComponent* getPhysics(EntityID id) { return physics[id] ? &*physics[id] : nullptr; }
    RenderComponent*  getRender (EntityID id) { return renders[id] ? &*renders[id] : nullptr; }
};

// ============================================================================
// Systems — Pure functions that iterate component arrays
// These are the "behavior". Entities and Components are just data.
// ============================================================================

// PhysicsSystem: iterates ALL physics components in one tight loop (cache-friendly)
void physicsSystem(World& world, float dt) {
    for (auto& phys : world.physics) {
        if (!phys) continue;  // Skip entities without physics
        phys->x += phys->vx * dt;
        phys->y += phys->vy * dt;
    }
}

// HealthSystem: Kills entities at 0 hp, regenerates over time
void healthSystem(World& world, float dt) {
    for (auto& health : world.healths) {
        if (!health) continue;
        if (health->hp <= 0.f) {
            health->alive = false;
        } else {
            // Passive regeneration: +1 HP/sec
            health->hp = std::min(health->hp + 1.0f * dt, health->maxHp);
        }
    }
}

// DamageSystem: Deals damage to entities in a radius
void damageSystem(World& world, EntityID source, float radius, float dmg) {
    auto* srcPhys = world.getPhysics(source);
    if (!srcPhys) return;

    for (size_t i = 0; i < world.entityCount; ++i) {
        if (i == source) continue;
        auto* phys   = world.getPhysics(i);
        auto* health = world.getHealth(i);
        if (!phys || !health) continue;

        float dx = phys->x - srcPhys->x;
        float dy = phys->y - srcPhys->y;
        float dist = std::sqrt(dx*dx + dy*dy);

        if (dist < radius) {
            health->hp -= dmg;
            std::cout << "    [DamageSystem] Entity " << i
                      << " hit for " << dmg
                      << " HP. Remaining: " << health->hp << "\n";
        }
    }
}

// RenderSystem: "Renders" all visible entities
void renderSystem(const World& world) {
    std::cout << "    [RenderSystem] Rendering scene:\n";
    for (size_t i = 0; i < world.entityCount; ++i) {
        const auto& render = world.renders[i];
        const auto& phys   = world.physics[i];
        if (!render || !render->visible) continue;

        std::cout << "      Entity " << i
                  << " [" << render->sprite << "]";
        if (phys) {
            std::cout << " at (" << phys->x << ", " << phys->y << ")";
        }
        std::cout << "\n";
    }
}

// AISystem: Simple state machine
void aiSystem(World& world) {
    for (size_t i = 0; i < world.entityCount; ++i) {
        auto& ai = world.ais[i];
        if (!ai) continue;
        switch (ai->state) {
            case AIComponent::State::Idle:
                std::cout << "    [AISystem] Entity " << i << ": Idle → Patrolling\n";
                ai->state = AIComponent::State::Patrol;
                break;
            case AIComponent::State::Patrol:
                std::cout << "    [AISystem] Entity " << i << ": Patrolling\n";
                break;
            default: break;
        }
    }
}

// ============================================================================
// DEMONSTRATION
// ============================================================================
int main() {
    std::cout << "=== Component Pattern & Data-Oriented Design Demo ===\n\n";

    // ------------------------------------------------------------------
    // PART A: Classic OOP Demo (pointer-graph, for contrast)
    // ------------------------------------------------------------------
    std::cout << "--- PART A: Classic OOP Component Graph ---\n";

    std::vector<OOPEntity> oopEntities;
    oopEntities.emplace_back(0, true, true, true);   // Full entity
    oopEntities.emplace_back(1, true, false, true);  // No physics (static prop)
    oopEntities.emplace_back(2, false, true, false); // Physics-only (ghost)

    for (auto& e : oopEntities) {
        e.update(0.016f); // ~60fps
    }
    std::cout << "  Updated " << oopEntities.size()
              << " OOP entities (scattered heap pointers — cache unfriendly)\n\n";

    // ------------------------------------------------------------------
    // PART B: ECS Demo (data-oriented, contiguous arrays)
    // ------------------------------------------------------------------
    std::cout << "--- PART B: ECS — Entity-Component-System ---\n\n";

    World world;

    // Create entities (just IDs)
    EntityID player   = world.createEntity();
    EntityID enemy1   = world.createEntity();
    EntityID enemy2   = world.createEntity();
    EntityID sceneProp = world.createEntity();

    // Attach components (mix-and-match, no inheritance)
    world.addHealth (player,    {100.f, 100.f, true});
    world.addPhysics(player,    {0.f, 0.f, 2.f, 1.f, 70.f});   // Moves right+up
    world.addRender (player,    {"player_sprite.png", 0.f,1.f,0.f, true});  // Green

    world.addHealth (enemy1,    {80.f, 80.f, true});
    world.addPhysics(enemy1,    {10.f, 5.f, -1.f, 0.f, 60.f});  // Moves left
    world.addRender (enemy1,    {"enemy_sprite.png", 1.f,0.f,0.f, true});   // Red
    world.addAI     (enemy1,    {AIComponent::State::Idle, 150.f, player});

    world.addHealth (enemy2,    {60.f, 60.f, true});
    world.addPhysics(enemy2,    {-5.f, 10.f, 0.f, -1.f, 55.f}); // Moves down
    world.addRender (enemy2,    {"enemy2_sprite.png", 1.f,0.5f,0.f, true}); // Orange
    world.addAI     (enemy2,    {AIComponent::State::Patrol, 100.f, player});

    // sceneProp: no health, no physics, no AI — just render
    world.addRender (sceneProp, {"tree.png", 0.f,0.5f,0.f, true}); // Dark green

    std::cout << "  World created with " << world.entityCount << " entities.\n\n";

    // --- Frame 1 ---
    std::cout << "=== Frame 1 ===\n";
    const float dt = 0.016f; // ~60fps

    std::cout << "  [PhysicsSystem]\n";
    physicsSystem(world, dt);
    auto* playerPhys = world.getPhysics(player);
    std::cout << "    Player position after 1 frame: ("
              << playerPhys->x << ", " << playerPhys->y << ")\n";

    std::cout << "  [HealthSystem]\n";
    healthSystem(world, dt);
    auto* playerHealth = world.getHealth(player);
    std::cout << "    Player HP: " << playerHealth->hp << "\n";

    std::cout << "  [AISystem]\n";
    aiSystem(world);

    std::cout << "  [RenderSystem]\n";
    renderSystem(world);

    // --- Frame 2 with Damage ---
    std::cout << "\n=== Frame 2 — Explosion near Player! ===\n";
    std::cout << "  [DamageSystem] Player (entity 0) causes explosion radius=15, dmg=25:\n";
    damageSystem(world, player, 15.0f, 25.0f);

    physicsSystem(world, dt);
    healthSystem(world, dt);

    std::cout << "\n  After explosion:\n";
    for (size_t i = 0; i < world.entityCount; ++i) {
        auto* h = world.getHealth(i);
        if (h) {
            std::cout << "    Entity " << i << " HP: " << h->hp
                      << (h->alive ? " (alive)" : " (DEAD)") << "\n";
        }
    }

    // ------------------------------------------------------------------
    // Performance Note
    // ------------------------------------------------------------------
    std::cout << "\n--- Performance Insight ---\n";
    std::cout << "  SOA layout: PhysicsSystem iterates one contiguous vector.\n";
    std::cout << "  If 100,000 entities: all PhysicsComponents fit in L2 cache.\n";
    std::cout << "  No pointer chasing. CPU prefetcher works at maximum efficiency.\n";
    std::cout << "  Independent systems can run on separate threads simultaneously.\n";

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
