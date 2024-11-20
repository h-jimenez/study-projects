/*
  ==============================================================================

    CircleComponent.cpp
    Created: 20 Oct 2024 4:45:36am
    Author:  Legen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CircleComponent.h"

//==============================================================================
CircleComponent::CircleComponent()
{

    
    
    

}

CircleComponent::~CircleComponent()
{
}

void CircleComponent::paint(juce::Graphics& g)
{

    int centerX = getLocalBounds().getCentreX();
    int centerY = getLocalBounds().getCentreY();
    int radius = getWidth() / 4;

    int sum = 0;

    //Path for drawing lines between center and notes
    juce::Path path;
    path.startNewSubPath(centerX, centerY); // Commence au centre du cercle

    //Draw big circle 
    g.setColour(juce::Colours::red);
    g.drawEllipse(centerX - radius, centerY - radius, radius * 2, radius * 2, 2.0f);


    const char* noteNames[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    g.setColour(juce::Colours::darkred);
    //Draw markers for common notes
    for (int i = 0; i < 12; ++i)
    {
        double angle = i * (juce::MathConstants<double>::twoPi / 12.0) - juce::MathConstants<double>::halfPi; // Angle ajusté

        //Begin markers pos
        double x1 = centerX + radius * std::cos(angle);
        double y1 = centerY + radius * std::sin(angle);
        //End markers pos
        double x2 = centerX + (radius + 10) * std::cos(angle); 
        double y2 = centerY + (radius + 10) * std::sin(angle);
        //Notes pos
        double x3 = centerX + (radius + 30) * std::cos(angle); 
        double y3 = centerY + (radius + 30) * std::sin(angle);

        g.drawLine(x1, y1, x2, y2, 2.0f); 
        
        juce::String noteName = noteNames[i];
        int textWidth = g.getCurrentFont().getStringWidth(noteName);
        int textHeight = g.getCurrentFont().getHeight();
        g.drawFittedText(noteName, x3 - textWidth / 2, y3 - textHeight / 2, textWidth, textHeight, juce::Justification::centred, 1);

    }




    g.setColour(juce::Colours::lightblue);
    
    //Draw Notes on circle from scale intervals

    for (size_t i = 0; i < userIntervals.size(); ++i)
    {
        g.setColour(juce::Colours::lightblue);
        sum += userIntervals[i];
        double angle = (sum / 1200.0) * juce::MathConstants<double>::twoPi - juce::MathConstants<double>::halfPi; // Ajustement de l'angle

        // Calcul de la position
        double x = centerX + radius * std::cos(angle);
        double y = centerY + radius * std::sin(angle);

        juce::String noteInScale;
        noteInScale = juce::String(i+1);

        float ellipseSize = 10.0f;
        g.fillEllipse(x - ellipseSize / 2, y - ellipseSize / 2, ellipseSize, ellipseSize); // Centrer l'ellipse
        g.setColour(juce::Colours::green);
        g.drawFittedText(noteInScale, x - ellipseSize, y - ellipseSize, ellipseSize * 4, ellipseSize * 4, juce::Justification::centred, 1);
        g.setColour(juce::Colours::lightblue);

        //Store position of note 
        noteCirclePos.push_back(std::make_pair(x - ellipseSize / 2, y - ellipseSize / 2));


        juce::Path centerToEllipse;
        centerToEllipse.startNewSubPath(centerX, centerY);
        centerToEllipse.lineTo(x, y);

        path.addPath(centerToEllipse); // Ajoute une ligne entre le centre et l'ellipse
    }
    DBG(userIntervals.size());
    g.setColour(juce::Colours::lightblue);
    g.strokePath(path, juce::PathStrokeType(1.0f)); // Trace les lignes
}

void CircleComponent::resized()
{


}

void CircleComponent::onScaleChange(std::vector<double> centsVector)
{

    userIntervals = centsVector;
    repaint();

}

void CircleComponent::mouseDown(const juce::MouseEvent& event)
{
    int clickX = event.getPosition().getX();
    int clickY = event.getPosition().getY();

    for (size_t i = 0; i < userIntervals.size(); i++)
    {
        double noteX = noteCirclePos[i].first;
        double noteY = noteCirclePos[i].second;


        if (std::hypot(clickX - noteX, clickY - noteY) <= clickRadius)
        {
            
            DBG("Click on note");
            


        }
    }



}