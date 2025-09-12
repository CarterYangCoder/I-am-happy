# Makefile for the game project
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TARGET = game
SRCDIR = .
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# 确保所有源文件都被包含
SOURCES = main.cpp Game.cpp Player.cpp Attribute.cpp Map.cpp Room.cpp \
          NPC.cpp EvilGeneral.cpp CommonEnemy.cpp BossWanEshuji.cpp \
          Equipment.cpp DivineWeapon.cpp DivineSet.cpp Skills.cpp \
          Item.cpp TaskSystem.cpp ShopSystem.cpp CommandParser.cpp \
          CombatSystem.cpp SaveLoadSystem.cpp UIManager.cpp GameData.cpp \
          MainMenu.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild

# 依赖关系
main.o: main.cpp Game.h MainMenu.h TaskSystem.h
Game.o: Game.h Player.h Map.h CombatSystem.h TaskSystem.h ShopSystem.h CommandParser.h SaveLoadSystem.h UIManager.h
MainMenu.o: MainMenu.h UIManager.h SaveLoadSystem.h TaskSystem.h Player.h
Player.o: Player.h Attribute.h Equipment.h DivineWeapon.h Skills.h TaskSystem.h DivineSet.h
Attribute.o: Attribute.h GameData.h
Map.o: Map.h Room.h NPC.h EvilGeneral.h CommonEnemy.h BossWanEshuji.h SaveLoadSystem.h
Room.o: Room.h
NPC.o: NPC.h TaskSystem.h UIManager.h
EvilGeneral.o: EvilGeneral.h CommonEnemy.h GameData.h Item.h
CommonEnemy.o: CommonEnemy.h Attribute.h
BossWanEshuji.o: BossWanEshuji.h EvilGeneral.h
Equipment.o: Equipment.h Item.h GameData.h
DivineWeapon.o: DivineWeapon.h Equipment.h Skills.h
DivineSet.o: DivineSet.h Equipment.h
Skills.o: Skills.h GameData.h
Item.o: Item.h
TaskSystem.o: TaskSystem.h UIManager.h Player.h Equipment.h
ShopSystem.o: ShopSystem.h Item.h UIManager.h Player.h
CommandParser.o: CommandParser.h
CombatSystem.o: CombatSystem.h UIManager.h Player.h BossWanEshuji.h
SaveLoadSystem.o: SaveLoadSystem.h Player.h UIManager.h TaskSystem.h Equipment.h
UIManager.o: UIManager.h Player.h
GameData.o: GameData.h