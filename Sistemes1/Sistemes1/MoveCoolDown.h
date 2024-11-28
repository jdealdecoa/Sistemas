#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>


class MoveCooldown {
private:
    bool canMove; // Indica si el jugador puede moverse
    bool running; // Controla si el hilo está activo
    std::chrono::milliseconds cooldownTime; // Tiempo entre movimientos
    std::mutex cooldownMutex;

    void CooldownLoop() {
        while (true) {
            cooldownMutex.lock();
            if (!running) {
                cooldownMutex.unlock();
                break;
            }
            canMove = true; // Habilitar movimiento
            cooldownMutex.unlock();

            std::this_thread::sleep_for(cooldownTime); // Esperar cooldown
        }
    }

public:
    MoveCooldown(int cooldownMs) : cooldownTime(cooldownMs), canMove(true), running(true) {
        std::thread(&MoveCooldown::CooldownLoop, this).detach();
    }

    ~MoveCooldown() {
        Stop();
    }

    bool TryMove() {
        cooldownMutex.lock();
        if (canMove) {
            canMove = false;
            cooldownMutex.unlock();
            return true;
        }
        cooldownMutex.unlock();
        return false;
    }

    void Stop() {
        cooldownMutex.lock();
        running = false; // Detener el hilo
        cooldownMutex.unlock();
    }
};