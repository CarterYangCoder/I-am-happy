#ifndef MAINMENU_H
#define MAINMENU_H

#include "UIManager.h"
#include "SaveLoadSystem.h"
#include "Player.h"
#include "TaskSystem.h"

enum class MenuChoice {
    START_GAME = 1,
    LOAD_GAME = 2,
    SHOW_HELP = 3,
    EXIT_GAME = 4
};

class MainMenu {
private:
    UIManager ui;
    
    void initializeConsole() const;
    void clearScreen() const;
    bool checkColorSupport() const;
    void displayLogo() const;
    void displayMenuOptions() const;
    MenuChoice getPlayerChoice() const;
    void showHelp() const;
    bool handleLoadGame(Player& player, TaskSystem& tasks) const;

public:
    MainMenu();
    MenuChoice run(Player& player, TaskSystem& tasks);
};

#endif // MAINMENU_H
