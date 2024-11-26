#ifndef GAMBLER_H
#define GAMBLER_H

#include <string>
#include "SlotMachine.h"
#include <iostream>

class Gambler {
    std::string name;
    double balance;

public:
    Gambler(const std::string& name, double initial_balance)
        : name(name), balance(initial_balance) {}

    std::string getName() const { return name; }
    double getBalance() const { return balance; }

    void addBalance(double amount) { balance += amount; }
    void deductBalance(double amount) { balance -= amount; }

    void playSlotMachine(SlotMachine& machine, double bet) {
        if (bet > balance) {
            std::cout << "Insufficient balance to place the bet.\n";
            return;
        }

        deductBalance(bet);
        double winnings = machine.play(bet);
        addBalance(winnings);

        std::cout << name << "'s balance: $" << balance << "\n";
    }
};

#endif // BODY_PART_H