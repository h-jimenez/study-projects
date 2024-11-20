/*
  ==============================================================================

    ScaleInfoComponent.cpp
    Created: 17 Oct 2024 11:59:53pm
    Author:  Legen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ScaleInfoComponent.h"

//==============================================================================
ScaleInfoComponent::ScaleInfoComponent()
{
    

}

ScaleInfoComponent::~ScaleInfoComponent()
{
}

void ScaleInfoComponent::paint (juce::Graphics& g)
{
    //g.fillAll(juce::Colours::white);
    //rectangle
    g.setColour(juce::Colours::grey);
    juce::Rectangle<int> localBounds = getLocalBounds().reduced(15);
    g.drawRect(localBounds, 1.5f);

    //background Rectangle 

    //background text
    g.setColour(juce::Colours::black);

    juce::Point Centre(getWidth() / 2, 0);

    juce::Rectangle<int> positionRect = localBounds.removeFromTop(75).removeFromLeft(120);
    positionRect.setCentre(Centre);

    g.fillRect(positionRect);


    //text 
    g.setColour(juce::Colours::grey);
    g.setFont(22.0f);
    g.drawFittedText("Scale Info", positionRect.removeFromBottom(50), juce::Justification::centred, 1);
}

void ScaleInfoComponent::resized()
{
   

}
