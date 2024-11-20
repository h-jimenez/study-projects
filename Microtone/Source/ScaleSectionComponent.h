/*
  ==============================================================================

    ScaleSectionComponent.h
    Created: 14 Oct 2024 3:23:21pm
    Author:  Legen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ScaleSectionComponent  : public juce::Component
{
public:
    ScaleSectionComponent();
    ~ScaleSectionComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScaleSectionComponent)
};
