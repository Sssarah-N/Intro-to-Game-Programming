#include "Dialoge.h"

Dialoge::Dialoge (std::vector<std::string> lines, std::string name) 
    : lines(lines), name(name), currline(0), revealedChars(0), 
      typingTimer(0.0f), charsPerSecond(30.0f), lineFullyRevealed(false), 
      typingSound(LoadSound("assets/OSD.wav")) {
        if (name == "character") {
            box = LoadTexture("assets/dialoge_box.png");
        } 
        else {
            box = LoadTexture("assets/dialoge_box.png");
            namebox = LoadTexture("assets/nametag.png");
        }

        SetSoundVolume(typingSound, 1.0f);

      }
      
    

Dialoge::~Dialoge() {
    UnloadTexture(box);
    if (typingSound.frameCount > 0) {
        StopSound(typingSound);
        UnloadSound(typingSound);
    }
}

std::vector<std::string> Dialoge::wrapText(const std::string& text, float maxWidth, int fontSize) {
    std::vector<std::string> wrappedLines;    
    std::string currentLine = "";
    std::string word = "";
    
    for (size_t i = 0; i < text.length(); i++) {
        char c = text[i];
        if (c == ' ' || i == text.length() - 1) {
            if (i == text.length() - 1 && c != ' ') {
                word += c;
            }
            std::string thisline = currentLine;
            if (!thisline.empty()) {
                thisline += " ";
            }
            thisline += word;
            
            if (MeasureText(thisline.c_str(), fontSize) <= maxWidth) {
                currentLine = thisline;
            } else {
                // doesn't fit
                if (!currentLine.empty()) {
                    wrappedLines.push_back(currentLine);
                    currentLine = word;
                } else {
                    currentLine = word;
                }
            }
            
            word = "";
        } else {
            word += c;
        }
    }
    
    if (!currentLine.empty()) {
        wrappedLines.push_back(currentLine);
    }
    
    return wrappedLines;
}

void Dialoge::nextLine() {
    currline++;
    revealedChars = 0;
    typingTimer = 0.0f;
    lineFullyRevealed = false;
    
    if (currline < lines.size()) {
        PlaySound(typingSound);
    }
}

void Dialoge::update(float deltaTime) {
    if (currline >= lines.size() || lineFullyRevealed) return;
    if (revealedChars == 0 && !IsSoundPlaying(typingSound)) {
        PlaySound(typingSound);
    }
    
    // Advance the typing timer
    typingTimer += deltaTime;
    
    // Calculate how many characters should be revealed
    float charsToReveal = typingTimer * charsPerSecond;
    revealedChars = (size_t)charsToReveal;
    
    // Check if we've revealed the entire line
    if (revealedChars >= lines[currline].length()) {
        revealedChars = lines[currline].length();
        lineFullyRevealed = true;
        StopSound(typingSound);
    }
}

void Dialoge::skipTyping() {
    if (currline < lines.size()) {
        revealedChars = lines[currline].length();
        lineFullyRevealed = true;
        StopSound(typingSound);
    }
}

bool Dialoge::isLineComplete() const {
    return lineFullyRevealed;
}

bool Dialoge::isDialogueComplete() const {
    return currline >= lines.size();
}

void Dialoge::render() {
    if (currline >= lines.size()) return;
    
    // Draw the dialogue box
    Rectangle source = {0, 0, (float)box.width, (float)box.height};
    Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
    DrawTexturePro(box, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);

    int fontSize = 35;
    float padding = 110.0f; 
    float maxTextWidth = GetScreenWidth() - (padding * 2);
    std::string revealedText = lines[currline].substr(0, revealedChars);
    std::vector<std::string> wrappedLines = wrapText(revealedText, maxTextWidth, fontSize);
    
    float startX = padding;
    float startY = GetScreenHeight() * 0.7f; 
    
    for (size_t i = 0; i < wrappedLines.size(); i++) {
        DrawText(
            wrappedLines[i].c_str(), 
            (int)startX, 
            (int)(startY + i * 35.0f), 
            fontSize, 
            BLACK
        );
    }

    if (name != "character") {
        Rectangle source = {0, 0, (float)namebox.width, (float)namebox.height};
        Rectangle dest = {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        DrawTexturePro(namebox, source, dest, {0.0f, 0.0f}, 0.0f, WHITE);
        DrawText(
            name.c_str(), 
            (int) startX + 20, 
            (int) startY - 120, 
            fontSize, 
            BLACK
        );
    }
}