#include "raylib-cpp.hpp"
#include <vector>
#include <cmath>

const int screenWidth = 1200;
const int screenHeight = 900;
const int cellSize = 40;

int currentFps = 60;

const float earthMass = 5.972e24f;      // kg
const float moonMass = 7.35e22f;        // kg
const float earthRadius = 6371e3f;      // m
const float moonRadius = 1737e3f;       // m
const float moonDistance = 384400e3f;   // m (average distance from Earth)
const float G = 6.67430e-11f;           // m^3 kg^-1 s^-2
const float timeStep = 3600.0f;

const float scale = 1.0f / 1.0e6f;      // 1 pixel = 1,000,000 meters

float gravity = G; 

class Object {
    public:
    Vector2 position;
    Vector2 velocity;
    float radius;
    float mass;

    Object(Vector2 pos, Vector2 vel, float rad, float m) : position(pos), velocity(vel), radius(rad), mass(m) {}

    void accelerate(float x, float y) {
        this->velocity.x += x;
        this->velocity.y += y;
    }

    void update(){
        this->position.x += this->velocity.x;
        this->position.y += this->velocity.y;
    }

    float checkCollision(Object& other){
        float dx = other.position.x - this->position.x;
        float dy = other.position.y - this->position.y;
        float distance = sqrt(dx * dx + dy * dy);
        float minDist = this->radius + other.radius;

        if (distance < minDist) {
            float overlap = minDist - distance;
            float separationX = dx / distance * overlap * 0.5f;
            float separationY = dy / distance * overlap * 0.5f;
            this->position.x -= separationX;
            this->position.y -= separationY;
            other.position.x += separationX;
            other.position.y += separationY;
            return 0.95f;
        }
        return 1.0f;
    }
};

void DrawGrid2D() {
    for (int x = 0; x <= screenWidth; x += cellSize) {
        DrawLine(x, 0, x, screenHeight, LIGHTGRAY);
    }
    for (int y = 0; y <= screenHeight; y += cellSize) {
        DrawLine(0, y, screenWidth, y, LIGHTGRAY);
    }
}




int main() {
    raylib::Window window(screenWidth, screenHeight, "2D Physics Engine");
    SetTargetFPS(currentFps);

    Vector2 earthPos = {screenWidth / 2.0f, screenHeight / 2.0f};
    float earthDrawRadius = earthRadius * scale;

    Vector2 moonPos = {
        earthPos.x + moonDistance * scale,
        earthPos.y
    };
    float moonDrawRadius = moonRadius * scale;

    float orbitalVelocity = sqrt(G * earthMass / moonDistance);

    std::vector<Object> objects = {
        Object(earthPos, {0, 0}, earthDrawRadius, earthMass),
        Object(moonPos, {0, -orbitalVelocity * scale}, moonDrawRadius, moonMass)
    };

    while (!window.ShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);

            for (auto& obj : objects) {
                for (auto& obj2 : objects) {
                    if(&obj2 == &obj ) continue;
                    float directionX = obj2.position.x - obj.position.x;
                    float directionY = obj2.position.y - obj.position.y;
                    float distance = sqrt(directionX * directionX + directionY * directionY);
                    if (distance < 1.0f) distance = 1.0f;

                    float normX = directionX / distance;
                    float normY = directionY / distance;

                    float gravitationalForce = (gravity * obj.mass * obj2.mass) / (distance / scale * distance / scale);
                    float acc1 = gravitationalForce / obj.mass;
                    obj.accelerate(normX * acc1 * scale * timeStep, normY * acc1 * scale * timeStep);

                    float damping = obj.checkCollision(obj2);
                    obj.velocity.x *= damping;
                    obj.velocity.y *= damping;
                }

                obj.position.x += obj.velocity.x * timeStep;
                obj.position.y += obj.velocity.y * timeStep;

                DrawCircleV(obj.position, obj.radius, obj.mass == earthMass ? BLUE : WHITE);

                if (obj.position.x - obj.radius < 0) {
                    obj.position.x = obj.radius;
                    obj.velocity.x *= -0.8f;
                }
                if (obj.position.x + obj.radius > screenWidth) {
                    obj.position.x = screenWidth - obj.radius;
                    obj.velocity.x *= -0.8f;
                }
                if (obj.position.y - obj.radius < 0) {
                    obj.position.y = obj.radius;
                    obj.velocity.y *= -0.8f;
                }
                if (obj.position.y + obj.radius > screenHeight) {
                    obj.position.y = screenHeight - obj.radius;
                    obj.velocity.y *= -0.8f;
                }
            }

        EndDrawing();
    }
}