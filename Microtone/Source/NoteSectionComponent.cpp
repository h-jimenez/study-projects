/*
  ==============================================================================

    NoteSectionComponent.cpp
    Created: 14 Oct 2024 2:38:09pm
    Author:  Legen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NoteSectionComponent.h"

//==============================================================================
NoteSectionComponent::NoteSectionComponent()
{


}

NoteSectionComponent::~NoteSectionComponent()
{
}

void NoteSectionComponent::paint(juce::Graphics& g)
{
    //g.fillAll(juce::Colours::white);
    //rectangle
    g.setColour(juce::Colours::grey);
    juce::Rectangle<int> localBounds = getLocalBounds().reduced(15);
    g.drawRect(localBounds, 1.5f);

    //background Rectangle 

    //background text
    g.setColour(juce::Colours::black);

    juce::Point Centre(150, 0);

    juce::Rectangle<int> positionRect = localBounds.removeFromTop(75).removeFromLeft(90);
    positionRect.setCentre(Centre);

    g.fillRect(positionRect);


    //text 
    g.setColour(juce::Colours::grey);
    g.setFont(22.0f);
    g.drawFittedText("Notes", positionRect.removeFromBottom(50), juce::Justification::centred, 1);
}

void NoteSectionComponent::resized()
{

}
