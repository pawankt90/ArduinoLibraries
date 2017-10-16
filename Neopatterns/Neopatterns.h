#ifndef NEOPATTERNS_H
#define NEOPATTERNS_H

#include <Adafruit_NeoPixel.h>

/*
@typedef
    enum
@brief
    Used to specify which pattern to use
*/
enum  Pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, THEATER_CHASE_RAINBOW, COLOR_WIPE, SCANNER, FADE };

/*
@typedef
    enum
@brief
    Specifies direction of movement. Used for patterns like rainbow cycle and pattern chase
*/
enum  Direction { FORWARD, REVERSE };

/*
@typedef
    enum
@brief
    Enum for known colors
*/
typedef enum  KnownColors { 
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    PURPLE
};

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
public:

    // Member Variables:  
    Pattern         ActivePattern;      // which pattern is running
    Direction       PatternDirection;   // direction to run the pattern

    unsigned long   DelayTime;           // milliseconds between updates
    unsigned long   lastUpdate;         // last update of position

    uint32_t        Color1;
    uint32_t        Color2;  
    int16_t			TotalSteps;          // total number of steps in the pattern
    int16_t			CurStepIdx;          // current step within the pattern
	int8_t		RainbowTheaterIter;

    void(*OnComplete)();                // Callback on completion of pattern

    /*
    @brief
        Constructor of NeoPatterns, initializes parameters and sets a callback function, if passed

    @param
        None

    @return
        None
    */
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void(*callback)() = []() {});

    /*
    @brief
        Function to execute the function corresponding to the pattern that needs to be drawn

    @param
        None

    @return
        None
    */
    void PerformPattern();

    /*
    @brief
        Depending on the direction, increments or decrements index

    @param
        None

    @return
        None
    */
    void Increment();

    /*
    @brief
        Reverse Pattern Direction

    @param
        None

    @return
        None
    */
    void Reverse();

    /*
    @brief
        Sets RAINBOW_CYCLE as the active pattern and initializes parameters pertaining to the pattern
        Note: Does not draw pattern!
    @param
        [IN]    interval        - Delay time between cycles
        [IN]    dir             - Direction to perform rainbow cycle in

    @return
        None
    */
    void ConfigureRainbowCycle(uint32_t interval, Direction dir = FORWARD);

    /*
    @brief
        Draws/executes the rainbow cycle pattern

    @param
        None

    @return
        None
    */
    void PerformRainbowCycle();

    /*
    @brief
        Sets THEATER_CHASE as the active pattern and configures parameters pertaining to the pattern.
        Note: Does not draw pattern!

    @param
        [IN]    color1          - First color
        [In]    color2          - Second color
        [In]    interval        - Delay time between chases
        [In]    dir             - direction to perform theater chase in

    @return
        None
    */
    void ConfigureTheaterChase(uint32_t color1, uint32_t color2, uint32_t interval, Direction dir = FORWARD);

    /*
    @brief
        Draws/executes the theater chase pattern

    @param
        None

    @return
        None
    */
    void PerformTheaterChase();

	/*
	@brief
	Sets THEATER_CHASE_RAINBOW as the active pattern and configures parameters pertaining to the pattern.
	Note: Does not draw pattern!

	@param
	[IN]    color1          - First color
	[In]    color2          - Second color
	[In]    interval        - Delay time between chases
	[In]    dir             - direction to perform theater chase in

	@return
	None
	*/
	void ConfigureTheaterChaseRainbow(uint32_t interval, Direction dir = FORWARD);

	/*
	@brief
	Draws/executes the theater chase rainbow pattern

	@param
	None

	@return
	None
	*/
	void PerformTheaterChaseRainbow();

    /*
    @brief
        Sets COLOR_WIPE as the active pattern and configures parameters pertaining to the pattern.
        Note: Does not draw pattern!

    @param
        [IN]    color           - Color to wipe across pixels
        [In]    interval        - Delay time between each neopixel to wipe
        [In]    dir             - direction to perform color wipe in

    @return
        None
    */

    void ConfigureColorWipe(uint32_t color, uint32_t interval, Direction dir = FORWARD);

    /*
    @brief
        Draws/executes the color wipe pattern

    @param
        None

    @return
        None
    */
    void PerformColorWipe();

    /*
    @brief
        Sets SCANNER as the active pattern and configures parameters pertaining to the pattern.
        Note: Does not draw pattern!

    @param
        [IN]    color1          - Color to perform scan with
        [In]    interval        - delay time between scanner

    @return
        None
    */
    void ConfigureScanner(uint32_t color1, uint32_t interval);

    /*
    @brief
        Draws/executes the scanner pattern

    @param
        None

    @return
        None
    */
    void PerformScanner();

    /*
    @brief
        Sets FADE as the active pattern and configures parameters pertaining to the pattern.
        Note: Does not draw pattern!

    @param
        [IN]    color1          - First color
        [In]    color2          - Second color
        [In]    steps           - Number of steps to take when fading, denotes speed of fade
        [In]    interval        - Delay time between fades
        [In]    dir             - direction to fade in

    @return
        None
    */
    void ConfigureFade(uint32_t color1, uint32_t color2, uint16_t steps, uint32_t interval, Direction dir = FORWARD);

    /*
    @brief
        Draws/executes the scanner pattern

    @param
        None

    @return
        None
    */
    void PerformFade();

    /*
    @brief
        Used by perform scanner. This function calculates 50% dimmed version of a color.

    @param
        [IN] Color      -   Color to calculate dim for

    @return
        None
    */
    uint32_t DimColor(uint32_t color);

    /*
    @brief
        Sets all pixels to the same color with no delay

    @param
        [IN] Color      -   Color to set

    @return
        None
    */
    void ColorSet(uint32_t color);

    /*
    @brief
        Returns the red component of the specified color

    @param
        [IN] Color      -   Color to get the red component from

    @return
        uint32_t value corresponding to the red component of the color
    */
    uint8_t GetRedComponent(uint32_t color);

    /*
    @brief
        Returns the green component of the specified color

    @param
        [IN] Color      -   Color to get the green component from

    @return
        uint32_t value corresponding to the green component of the color
    */
    uint8_t GetGreenComponent(uint32_t color);

    /*
    @brief
        Returns the blue component of the specified color

    @param
        [IN] Color      -   Color to get the blue component from

    @return
        uint32_t value corresponding to the blue component of the color
    */
    uint8_t GetBlueComponent(uint32_t color);

    /*
    @brief
        Returns the uint32_t value of a color within a 0-255 range of values. 0 is red and 255 is blue. This is a cyclic
        calculation to avoid end-of-range issues.

    @param
        [IN] WheelPos      -   Color to get the red component from

    @return
        uint32_t value of corresponding color.
    */
    uint32_t Wheel(byte WheelPos);
};

/*
@brief
    Returns the uint32_t value for the known color

@param
    [IN] Color      -   enum of KnownColors

@return
    uint32_t value corresponding to the blue component of the color
*/
uint32_t GetKnownColorValue(KnownColors color);

#endif