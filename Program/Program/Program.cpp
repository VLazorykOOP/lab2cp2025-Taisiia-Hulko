#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

const double PI = 3.1415926535;

struct Position {
    double x, y;
};

class Developer {
public:
    Position pos;
    double angle; // напрям руху в радіанах
    double speed;

    Developer(double speed) : speed(speed) {
        pos = { 0.0, 0.0 };
        angle = (rand() % 360) * PI / 180.0;
    }

    void updateDirection() {
        angle = (rand() % 360) * PI / 180.0;
    }

    void move(double dt) {
        pos.x += speed * dt * cos(angle);
        pos.y += speed * dt * sin(angle);
    }

    void print() {
        std::cout << "Dev at (" << pos.x << ", " << pos.y << ")\n";
    }
};

class Manager {
public:
    Position pos;
    double angle; // поточне положення по колу в радіанах
    double speed;
    double radius;

    Manager(double speed, double radius) : speed(speed), radius(radius) {
        angle = 0.0;
        updatePosition();
    }

    void move(double dt) {
        angle += (speed * dt) / radius;
        updatePosition();
    }

    void updatePosition() {
        pos.x = radius * cos(angle);
        pos.y = radius * sin(angle);
    }

    void print() {
        std::cout << "Manager at (" << pos.x << ", " << pos.y << ")\n";
    }
};

int main() {
    srand(time(0));

    double V = 1.0;      // швидкість
    double R = 5.0;      // радіус кола для менеджерів
    int N = 5;           // інтервал зміни напрямку для розробників
    int totalTime = 30;  // тривалість симуляції
    double dt = 1.0;     // крок часу

    Developer dev(V);
    Manager man(V, R);

    for (int t = 0; t <= totalTime; t += dt) {
        std::cout << "Time: " << t << "s\n";
        if (t % N == 0) {
            dev.updateDirection();
        }

        dev.move(dt);
        man.move(dt);

        dev.print();
        man.print();
        std::cout << "-------------------\n";
    }

    return 0;
}
