#include <deque>
#include <iostream>
#include <algorithm>
#include <locale>
#include <ctime>
#include <experimental/random>
#include <getopt.h>

#include "deck_of_cards.h"
int DeckOfCards::handSize = 5;
int DeckOfCards::numPlayers = 1;

DeckOfCards::DeckOfCards()
{
    uint16_t num_suits = sizeof(SUITS)/sizeof(std::string);
    uint16_t num_values = sizeof(VALUES)/sizeof(std::string);

    // std::wcout << L"User-preferred locale setting is " << std::locale("").name().c_str() << std::endl;
    // std::locale::global(std::locale("en_US.UTF-8"));
    // std::wcout.imbue(std::locale());

    std::wcout  << L"Building a deck of cards.\n"
                << L"Suits: " << num_suits << L"\n"
                << L"Values: " << num_values
                << std::endl;

    for( int i=0; i<num_suits; i++)
    {
        for( int j=0; j<num_values; j++)
        {
            // std::cout << VALUES[j] << SUITS[i] << std::endl;

            Card card;
            card.Suit = SUITS[i];
            card.Value = VALUES[j];
            // std::cout << "Adding card: " << card.Value << card.Suit << std::endl;

            myCards.push_back(card);
        }
    }

    std::wcout << L"Deck has " << myCards.size() << std::endl;   
}

DeckOfCards::~DeckOfCards()
{

}

void DeckOfCards::PrintCards(CARDS cards)
{
    for (CARDS::iterator i = cards.begin(); i != cards.end(); i++)
    {
        Card* card = &(*i);
        std::cout << card->Value << card->Suit << " ";
    }
    std::cout << std::endl;
}

Card DeckOfCards::DrawCard()
{
    return (DrawCards(1)[0]);
}

CARDS DeckOfCards::DrawCards(int num)
{
    std::wcout << L"Drawing " << num << L" cards." << std::endl;

    CARDS hand;

    if (myCards.size() < num)
    {
        num = myCards.size();
    }

    hand.insert(hand.begin(), myCards.begin(), myCards.begin() + num);
    myCards.erase(myCards.begin(), myCards.begin()+num);

    std::cout << "Returning  " << hand.size() << " cards." << std::endl;
    std::cout << "Deck has " << Size() << " remaining." << std::endl;

    return hand;
}

void DeckOfCards::Shuffle()
{
    auto shuffler = [=] 
    {
        int shuffles = std::experimental::randint(17, 51);

        for(int i=0; i<shuffles; i++)
        {
            std::srand(std::time(nullptr));
            uint32_t seed = std::rand();
            std::shuffle(myCards.begin(), myCards.end(), std::default_random_engine(seed));
            std::random_shuffle(myCards.begin(), myCards.end());
        }
    };

    shuffler();
}

uint16_t DeckOfCards::Size()
{
    return myCards.size();
}

 void DeckOfCards::ProcessArgs(int argc, char* argv[])
{
     if (1 < argc)
    {
        static struct option long_options [] =
        {
            {"num-cards", required_argument, 0, 'n'},
            {"players", required_argument, 0, 'p'},
            {0,0,0,0}  
        };

        int option_index = 0;
        int c;

        while((c = getopt_long(argc, argv, "n:", long_options, &option_index)) != -1)
        {
            switch (c)
            {
                case 0:
                    std::cout << "Drawing " << DeckOfCards::handSize << " cards." << std::endl;
                break;
                case 'n':
                    if (long_options[option_index].has_arg)
                    {
                        DeckOfCards::handSize = std::stoi(optarg);
                        std::cout << "Being told to draw " << DeckOfCards::handSize << " cards." << std::endl;
                    }

                break;
                case 'p':
                    if (long_options[option_index].has_arg)
                    {
                        DeckOfCards::numPlayers = std::stoi(optarg);
                        std::cout << "There are " << DeckOfCards::numPlayers << " players." << std::endl;
                    }
                break;
                case 'h':
                case '?':
                    std::cout << "Help message." << std::endl;
                break;
                default:
                break;
            }
        }
    }
    else
    {
        /* code */
    }

    return;
}

int main(int argc, char* argv[])
{
    DeckOfCards::ProcessArgs(argc, argv);

    DeckOfCards deck;

    deck.Shuffle();
    std::deque<CARDS> hands(DeckOfCards::numPlayers);

    for (int i; i<DeckOfCards::handSize; i++)
    {
        for (std::deque<CARDS>::iterator it=hands.begin(); 
             it != hands.end();
             it++
            )    
            {
                (*it).push_back(deck.DrawCard());
            }
    } 

    for (std::deque<CARDS>::iterator it=hands.begin(); 
            it != hands.end();
            it++
        )    
        {
            deck.PrintCards((*it));
        }

    return 0;
}