#pragma once
#include <string>
#include <raylib.h>

namespace Utils
{
void DrawTextWithNewLines(const std::string &text, int x, int y, int fontSize, int maxWidth, Color color);
}
