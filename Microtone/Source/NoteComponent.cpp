/*
  ==============================================================================

    NoteComponent.cpp
    Created: 17 Oct 2024 3:25:16pm
    Author:  Legen

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NoteComponent.h"

//==============================================================================
NoteComponent::NoteComponent(juce::String& note, juce::String& cents)
{
    noteLabel.setFont(24.0f);
    centsLabel.setFont(24.0f);
    noteLabel.setText(note, juce::dontSendNotification);
    centsLabel.setText(cents, juce::dontSendNotification);
    addAndMakeVisible(noteLabel);
    addAndMakeVisible(centsLabel);
}

NoteComponent::~NoteComponent()
{
}

void NoteComponent::paint (juce::Graphics& g)
{
    
    
}

void NoteComponent::resized()
{
    
    auto area = getLocalBounds(); // Récupère l'espace total du composant

    // Taille des labels
    int labelWidth = 80; // Par exemple, 80 pixels de large pour chaque label
    int labelHeight = 20; // 20 pixels de hauteur pour chaque label

    // Centrer le noteLabel
    noteLabel.setBounds((getWidth() / 2) - (labelWidth / 2),
        (getHeight() / 2) - (labelHeight / 2) - 10,
        labelWidth, labelHeight);

    // Centrer le centsLabel juste en dessous du noteLabel
    centsLabel.setBounds((getWidth() / 2) - (labelWidth / 2) ,
        (getHeight() / 2) - (labelHeight / 2) +20 ,
        labelWidth, labelHeight);

    // Assure-toi que le texte dans les labels est bien centré
    noteLabel.setJustificationType(juce::Justification::centred);
    centsLabel.setJustificationType(juce::Justification::centred);

}
