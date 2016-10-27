#include <Neopatterns.h>

NeoPatterns::NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void(*callback)()):Adafruit_NeoPixel(pixels, pin, type)
{
    OnComplete = callback;
}

void NeoPatterns::PerformPattern()
{
    if ((millis() - lastUpdate) > DelayTime) // time to update
    {
        lastUpdate = millis();
        switch (ActivePattern)
        {
        case RAINBOW_CYCLE:
            PerformRainbowCycle();
            break;
        case THEATER_CHASE:
            PerformTheaterChase();
            break;
        case COLOR_WIPE:
            PerformColorWipe();
            break;
        case SCANNER:
            PerformScanner();
            break;
        case FADE:
            PerformFade();
            break;
        default:
            break;
        }
    }
}

void NeoPatterns::Increment()
{
    if (PatternDirection == FORWARD)
    {
        CurStepIdx++;
        if (CurStepIdx >= TotalSteps)
        {
            CurStepIdx = 0;
            if (OnComplete != NULL)
            {
                OnComplete(); // call the completion callback
            }
        }
    }
    else // Direction == REVERSE
    {
        --CurStepIdx;
        if (CurStepIdx <= 0)
        {
            CurStepIdx = TotalSteps - 1;
            if (OnComplete != NULL)
            {
                OnComplete(); // call the completion callback
            }
        }
    }
}

void NeoPatterns::Reverse()
{
    if (PatternDirection == FORWARD)
    {
        PatternDirection = REVERSE;
        CurStepIdx = TotalSteps - 1;
    }
    else
    {
        PatternDirection = FORWARD;
        CurStepIdx = 0;
    }
}


void NeoPatterns::ConfigureRainbowCycle(uint32_t interval, Direction dir)
{
    ActivePattern = RAINBOW_CYCLE;
    DelayTime = interval;
    TotalSteps = 255;
    CurStepIdx = 0;
    PatternDirection = dir;
}

void NeoPatterns::PerformRainbowCycle()
{
    for (int i = 0; i < numPixels(); i++)
    {
        setPixelColor(i, Wheel(((i * 256 / numPixels()) + CurStepIdx) & 255));
    }
    show();
    Increment();
}

void NeoPatterns::ConfigureTheaterChase(uint32_t color1, uint32_t color2, uint32_t interval, Direction dir)
{
    ActivePattern = THEATER_CHASE;
    DelayTime = interval;
    TotalSteps = numPixels();
    Color1 = color1;
    Color2 = color2;
    CurStepIdx = 0;
    PatternDirection = dir;
}

void NeoPatterns::PerformTheaterChase()
{
    for (int i = 0; i < numPixels(); i++)
    {
        if ((i + CurStepIdx) % 3 == 0)
        {
            setPixelColor(i, Color1);
        }
        else
        {
            setPixelColor(i, Color2);
        }
    }
    show();
    Increment();
}

void NeoPatterns::ConfigureColorWipe(uint32_t color, uint32_t interval, Direction dir)
{
    ActivePattern = COLOR_WIPE;
    DelayTime = interval;
    TotalSteps = numPixels();
    Color1 = color;
    CurStepIdx = 0;
    PatternDirection = dir;
}

void NeoPatterns::PerformColorWipe()
{
    setPixelColor(CurStepIdx, Color1);
    show();
    Increment();
}

void NeoPatterns::ConfigureScanner(uint32_t color1, uint32_t interval)
{
    ActivePattern = SCANNER;
    DelayTime = interval;
    TotalSteps = (numPixels() - 1) * 2;
    Color1 = color1;
    CurStepIdx = 0;
}

void NeoPatterns::PerformScanner()
{
    for (int i = 0; i < numPixels(); i++)
    {
        if (i == CurStepIdx)  // Scan Pixel to the right
        {
            setPixelColor(i, Color1);
        }
        else if (i == TotalSteps - CurStepIdx) // Scan Pixel to the left
        {
            setPixelColor(i, Color1);
        }
        else // Fading tail
        {
            setPixelColor(i, DimColor(getPixelColor(i)));
        }
    }
    show();
    Increment();
}

void NeoPatterns::ConfigureFade(uint32_t color1, uint32_t color2, uint16_t steps, uint32_t interval, Direction dir = FORWARD)
{
    ActivePattern = FADE;
    DelayTime = interval;
    TotalSteps = steps;
    Color1 = color1;
    Color2 = color2;
    CurStepIdx = 0;
    PatternDirection = dir;
}


void NeoPatterns::PerformFade()
{
    // Calculate linear interpolation between Color1 and Color2
    // Optimize order of operations to minimize truncation error
    uint8_t red = ((GetRedComponent(Color1) * (TotalSteps - CurStepIdx)) + (GetRedComponent(Color2) * CurStepIdx)) / TotalSteps;
    uint8_t green = ((GetGreenComponent(Color1) * (TotalSteps - CurStepIdx)) + (GetGreenComponent(Color2) * CurStepIdx)) / TotalSteps;
    uint8_t blue = ((GetBlueComponent(Color1) * (TotalSteps - CurStepIdx)) + (GetBlueComponent(Color2) * CurStepIdx)) / TotalSteps;

    ColorSet(Color(red, green, blue));
    show();
    Increment();
}

uint32_t NeoPatterns::DimColor(uint32_t color)
{
    // Shift R, G and B components one bit to the right
    uint32_t dimColor = Color(GetRedComponent(color) >> 1, GetGreenComponent(color) >> 1, GetBlueComponent(color) >> 1);
    return dimColor;
}

void NeoPatterns::ColorSet(uint32_t color)
{
    for (int curPixel = 0; curPixel < numPixels(); curPixel++)
    {
        setPixelColor(curPixel, color);
    }
    show();
}

uint8_t NeoPatterns::GetRedComponent(uint32_t color)
{
    return (color >> 16) & 0xFF;
}

uint8_t NeoPatterns::GetGreenComponent(uint32_t color)
{
    return (color >> 8) & 0xFF;
}

uint8_t NeoPatterns::GetBlueComponent(uint32_t color)
{
    return color & 0xFF;
}

uint32_t NeoPatterns::Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}

uint32_t GetKnownColorValue(KnownColors color)
{
    uint8_t r = 0, g = 0, b = 0;

    switch (color)
    {
    case RED:
        r = 255;
        g = 0;
        b = 0;
        break;
    case ORANGE:
        r = 255;
        g = 128;
        b = 0;
        break;
    case YELLOW:
        r = 255;
        g = 255;
        b = 0;
        break;
    case GREEN:
        r = 0;
        g = 255;
        b = 0;
        break;
    case BLUE:
        r = 0;
        g = 0;
        b = 255;
        break;
    case PURPLE:
        r = 128;
        g = 0;
        b = 128;
        break;
    }

    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
