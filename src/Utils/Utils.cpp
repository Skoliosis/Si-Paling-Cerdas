#include "Utils.hpp"
#include <sstream>

namespace Utils
{
void DrawTextWithNewLines(const std::string &text, int x, int y, int fontSize, int maxWidth, Color color)
{
    int currentY = y;
    int currentX = x;

    std::istringstream iss(text);
    std::string word;
    while (iss >> word)
    {
        int wordWidth = MeasureText(word.data(), fontSize);
        if (currentX + wordWidth >= x + maxWidth)
        {
            // word won't fit on current line, move to next line
            currentX = x;
            currentY += fontSize;
        }

        DrawText(word.data(), currentX, currentY, fontSize, color);
        currentX += wordWidth + MeasureText(" ", fontSize);
    }
}

void DrawTextCentered(const char* text, int y, int fontSize, Color color)
{
    int length = MeasureText(text, fontSize);
    int x = GetScreenWidth() / 2 - length / 2;

    DrawText(text, x, y, fontSize, color);
}

}
