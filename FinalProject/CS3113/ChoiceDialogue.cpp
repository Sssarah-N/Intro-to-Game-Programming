#include "ChoiceDialogue.h"

ChoiceDialogue::ChoiceDialogue(std::string name, const std::vector<std::string>& options)
    : choices(options), selectedChoice(-1), name(name) {
    box = LoadTexture("assets/dialoge_box.png");
}

ChoiceDialogue::~ChoiceDialogue() {
    UnloadTexture(box);
}

void ChoiceDialogue::render() {
    Rectangle source = {0, 0, (float)box.width, (float)box.height};
    Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    DrawTexturePro(box, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
    
    // choice boxes
    float choiceStartY = GetScreenHeight() * 0.65f;
    float choiceHeight = 70.0f;
    float choiceSpacing = 25.0f;
    float choiceWidth = 700.0f;
    float choiceX = (GetScreenWidth() - choiceWidth) / 2.0f;
    
    choiceBoxes.clear();
    Vector2 mousePos = GetMousePosition();
    
    for (size_t i = 0; i < choices.size(); i++) {
        float y = choiceStartY + i * (choiceHeight + choiceSpacing);
        Rectangle choiceBox = {choiceX, y, choiceWidth, choiceHeight};
        choiceBoxes.push_back(choiceBox);
    
        // change bg color if mouse hovering over
        bool isHovered = CheckCollisionPointRec(mousePos, choiceBox);
        Color bgColor = isHovered ? ColorFromHex("#d4c5b9") : ColorFromHex("#eeede7");
        Color borderColor = BLACK;
        
        // Draw choice box
        DrawRectangleRec(choiceBox, bgColor);
        DrawRectangleLinesEx(choiceBox, 3, borderColor);
        
        // Draw choice text (centered)
        int fontSize = 32;
        int textWidth = MeasureText(choices[i].c_str(), fontSize);
        DrawText(
            choices[i].c_str(),
            (int)(choiceBox.x + (choiceBox.width - textWidth) / 2),
            (int)(choiceBox.y + (choiceBox.height - fontSize) / 2),
            fontSize,
            BLACK
        );
    }
    
    // Draw instruction at top
    const char* instruction = "Choose an option:";
    int instructionWidth = MeasureText(instruction, 35);
    DrawText(
        instruction,
        (GetScreenWidth() - instructionWidth) / 2,
        (int)(choiceStartY - 80),
        35,
        BLACK
    );
}

void ChoiceDialogue::handleInput() {
    if (selectedChoice != -1) return; 
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition(); 
        for (size_t i = 0; i < choiceBoxes.size(); i++) {
            if (CheckCollisionPointRec(mousePos, choiceBoxes[i])) {
                selectedChoice = (int)i;
                break;
            }
        }
    }
}

bool ChoiceDialogue::finished() {
    return selectedChoice != -1; 
}

std::string ChoiceDialogue::getName() const {
    return name;
}

