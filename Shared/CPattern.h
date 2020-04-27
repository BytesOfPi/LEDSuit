/*
 * CPattern.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This class stores the globally held information about a given
 * component's pattern so it can be easily controlled with controllers
 * (like BLE or physical device) and responded to by the component.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CPattern_H
#define CPattern_H

//#################################################################
// Predefined methods to advance/go back on a given array without
// exceeding the bounds of the array assuming the array goes from
// 0 to max
#define SAFE_NEXT_PATTERN(val, max) (val + 1 >= max ? 0 : val + 1)
#define SAFE_PREV_PATTERN(val, max) (val == 0 ? max - 1 : val - 1)

/**
 * CPattern Class
 * This class stores the globally held information about a given
 * component's pattern so it can be easily controlled with controllers
 * (like BLE or physical device) and responded to by the component.
 */
class CPattern
{
private:
    /**
     * _indexOf()
     * This method loops through an array and determines if a String
     * is in that list.
     */
    int _indexOf(String val, String list[], uint16_t listSize)
    {
        for (int i = 0; i <= listSize; i++)
        {
            // if we find the selected pattern, set the index
            if (val.equalsIgnoreCase(list[i]))
            {
                return i;
            }
        }
        return -1;
    }

    /**
     * _changeIndex()
     * This method will find the next safe (non-skip list) index either going forward
     * or back through array (not exceeding array bounds) and set the index to that.
     */
    int _changeIndex(bool next)
    {
        // Go to the next safe pattern
        skipIndex = next ? SAFE_NEXT_PATTERN(skipIndex, numPatterns) : SAFE_PREV_PATTERN(skipIndex, numPatterns);

        // If we've indicated we want to skip patterns, keep incrementing
        // until we are not in the skip list
        while (_indexOf(patterns[skipIndex], cycleSkip, numCycleSkip) > -1)
        {
            skipIndex = next ? SAFE_NEXT_PATTERN(skipIndex, numPatterns) : SAFE_PREV_PATTERN(skipIndex, numPatterns);
        }
        // return the pattern
        return skipIndex;
    }

    /**
     * _getIntendPatt()
     * Utility knife that takes in a command and returns the related pattern
     * based on the selection.  It ALSO advances the current index IF the
     * selected command is an index that is safe (i.e. not in skip list)
     */
    int _getIntendPatt(String val)
    {
        //---------------------------------------------------------
        // If "back" or "next" is sent, go to the next pattern
        if (val.equalsIgnoreCase("next"))
            return _changeIndex(true);
        if (val.equalsIgnoreCase("back"))
            return _changeIndex(false);

        //---------------------------------------------------------
        // Otherwise, check if value is in list
        int iHold = _indexOf(val, patterns, numPatterns);
        if (iHold < 0)
        {
            Serial.print("     COULDN't FIND PATTERN: [");
            Serial.print(val);
            Serial.print("] iHold [");
            Serial.print(iHold);
            Serial.println("]");
            return 0;
        }
        bool inSkipList = _indexOf(patterns[iHold], cycleSkip, numCycleSkip) >= 0;
        // if the pattern found is NOT in skip list, set the class index

        if (!inSkipList)
        {
            skipIndex = iHold;
        }
        return iHold;
    }

public:
    //------------------------------------------------------------
    // Making all the attributes public now because it is easier
    // than writing getters and setters where necessary.  Probably
    // better to move most of these private.
    boolean isNew = false;
    int skipIndex = 0; // determine which pattern
    String name;       // name of pattern
    // String thisPattern;    // currently selected pattern
    int thisIndex = 0;     // currently selected pattern
    String *cycleSkip;     // array of skipped patterns
    uint16_t numCycleSkip; // number of skipped patterns
    String *patterns;          // array of string patterns
    uint16_t numPatterns;      // number of string patterns

    /**
     * Constructor
     * Initialize the component pattern
     */
    CPattern(String nameIn, String pattIn[], uint16_t numPattIn, String cycleSkipIn[], uint16_t numCycleSkipIn)
    {
        name = nameIn;
        patterns = pattIn;
        numPatterns = numPattIn;
        cycleSkip = cycleSkipIn;
        numCycleSkip = numCycleSkipIn;
        //------------------------------------------------------------
        // Default the pattern to the first pattern and set the index
        // to the first safe non-skipped value
        thisIndex = 0;
        _changeIndex(true);
    }

    /**
     * isOnCycle()
     * Determines if the component is cycling through patterns or not
     */
    boolean isOnCycle() { return thisIndex == 0; }

    /**
     * isNewPattern()
     * Returns if someone changed the pattern.  This will alert the drawing code
     * to clear / startup the new pattern
     */
    boolean isNewPattern()
    {
        if (!isNew)
        {
            return false;
        }
        isNew = false;
        return true;
    }
    /* getPattern()
     * Gets the processed pattern.  If it is cycle, we will return the pattern
     * at the current index.
     */
    String getPattern()
    {
        //---------------------------------------------------------
        // if we're currently on "cycle", return pattern at index,
        // otherwise, return the current pattern
        return (isOnCycle()) ? patterns[skipIndex] : patterns[thisIndex];
    }

    /* setPattern()
     * Used to manually set the pattern (either from BLE or Keypad)
     */
    void setPattern(String val)
    {
        thisIndex = _getIntendPatt(val);
        isNew = true;
    }

    /* cycleNext()
     * Used to cycle to the next non-skipped pattern
     */
    void cycleNext()
    {
        //---------------------------------------------------------
        // if we're not currently on "cycle", skip
        if (!isOnCycle())
            return;

        //---------------------------------------------------------
        // Go forward to the next safe index (using skip list)
        _changeIndex(true);
    }

    String toString()
    {
        return name + ": thisPattern[" + patterns[thisIndex] + "] getPattern()[" + getPattern() +
               "] index:maxVal:maxSkip[" + skipIndex + ":" + numPatterns + ":" + numCycleSkip + "]";
    }
};

#endif