#pragma once
#include <string>
#include <vector>
#include <raylib.h>

namespace Utils
{
void DrawTextWithNewLines(const std::string &text, int x, int y, int fontSize, int maxWidth, Color color);
void DrawTextWithNewLinesCentered(const std::string &text, int x, int y, int fontSize, int maxWidth, Color color);
void DrawTextCentered(std::string_view text, int y, int fontSize, Color color);
void DrawTextCentered(std::string_view text, int x, int y, int w, int h, int fontSize, Color color);
void DrawHoverButton(const std::string &button, const std::string &hoveredButton, Rectangle rec);
void CropPixelsToCircle(uint8_t *pixels, bool *transparent, int width, int height, float centerX, float centerY,
                        float radius);
Image RoundImage(Image image);
}
