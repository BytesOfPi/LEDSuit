
#ifndef Component_H
#define Component_H

#include "../Shared/Share.h"
class ComponentLED
{
private:
    String *cycleSkip;          // array of skipped patterns
    uint16_t numSkip;           // number of skipped patterns
public:
    uint8_t cyclePattIndex = 0; // determine which pattern
    String name;
    String currentPatt;
    String thisPatt;
    String *patterns; // array of string patterns
    uint16_t numPatt;

    ComponentLED(String nameIn, String &currentPattIn, String patternsIn[], uint16_t numPattIn, String cycleSkipIn[], uint16_t numSkipIn)
    {
        Serial.print("ComponentLED constructor");
        name = nameIn;
        //--------------------------------------------------------------
        // Store Component Data
        currentPatt = currentPattIn;
        patterns = patternsIn;
        numPatt = numPattIn;
        cycleSkip = cycleSkipIn;
        numSkip = numSkipIn;
    }

    virtual unsigned int drawFrame()
    {
        //------------------------------------------------------------
        // Adjust pattern periodically
        periodicAdjust();

        //------------------------------------------------------------
        // If pattern isn't cycle and it doesn't match this class pattern,
        // we switched patterns and need to alert everyone
        if (!currentPatt.equals("cycle") && !currentPatt.equals(thisPatt))
        {
            switchPattern();
            thisPatt = currentPatt;
        }

        return drawPattern(currentPatt);
    };

    virtual unsigned int drawPattern(String patt) { return 0; }

    /**
      * switchPattern()
      * This method is called every time there is a pattern switch.  This will allow
      * us to reset any stored values that patterns use
      */
    virtual void switchPattern() {}

    virtual void periodicAdjust()
    {
        //--------------------------------------------------------------
        // Switch cycle() pattern every 10 seconds
        EVERY_N_SECONDS(10)
        {
            cycleNext();
        }
    }

    /**
      * cycle()
      * Tell object to draw the next safe cycled pattern
      */
    virtual int cycle()
    {
        return drawPattern(patterns[cyclePattIndex]);
    }

    /**
      * cycleSkipPatt()
      * Check if the pattern is in the list of patterns that should be
      * skipped.
      */
    virtual bool cycleSkipPatt(String patt)
    {
        for (uint8_t i = 0; i < numSkip; i++)
        {
            if (cycleSkip[i].equals(patt))
                return true;
        }
        return false;
    }

    /**
      * cycleNext()
      * Go to the next safe cycle pattern index
      */
    virtual void cycleNext()
    {
        //------------------------------------------------------------
        // Get next safe index
        cyclePattIndex = (cyclePattIndex == numPatt - 1) ? 0 : cyclePattIndex + 1;

        //------------------------------------------------------------
        // while the pattern is in the skip list, increment to next
        // pattern
        while (cycleSkipPatt(patterns[cyclePattIndex]))
        {
            cyclePattIndex = (cyclePattIndex == numPatt - 1) ? 0 : cyclePattIndex + 1;
        }
        //------------------------------------------------------------
        // Alert everyone we're going to the next pattern
        switchPattern();
    }
};

#endif