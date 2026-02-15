#include "BlackjackDialog.h"

#include <SDL.h>

#include <algorithm>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace {

enum class Suit { HEARTS, DIAMONDS, CLUBS, SPADES };

enum class Rank {
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

struct Card {
    Suit suit;
    Rank rank;
};

static int cardValue(const Card &c) {
    switch (c.rank) {
        case Rank::ACE:
            return 11;
        case Rank::TWO:
            return 2;
        case Rank::THREE:
            return 3;
        case Rank::FOUR:
            return 4;
        case Rank::FIVE:
            return 5;
        case Rank::SIX:
            return 6;
        case Rank::SEVEN:
            return 7;
        case Rank::EIGHT:
            return 8;
        case Rank::NINE:
            return 9;
        case Rank::TEN:
        case Rank::JACK:
        case Rank::QUEEN:
        case Rank::KING:
            return 10;
    }
    return 0;
}

static const char *rankSymbol(Rank r) {
    switch (r) {
        case Rank::ACE:
            return "A";
        case Rank::TWO:
            return "2";
        case Rank::THREE:
            return "3";
        case Rank::FOUR:
            return "4";
        case Rank::FIVE:
            return "5";
        case Rank::SIX:
            return "6";
        case Rank::SEVEN:
            return "7";
        case Rank::EIGHT:
            return "8";
        case Rank::NINE:
            return "9";
        case Rank::TEN:
            return "10";
        case Rank::JACK:
            return "J";
        case Rank::QUEEN:
            return "Q";
        case Rank::KING:
            return "K";
    }
    return "?";
}

static const char *suitSymbol(Suit s) {
    switch (s) {
        case Suit::HEARTS:
            return "H";
        case Suit::DIAMONDS:
            return "D";
        case Suit::CLUBS:
            return "C";
        case Suit::SPADES:
            return "S";
    }
    return "?";
}

static int handValue(const std::vector<Card> &hand) {
    int sum = 0;
    int aces = 0;
    for (const auto &c : hand) {
        sum += cardValue(c);
        if (c.rank == Rank::ACE) {
            aces++;
        }
    }
    while (sum > 21 && aces-- > 0) {
        sum -= 10;
    }
    return sum;
}

static bool isSoft(const std::vector<Card> &hand) {
    int sum = 0;
    int aces = 0;
    for (const auto &c : hand) {
        sum += cardValue(c);
        if (c.rank == Rank::ACE) {
            aces++;
        }
    }
    return aces > 0 && sum <= 21;
}

static bool dealerShouldHit(const std::vector<Card> &hand) {
    int value = handValue(hand);
    return value < 17 || (value == 17 && isSoft(hand));
}

static std::string formatHand(const std::vector<Card> &hand, bool hideFirst) {
    std::ostringstream oss;
    for (size_t i = 0; i < hand.size(); i++) {
        if (i > 0) {
            oss << " ";
        }
        if (hideFirst && i == 0) {
            oss << "??";
        } else {
            oss << rankSymbol(hand[i].rank) << suitSymbol(hand[i].suit);
        }
    }
    return oss.str();
}

static std::vector<Card> makeDeck() {
    std::vector<Card> deck;
    deck.reserve(52);
    for (int s = 0; s < 4; s++) {
        for (int r = 0; r < 13; r++) {
            deck.push_back(Card{static_cast<Suit>(s), static_cast<Rank>(r)});
        }
    }
    return deck;
}

static void shuffleDeck(std::vector<Card> &deck) {
    static std::mt19937 rng{std::random_device{}()};
    std::shuffle(deck.begin(), deck.end(), rng);
}

static Card draw(std::vector<Card> &deck) {
    if (deck.empty()) {
        deck = makeDeck();
        shuffleDeck(deck);
    }
    Card c = deck.back();
    deck.pop_back();
    return c;
}

static int showHitStandDialog(SDL_Window *window, const std::string &message) {
    const SDL_MessageBoxButtonData buttons[] = {
        {0, 0, "Hit"},
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Stand"},
        {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "Quit"},
    };
    const SDL_MessageBoxData data = {
        SDL_MESSAGEBOX_INFORMATION,
        window,
        "Blackjack",
        message.c_str(),
        3,
        buttons,
        nullptr,
    };
    int buttonid = -1;
    SDL_ShowMessageBox(&data, &buttonid);
    return buttonid;
}

static void showInfoDialog(SDL_Window *window, const std::string &message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Blackjack", message.c_str(), window);
}

} // namespace

bool BlackjackDialog::play(SDL_Window *window) {
    std::vector<Card> deck = makeDeck();
    shuffleDeck(deck);

    while (true) {
        std::vector<Card> playerHand;
        std::vector<Card> dealerHand;
        playerHand.reserve(12);
        dealerHand.reserve(12);

        bool dealerHideFirst = true;
        bool finished = false;
        bool playerWon = false;

        playerHand.push_back(draw(deck));
        playerHand.push_back(draw(deck));
        dealerHand.push_back(draw(deck));
        dealerHand.push_back(draw(deck));

        if (handValue(playerHand) == 21) {
            dealerHideFirst = false;
            finished = true;
            playerWon = true;
        }

        while (!finished) {
            std::ostringstream msg;
            msg << "Dealer: " << formatHand(dealerHand, dealerHideFirst) << "\n";
            msg << "Player: " << formatHand(playerHand, false) << "\n";
            msg << "Points: " << handValue(playerHand) << "\n\n";
            msg << "Choose an action.";

            int choice = showHitStandDialog(window, msg.str());
            if (choice == 2 || choice == -1) {
                return false;
            }
            if (choice == 0) {
                playerHand.push_back(draw(deck));
                if (handValue(playerHand) > 21) {
                    finished = true;
                    playerWon = false;
                }
            } else {
                dealerHideFirst = false;
                while (dealerShouldHit(dealerHand)) {
                    dealerHand.push_back(draw(deck));
                }
                int p = handValue(playerHand);
                int d = handValue(dealerHand);
                if (p > 21) {
                    finished = true;
                    playerWon = false;
                } else if (d > 21) {
                    finished = true;
                    playerWon = true;
                } else if (p > d) {
                    finished = true;
                    playerWon = true;
                } else if (p < d) {
                    finished = true;
                    playerWon = false;
                } else {
                    showInfoDialog(window, "TIE! DEALING AGAIN");
                    finished = false;
                    goto next_round;
                }
            }
        }

        {
            std::ostringstream finalMsg;
            finalMsg << (playerWon ? "YOU WIN!" : "YOU LOSE") << "\n\n";
            finalMsg << "Dealer: " << formatHand(dealerHand, false) << " (" << handValue(dealerHand) << ")\n";
            finalMsg << "Player: " << formatHand(playerHand, false) << " (" << handValue(playerHand) << ")\n";
            showInfoDialog(window, finalMsg.str());
        }

        return playerWon;

    next_round:
        continue;
    }
}
