#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cassert>
#include <iostream>
#include <vector>
#include <string_view>
#include <bits/stdc++.h>
#include "random.h"
#include <algorithm>
#include <algorithm>        // for std::copy()
#include <cassert>          // for assert()
#include <initializer_list> // for std::initializer_list
#include <iostream>
#include <exception>
#include <functional>
class Creature
{
protected:
    std::string m_name{"John Doe"};
    char m_symbol{};
    int m_health{};
    int m_damage{};
    int m_goldCarrying{};

public:
    Creature(std::string_view name, char symbol, int health, int damage, int gold) : m_name{name}, m_symbol{symbol}, m_health{health}, m_damage{damage}, m_goldCarrying{gold}
    {
    }
    const std::string &getName() const
    {
        return m_name;
    }
    int getHealth() const
    {
        return m_health;
    }
    char getSymbol() const
    {
        return m_symbol;
    }
    int getDamage() const
    {
        return m_damage;
    }
    int getGold() const
    {
        return m_goldCarrying;
    }
    void reduceHealth(int reduction)
    {
        m_health -= reduction;
    }
    bool isDead()
    {
        return m_health <= 0;
    }
    void addGold(int addition)
    {
        m_goldCarrying += addition;
    }
};
class Player : public Creature
{
    int m_level{1};

public:
    Player(std::string_view name)
        : Creature{name, '@', 10, 1, 0}
    {
    }

    void levelUp()
    {
        ++m_level;
        ++m_damage;
    }

    int getLevel() const { return m_level; }
    bool hasWon() const { return m_level >= 20; }
};
class Monster : public Creature
{
public:
    enum Type
    {
        dragon,
        orc,
        slime,
        maxMonsters,
    };
    const static inline Creature monsterData[maxMonsters]{{"dragon", 'D', 20, 4, 100}, {"orc", 'o', 4, 2, 25}, {"slime", 's', 1, 1, 10}};
    Monster(Type type) : Creature(monsterData[type])
    {
    }
    Monster(Monster &) = default;
    static Monster getRandomMonster()
    {
        int num{Random::get(0, maxMonsters - 1)};
        return Monster{static_cast<Type>(num)};
    }
};
char getPlay(std::string_view prompt)
{
    char attempt{};
    bool isFirstAttempt{true};
    do
    {
        if (!isFirstAttempt)
        {
            std::cin.clear();
            std::cout << "That wasn't a valid input.  Try again.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            isFirstAttempt = false;
        }
        std::cout << prompt;
        std::cin >> attempt;

    } while (!std::cin || (attempt != 'r' && attempt != 'f'));
    
    return attempt;
}
// Returns if player survived
bool attackPlayer(Player &player, Monster &monster)
{
    player.reduceHealth(monster.getDamage());
    std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";
    return player.getHealth() > 0;
}
// Returns if monster died
bool attackMonster(Player &player, Monster &monster)
{
    monster.reduceHealth(player.getDamage());
    std::cout << "You hit the " << monster.getName() << " for " << player.getDamage() << " damage.\n";
    if (monster.getHealth() <= 0)
    {
        std::cout << "The " << monster.getName() << " died.\n";
        player.levelUp();
        player.addGold(monster.getGold());
        return false;
    }
    return true;
}
bool fightMonster(Player& player, Monster& monster, char action){
    
    bool randomBool{Random::getBoolean()};
    if (action == 'r' && randomBool)
    {
        std::cout << "You successfully fled. \n";
        return true;
    }
    else if (action == 'r' && !randomBool)
    {
        std::cout << "You failed to flee.\n";
        action='f';
        
            bool survived{attackPlayer(player, monster)};
            if (!survived)
            {
                std::cout << "You died at level " << player.getLevel() << " and damage " << player.getDamage()<<'\n';
                return false;
            }
            
            action=getPlay("(R)un or (F)ight: ");
            
            return fightMonster(player,monster,action);
            
        
    }
    
        while (action=='f')
        {
            bool monsterSurvived{attackMonster(player, monster)};
            if (!monsterSurvived)
            {
                return true;
            }
            bool survived{attackPlayer(player, monster)};
            if (!survived)
            {
                std::cout << "You died at level " << player.getLevel() << " and damage " << player.getDamage()<<'\n';
                return false;
            }
            action=getPlay("(R)un or (F)ight: ");
            if(action=='r'){
                return fightMonster(player,monster,action);
            }
        }
        return !player.isDead();
}
// Returns if player survived
bool fightMonster(Player& player)
{
    Monster monster{Monster::getRandomMonster()};
    std::cout << "You have encountered a " << monster.getName()<<'\n';
    char action=getPlay("(R)un or (F)ight: ");
    return fightMonster(player,monster,action);
}

int main()
{
    std::cout << "Enter your name";
    std::string name{"John Doe"};
    std::cin >> name;
    Player player{name};
    while(fightMonster(player)){
        std::cout<<player.getName()<<"(health, level, damage, gold) "<<player.getHealth()<<", "<<player.getLevel()<<", "<<player.getDamage()<<", "<<player.getGold()<<"\n";
    }
    std::cout<<"Game over";
    return 0;
}