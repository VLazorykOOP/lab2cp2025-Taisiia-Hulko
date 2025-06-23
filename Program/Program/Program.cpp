#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <chrono>

const double PI = 3.1415926535;

struct Position {
    double x, y;
};

std::mutex cout_mutex;

class Developer {
public:
    Position pos;
    double angle;
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

    void print(int t) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "[Time " << t << "s] Dev at (" << pos.x << ", " << pos.y << ")\n";
    }

    void run(int totalTime, int N, double dt) {
        for (int t = 0; t <= totalTime; t += dt) {
            if (t % N == 0) {
                updateDirection();
            }
            move(dt);
            print(t);
            std::this_thread::sleep_for(std::chrono::milliseconds(int(dt * 1000)));
        }
    }
};

class Manager {
public:
    Position pos;
    double angle;
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

    void print(int t) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "[Time " << t << "s] Manager at (" << pos.x << ", " << pos.y << ")\n";
    }

    void run(int totalTime, double dt) {
        for (int t = 0; t <= totalTime; t += dt) {
            move(dt);
            print(t);
            std::this_thread::sleep_for(std::chrono::milliseconds(int(dt * 1000)));
        }
    }
};

int main() {
    srand(time(0));

    double V = 1.0;
    double R = 5.0;
    int N = 5;
    int totalTime = 30;
    double dt = 1.0;

    Developer dev(V);
    Manager man(V, R);

    std::thread devThread(&Developer::run, &dev, totalTime, N, dt);
    std::thread manThread(&Manager::run, &man, totalTime, dt);

    devThread.join();
    manThread.join();

    return 0;
}
