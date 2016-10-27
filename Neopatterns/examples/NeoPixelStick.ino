#include <NeoPatterns.h>

void StickComplete();

// Define some NeoPatterns for the stick
//  as well as some completion routines

NeoPatterns Stick(16, 7, NEO_GRB + NEO_KHZ800, &StickComplete);

// Initialize everything and prepare to start
void setup()
{
    Serial.begin(115200);

    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);

    // Initialize all the pixelStrips
    Stick.begin();

    // Kick off a pattern
    Stick.ConfigureScanner(Ring1.Color(255, 0, 0), 55);
}

// Main loop
void loop()
{
    // Update the rings.

    // Switch patterns on a button press:
    if (digitalRead(8) == LOW) // Button #1 pressed
    {
        // Set stick to all red
        Stick.ColorSet(Stick.Color(255, 0, 0));
    }
    else if (digitalRead(9) == LOW) // Button #2 pressed
    {
        Stick.Update();
    }
    else // Back to normal operation
    {
        Stick.Update();
    }
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// Stick Completion Callback
void StickComplete()
{
    // Random color change for next scan
    Stick.Color1 = Stick.Wheel(random(255));
}