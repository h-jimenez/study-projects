/*
  ==============================================================================

    NoteSectionComponent.h
    Created: 14 Oct 2024 3:22:27pm
    Author:  Legen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class NoteSectionComponent  : public juce::Component
{
public:
    NoteSectionComponent();
    ~NoteSectionComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteSectionComponent)
};
