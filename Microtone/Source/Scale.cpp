/*
  ==============================================================================

    Scale.cpp
    Created: 20 Oct 2024 4:48:32am
    Author:  Legen

  ==============================================================================
*/

#include "Scale.h"





Scale::Scale() 
{
   


}

void Scale::setNoteIntervals(const std::vector<int>& intervalsInCents)
{
    noteIntervals = intervalsInCents;
    calculateFrequencies();

}

void Scale::calculateFrequencies()
{
    noteFrequencies.clear();
    noteFrequencies.push_back(baseFrequency);

    for (int centsInterval : noteIntervals)
    {
        double previousFrequency = noteFrequencies.back();
        double newFrequency = previousFrequency * std::pow(2.0, centsInterval / 1200.0);
        noteFrequencies.push_back(newFrequency);
    }

}

void Scale::setBaseFrequency(double frequency)
{
    baseFrequency = frequency;
}

std::vector<double> Scale::getFrequencies()
{
    return noteFrequencies;
}

std::vector<double> Scale::setEDOCentsVector(int divisions)
{
    double centsPerNotes = 1200.0/divisions;
    std::vector<double> centsVector;
    for (int i = 0; i < divisions+1; i++)
    {
        if (i == 0)
        {
            centsVector.push_back(0.0);
        }
        else
        {
            centsVector.push_back(centsPerNotes);
        }
        
        
    }



    return centsVector;
}

