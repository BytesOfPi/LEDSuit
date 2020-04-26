
#ifndef Component_H
#define Component_H

#include "../Shared/Share.h"
class ComponentLED
{
public:
    String name;
    CPattern *patt;

    ComponentLED(String nameIn, CPattern *pattIn)
    {
        Serial.print("ComponentLED constructor");
        name = nameIn;
        //--------------------------------------------------------------
        // Store pattern data
        patt = pattIn;
    }

    virtual String getCurrentPatt() { return patt->getPattern(); };

    virtual unsigned int drawFrame()
    {
        //------------------------------------------------------------
        // Adjust pattern periodically
        periodicAdjust();

        //------------------------------------------------------------
        // If pattern isn't cycle and it's a new pattern, we need to
        // alert everyone and switch
        if (!patt->isOnCycle() && patt->isNewPattern())
        {
            switchPattern();
        }

        //------------------------------------------------------------
        // Draw the current pattern
        return drawPattern(getCurrentPatt());
    };

    virtual void printCurrentPattern()
    {
        Serial.print(name); Serial.print(": [");
        Serial.print(getCurrentPatt()); Serial.print("][");
        Serial.print(patt->thisPattern); Serial.println("]");
    }

    virtual unsigned int drawPattern(String patt) { return 0; }

    /**
      * switchPattern()
      * This method is called every time there is a pattern switch.  This will allow
      * us to reset any stored values that patterns use
      */
    virtual void switchPattern() { printCurrentPattern(); }

    /**
      * periodicAdjust()
      * This method is called every time drawFrame() is called and sets up periodic
      * adjustments.
      */
    virtual void periodicAdjust()
    {
        //--------------------------------------------------------------
        // Switch cycle() pattern every 10 seconds
        EVERY_N_SECONDS(10)
        {
            printCurrentPattern();
            patt->cycleNext();
            switchPattern();
        }
    }
};

#endif