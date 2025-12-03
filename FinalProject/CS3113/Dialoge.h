#include "cs3113.h"

#ifndef DIALOGE_H
#define DIALOGE_H


class Dialoge
{
private:
    std::vector<std::string> lines;
    size_t currline;
    std::string name;
    Texture2D box;
    Texture2D namebox;
    Sound typingSound;
    
    // Typing effect variables
    size_t revealedChars;           // Number of characters currently revealed
    float typingTimer;              // Timer for typing effect
    float charsPerSecond;           // Typing speed
    bool lineFullyRevealed;         // Whether current line is fully shown

    
    // Helper function to wrap text to fit within a given width
    std::vector<std::string> wrapText(const std::string& text, float maxWidth, int fontSize);
public:
    Dialoge (std::vector<std::string> lines, std::string name);
    ~Dialoge();

    void nextLine();
    void update(float deltaTime);   // Update typing effect
    void render();
    void skipTyping();              // Instantly reveal all text
    bool isLineComplete() const;    // Check if current line typing is done
    bool isDialogueComplete() const; // Check if all lines have been shown
};

#endif