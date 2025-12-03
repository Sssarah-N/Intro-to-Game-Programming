#ifndef CHOICEDIALOGUE_H
#define CHOICEDIALOGUE_H

#include "cs3113.h"
#include <vector>
#include <string>

class ChoiceDialogue
{
private:
    std::vector<std::string> choices;
    int selectedChoice;
    std::vector<Rectangle> choiceBoxes;
    Texture2D box;
    std::string name;
    
public:
    ChoiceDialogue(std::string name, const std::vector<std::string>& options);
    ~ChoiceDialogue();
    
    void render();
    void handleInput();
    int getSelectedChoice() const { return selectedChoice; }
    bool hasSelectedChoice() const { return selectedChoice != -1; }
    void reset() { selectedChoice = -1; choiceBoxes.clear(); }
    bool finished();
    std::string getName() const;
};

#endif

