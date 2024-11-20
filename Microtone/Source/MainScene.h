/*
  ==============================================================================

    MainScene.h
    Created: 14 Oct 2024 2:20:21am
    Author:  Legen

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "NoteSectionComponent.h"
#include "ScaleSectionComponent.h"
#include "ScaleInfoComponent.h"
#include "CircleComponent.h"

//==============================================================================
/*
*/
class MainScene  : public juce::Component
{
public:
    MainScene();
    ~MainScene() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    NoteSectionComponent noteSection;
    ScaleSectionComponent scaleSection;
    ScaleInfoComponent scaleInfoSection;
    CircleComponent circleComponent;

private:

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainScene)
};
