#ifndef CARD_H
#define CARD_H

#include <string>

//This class was for the slot machine before we scrapped part since it would take too much time.
class Card {
    std::string symbol;
    std::string image_file;
    int Payout;

public:
    Card(const std::string& symbol, const std::string& image_file,int payout)
        : symbol(symbol), image_file(image_file), Payout(payout) {}

    std::string getSymbol() const { return symbol; }
    std::string getImageFile() const { return image_file; }

    int get_Payout(){
        return Payout;
    }
};

#endif // BODY_PART_H