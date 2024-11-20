/*
  ==============================================================================

    MainScene.cpp
    Created: 14 Oct 2024 2:20:21am
    Author:  Legen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainScene.h"

using namespace juce;

//==============================================================================
MainScene::MainScene()
{
    noteSection.setComponentID("Note Section");
    addAndMakeVisible(noteSection);
    scaleSection.setComponentID("Scale Section");
    addAndMakeVisible(scaleSection);
    scaleInfoSection.setComponentID("Scale Info Section");
    addAndMakeVisible(scaleInfoSection);
    circleComponent.setComponentID("Circle Component");
    addAndMakeVisible(circleComponent);
    circleComponent.setAlwaysOnTop(true);
}

MainScene::~MainScene()
{
}

void MainScene::paint (juce::Graphics& g)
{
    
    //Big rectangle
    g.setColour(juce::Colours::grey);
    Rectangle<int> sceneBounds = getLocalBounds().reduced(15);
    g.drawRect(sceneBounds, 2.0f);

    //background text
    g.setColour(juce::Colours::black);

    juce::Point Centre(getWidth() / 2, 0);

    Rectangle<int> positionRect = getLocalBounds().removeFromTop(50).removeFromLeft(200);
    positionRect.setCentre(Centre);

    g.fillRect(positionRect);

    //text
    g.setColour(juce::Colours::grey);
    g.setFont(32.0f);
    g.drawFittedText("Microtone", sceneBounds.removeFromTop(1), Justification::centred, 1);
    
}

void MainScene::resized()
{
    noteSection.setBoundsRelative(0.22f, 0.1f, 0.75f, 0.35f);
    scaleSection.setBoundsRelative(0.05f, 0.1f, 0.15f, 0.8f);
    scaleInfoSection.setBoundsRelative(0.22f, 0.5f, 0.2f, 0.4f);
    circleComponent.setBoundsRelative(0.4f, 0.42f, 0.55f, 0.55f);
}
