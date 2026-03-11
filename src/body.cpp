#include "raylib-cpp.hpp"

const int screenWidth = 800;
const int screenHeight = 600;
const int cellSize = 40;

int currentFps = 60;
float gravity = 500.0f; // pixels/s^2
float spaceDensity = 0.001f;

Vector2 position = { 400, 300 };
Vector2 velocity = { 0, 0 };



struct Object {
    Vector2 position;
    Vector2 velocity;
    float mass;
    float dragCoefficient;
    float projectedArea;

    Object(Vector2 pos, Vector2 vel, float m) : position(pos), velocity(vel), mass(m) {
        dragCoefficient = 0.47f; 
        projectedArea = 3.14159f * 0.3f * 0.3f;
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
    Object ball(position, velocity, 1.0f);

    bool dragging = false;
    Vector2 dragOffset = {0.0f, 0.0f};
    Vector2 lastMouse = {0.0f, 0.0f};

    while (!window.ShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawGrid2D();
            DrawCircleV(ball.position, 30, MAROON);

            Vector2 mouse = GetMousePosition();
            bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
            bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
            bool mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

            if (mousePressed && CheckCollisionPointCircle(mouse, ball.position, 30)) {
                dragging = true;
                dragOffset.x = mouse.x - ball.position.x;
                dragOffset.y = mouse.y - ball.position.y;
                lastMouse = mouse;
                ball.velocity = {0.0f, 0.0f}; 
            }

            if (dragging && mouseDown) {
                ball.position.x = mouse.x - dragOffset.x;
                ball.position.y = mouse.y - dragOffset.y;
            }

            if (dragging && mouseReleased) {
                ball.velocity.x = (mouse.x - lastMouse.x) * currentFps * 0.02f;
                ball.velocity.y = (mouse.y - lastMouse.y) * currentFps * 0.02f;
                dragging = false;
            }

            lastMouse = mouse;

            if (!dragging) {
                // Calculate forces
                // F_drag = 0.5 * ρ * v² * Cd * A
                // F_drag_x = -F_drag * (vx / |v|)
                // F_drag_y = -F_drag * (vy / |v|)
                float speed2 = ball.velocity.x * ball.velocity.x + ball.velocity.y * ball.velocity.y;
                float speed = sqrtf(speed2);
                float dragForceMagnitude = 0.5f * spaceDensity * speed2 * ball.dragCoefficient * ball.projectedArea;
                Vector2 dragForce = { 0.0f, 0.0f };
                if (speed > 0.0f) {
                    dragForce.x = -dragForceMagnitude * (ball.velocity.x / speed);
                    dragForce.y = -dragForceMagnitude * (ball.velocity.y / speed);
                }

                // F_gravity = m * g
                Vector2 gravityForce = { 0.0f, ball.mass * gravity };

                // a = F / m,  v += a * dt,  p += v * dt
                float dt = 1.0f / currentFps;
                ball.velocity.x += (dragForce.x / ball.mass) * dt;
                ball.velocity.y += (dragForce.y / ball.mass + gravityForce.y / ball.mass) * dt;
                ball.position.x += ball.velocity.x * dt;
                ball.position.y += ball.velocity.y * dt;

                if (ball.position.y > screenHeight - 30) {
                    ball.position.y = screenHeight - 30;
                    ball.velocity.y *= -0.8f;
                }
            }

        EndDrawing();
    }
}