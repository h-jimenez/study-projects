/*
  ==============================================================================

    ScaleInfoComponent.h
    Created: 17 Oct 2024 11:59:52pm
    Author:  Legen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ScaleInfoComponent  : public juce::Component
{
public:
    ScaleInfoComponent();
    ~ScaleInfoComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScaleInfoComponent)
};
