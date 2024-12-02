#ifndef SLOTMACHINE_H
#define SLOTMACHINE_H

#include <vector>
#include <string>
#include <random>
#include "Card.h"
#include <ctime>
#include <iostream>

//This was going to be the slot machine class. This all worked in the command window btw incase you have been folowing the lore.
//But if you knew the amount of head banging getting the snake visual to work. you cant make us do it for the slot machine.
class SlotMachine {
    std::vector<Card> cards;
    int reel_count;

    // Utility function to spin the reels and return results
    std::vector<Card> spinReels() {
        std::vector<Card> results;
        static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
        std::uniform_int_distribution<> dist(0, cards.size() - 1);

        for (int i = 0; i < reel_count; ++i) {
            results.push_back(cards[dist(rng)]);
        }
        return results;
    }

public:
    SlotMachine(int reels, const std::vector<Card>& available_cards)
        : reel_count(reels), cards(available_cards) {}

    double play(double bet) {
        std::vector<Card> results = spinReels();

        std::cout << "Reels: ";
        for (const auto& card : results) {
            std::cout << card.getSymbol() << " ";
        }
        std::cout << "\n";

        // Example payout logic: if all symbols match, 5x payout
        if (results[0].getSymbol() == results[1].getSymbol() &&
            results[1].getSymbol() == results[2].getSymbol()) {
            double payout = bet * results[0].get_Payout();
            std::cout << "Jackpot! You win $" << payout << "!\n";
            return payout;
        }

        std::cout << "Better luck next time!\n";
        return 0.0;
    }
};

#endif // BODY_PART_H