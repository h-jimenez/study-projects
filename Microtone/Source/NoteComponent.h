/*
  ==============================================================================

    NoteComponent.h
    Created: 17 Oct 2024 3:25:16pm
    Author:  Legen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class NoteComponent  : public juce::Component
{
public:
    NoteComponent(juce::String& note, juce::String& cents);
    ~NoteComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;


    juce::Label noteLabel;
    juce::Label centsLabel;

private:

   

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteComponent)
};
