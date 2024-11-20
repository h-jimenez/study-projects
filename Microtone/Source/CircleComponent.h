/*
  ==============================================================================

    CircleComponent.h
    Created: 20 Oct 2024 4:45:36am
    Author:  Legen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Scale.h"

//==============================================================================
/*
*/
class CircleComponent  : public juce::Component
{
public:
    CircleComponent();
    ~CircleComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void onScaleChange(std::vector<double> centsVector);

    void mouseDown(const juce::MouseEvent& event);

    std::vector<double> userIntervals;

    std::vector<double> frequenciesVector;

    std::vector<std::pair<double,double>> noteCirclePos;

    std::vector<std::pair<double, double>> selectedNotes;


private:

    double clickRadius = 10.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircleComponent)
};
