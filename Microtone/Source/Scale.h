/*
  ==============================================================================

    Scale.h
    Created: 20 Oct 2024 4:48:32am
    Author:  Legen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>



class Scale
{
public:
    Scale();

    void setNoteIntervals(const std::vector<int>& intervalsInCents);
    void calculateFrequencies();
    void setBaseFrequency(double frequency);
    std::vector<double> getFrequencies();

    //Function for EDO scales
    std::vector<double> setEDOCentsVector(int divisions);
    

    

private:

    double baseFrequency;
    std::vector<int> noteIntervals;
    std::vector<double> noteFrequencies;


};
