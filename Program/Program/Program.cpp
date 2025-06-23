#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>

const double PI = 3.1415926535;

struct Position {
    double x, y;
};

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

    void print() {
        std::cout << "Dev at (" << pos.x << ", " << pos.y << ")\n";
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

    void print() {
        std::cout << "Manager at (" << pos.x << ", " << pos.y << ")\n";
    }
};

std::mutex mtx;

void developerThread(Developer& dev, int N, double dt, int totalTime) {
    for (int t = 0; t <= totalTime; t += dt) {
        if (t % N == 0) {
            dev.updateDirection();
        }
        dev.move(dt);

        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[Time " << t << "s] ";
        dev.print();
    }
}

void managerThread(Manager& man, double dt, int totalTime) {
    for (int t = 0; t <= totalTime; t += dt) {
        man.move(dt);

        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[Time " << t << "s] ";
        man.print();
    }
}

int main() {
    srand(time(0));

    double V = 1.0;
    double R = 5.0;
    int N = 5;
    int totalTime = 30;
    double dt = 1.0;

    Developer dev(V);
    Manager man(V, R);

    std::thread devThread(developerThread, std::ref(dev), N, dt, totalTime);
    std::thread manThread(managerThread, std::ref(man), dt, totalTime);

    devThread.join();
    manThread.join();

    return 0;
}
