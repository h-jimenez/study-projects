/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MainScene.h"




//==============================================================================
/**
*/
class MicrotoneAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::ChangeListener, public juce::ComboBox::Listener
{
public:
    MicrotoneAudioProcessorEditor(MicrotoneAudioProcessor&, MpeProcessor&);
    ~MicrotoneAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void comboBoxChanged(juce::ComboBox* comboBox);

    
    
    

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void createNoteComponent(juce::String noteString, juce::String centsString,juce::String onOrOffString);




    int middle_x;
    int middle_y;

private:
  

    juce::ComboBox scaleBox;       

    juce::ComboBox keyBox;

    MainScene mainSceneComponent;   //MainScene

    //Map of played notes
    //key = midiNote,CentsVariation and refer to a noteComponent
    std::map < std::pair <juce::String, juce::String> , std::unique_ptr<NoteComponent >> notesToDisplay;

    juce::String noteString;
    juce::String centsString;
    juce::String onOrOffString;
    

    MpeProcessor& mpeProcessor;
    MicrotoneAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MicrotoneAudioProcessorEditor)
};
