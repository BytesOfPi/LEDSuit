
#ifndef CPattern_H
#define CPattern_H

#define SAFE_NEXT_PATTERN(val, max) (val + 1 > max ? 0 : val + 1)
#define SAFE_PREV_PATTERN(val, max) (val == 0 ? max : val - 1)

class CPattern
{
private:

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

    String _changeIndex(bool next)
    {
        // Go to the next safe pattern
        index = next ? SAFE_NEXT_PATTERN(index, numPatt) : SAFE_PREV_PATTERN(index, numPatt);
        // If we've indicated we want to skip patterns, keep incrementing
        // until we are not in the skip list
        while (_indexOf(patt[index], cycleSkip, numCycleSkip) > -1)
        {
            index = next ? SAFE_NEXT_PATTERN(index, numPatt) : SAFE_PREV_PATTERN(index, numPatt);
        }
        // return the pattern
        return patt[index];
    }

    String _getIntendPatt(String val)
    {
        // If "back" or "next" is sent, go to the next pattern
        if (val.equalsIgnoreCase("next"))
            return _changeIndex(true);
        if (val.equalsIgnoreCase("back"))
            return _changeIndex(false);

        //---------------------------------------------------------
        // Otherwise, check if value is in list
        int iHold = _indexOf(val, patt, numPatt);
        bool inSkipList = _indexOf(patt[iHold], cycleSkip, numCycleSkip) >= 0;
        // if the pattern found is NOT in skip list, set the class index

        if (!inSkipList)
        {
            index = iHold;
        }

        return patt[iHold];
    }

public:
    boolean isNew = false;
    int index = 0;         // determine which pattern
    String name;           // name of pattern
    String thisPattern;    // currently selected pattern
    String *cycleSkip;     // array of skipped patterns
    uint16_t numCycleSkip; // number of skipped patterns
    String *patt;          // array of string patterns
    uint16_t numPatt;      // number of string patterns

    CPattern(String nameIn, String pattIn[], uint16_t numPattIn, String cycleSkipIn[], uint16_t numCycleSkipIn)
    {
        name = nameIn;
        patt = pattIn;
        numPatt = numPattIn;
        cycleSkip = cycleSkipIn;
        numCycleSkip = numCycleSkipIn;
        //------------------------------------------------------------
        // Default the pattern to the first pattern and set the index
        // to the first safe non-skipped value
        thisPattern = pattIn[0];
        _changeIndex(true);
    }

    boolean isOnCycle() { return thisPattern.equals("cycle"); }

    boolean isNewPattern()
    {
        if(!isNew)
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
        return (isOnCycle()) ? patt[index] : thisPattern;
    }

    /* setPattern()
     * Used to manually set the pattern (either from BLE or Keypad)
     */
    void setPattern(String val)
    {
        thisPattern = _getIntendPatt(val);
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
};

#endif