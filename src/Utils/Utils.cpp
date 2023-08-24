#include "Utils.hpp"
#include "Graphics/TextureManager.hpp"

#include <sstream>
#include <iostream>

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

void DrawTextWithNewLinesCentered(const std::string &text, int x, int y, int fontSize, int maxWidth, Color color)
{
    int currentY = y;
    int currentX = x;

    std::istringstream iss(text);
    std::string word;
    std::string temp;
    while (iss >> word)
    {
        int wordWidth = MeasureText(word.data(), fontSize);
        if (currentX + wordWidth >= x + maxWidth)
        {
            int length = MeasureText(temp.data(), fontSize);
            int newX = x + (maxWidth / 2 - length / 2);

            DrawText(temp.data(), newX, currentY, fontSize, color);

            currentX = x;
            currentY += fontSize;
            temp = word + " ";
        }
        else
        {
            temp += word + " ";
            currentX += wordWidth + MeasureText(" ", fontSize);
        }
    }

    int length = MeasureText(temp.data(), fontSize);
    int newX = x + (maxWidth / 2 - length / 2);

    DrawText(temp.data(), newX, currentY, fontSize, color);
}

void DrawTextCentered(std::string_view text, int y, int fontSize, Color color)
{
    int length = MeasureText(text.data(), fontSize);
    int x = GetScreenWidth() / 2 - length / 2;

    DrawText(text.data(), x, y, fontSize, color);
}

void DrawTextCentered(std::string_view text, int x, int y, int w, int h, int fontSize, Color color)
{
    int length = MeasureText(text.data(), fontSize);
    int newX = x + (w / 2 - length / 2);
    int newY = y + (h / 2 - fontSize / 2);

    DrawText(text.data(), newX, newY, fontSize, color);
}

void DrawHoverButton(const std::string &button, const std::string &hoveredButton, Rectangle rec)
{
    if (CheckCollisionPointRec(GetMousePosition(), rec))
    {
        DrawTexture(GetTextureManager()->Get(hoveredButton), (int)rec.x, (int)rec.y, WHITE);
        //SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        DrawTexture(GetTextureManager()->Get(button), (int)rec.x, (int)rec.y, WHITE);
        //SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void CropPixelsToCircle(uint8_t *pixels, bool *transparent, int width, int height, float centerX, float centerY,
                        float radius)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float dx = (float)x - centerX;
            float dy = (float)y - centerY;
            float distanceSquared = dx * dx + dy * dy;

            if (distanceSquared > radius * radius)
            {
                transparent[x + y * width] = true;
            }
            else if (distanceSquared >= (radius - 5) * (radius - 5))
            {
                // Set pixels on the outline to red (with the specified intensity)
                pixels[3 * (y * width + x)] = 23;      // Red component
                pixels[3 * (y * width + x) + 1] = 179; // Green component
                pixels[3 * (y * width + x) + 2] = 193; // Blue component
            }
        }
    }
}

Image RoundImage(Image image)
{
    bool *transparent = new bool[image.width * image.height];
    uint8_t *pixels = new uint8_t[4 * image.width * image.height];
    memset(transparent, 0, (size_t)image.width * image.height);
    memset(pixels, 0, 4 * (size_t)image.width * image.height);

    CropPixelsToCircle((uint8_t *)image.data, transparent, image.width, image.height, (float)image.width / 2.0f,
                       (float)image.height / 2.0f, (float)image.width / 2.0f);

    uint8_t *imageData = (uint8_t *)image.data;
    for (int y = 0; y < image.height; ++y)
    {
        for (int x = 0; x < image.width; ++x)
        {
            uint8_t r = imageData[3 * (y * image.width + x) + 0];
            uint8_t g = imageData[3 * (y * image.width + x) + 1];
            uint8_t b = imageData[3 * (y * image.width + x) + 2];

            pixels[4 * (y * image.width + x) + 0] = r;
            pixels[4 * (y * image.width + x) + 1] = g;
            pixels[4 * (y * image.width + x) + 2] = b;

            if (transparent[x + y * image.width])
            {
                pixels[4 * (y * image.width + x) + 3] = 0;
            }
            else
            {
                pixels[4 * (y * image.width + x) + 3] = 255;
            }
        }
    }

    delete[] transparent;

    Image newImage{};
    newImage.width = image.width;
    newImage.height = image.height;
    newImage.mipmaps = 1;
    newImage.data = pixels;
    newImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    return newImage;
}

}
