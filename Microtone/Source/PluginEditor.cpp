/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


using namespace juce;

//==============================================================================
MicrotoneAudioProcessorEditor::MicrotoneAudioProcessorEditor(MicrotoneAudioProcessor& p, MpeProcessor& mpeProcessor)
    : AudioProcessorEditor (&p), audioProcessor (p), mpeProcessor(mpeProcessor)
{
    mpeProcessor.addChangeListener(this);

    addAndMakeVisible(mainSceneComponent);

    // Temporary ComboBox for scale selection
    scaleBox.addItem("19-TET Scale", 1);
    scaleBox.addItem("24-TET Scale", 2);
    scaleBox.addItem("31-TET Scale", 3);
    scaleBox.addListener(this);
    scaleBox.setSelectedId(1);
    scaleBox.setAlwaysOnTop(true);
    addAndMakeVisible(scaleBox);
    
    
    keyBox.addItem("C4",1);
    keyBox.addItem("C#4",2);
    keyBox.addItem("D4",3);
    keyBox.addItem("D#4",4);
    keyBox.addItem("E4",5);
    keyBox.addItem("F4",6);
    keyBox.addItem("F#4",7);
    keyBox.addItem("G4",8);
    keyBox.addItem("G#4",9);
    keyBox.addItem("A4",10);
    keyBox.addItem("A#4",11);
    keyBox.addItem("B4",12);
    keyBox.addListener(this);
    keyBox.setSelectedId(10);
    keyBox.setAlwaysOnTop(true);
    addAndMakeVisible(keyBox);
    
    


//----------------------------------------------------------------------------
    setSize (1200,800);

    middle_x = getWidth() / 2;
    middle_y = getHeight() / 2;
    

}





MicrotoneAudioProcessorEditor::~MicrotoneAudioProcessorEditor()
{
    mpeProcessor.removeChangeListener(this);
}

//==============================================================================
void MicrotoneAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
    



}

void MicrotoneAudioProcessorEditor::resized()
{
    scaleBox.setBoundsRelative(0.075f,0.15f,0.10f,0.04f);
    keyBox.setBoundsRelative(0.27f, 0.55f, 0.1f, 0.04f);
    


     // Récupère la zone de l'éditeur
    auto noteSection = mainSceneComponent.findChildWithID("Note Section");
    auto scaleSection = mainSceneComponent.findChildWithID("Scale Section");
    if (noteSection == nullptr)
    {
        DBG("Note Section not found");
    }


    int numNotes = notesToDisplay.size();
    if (numNotes > 0)
    {
        auto xNoteSection = mainSceneComponent.noteSection.getX();
        auto yNoteSection = mainSceneComponent.noteSection.getY();
        auto widthNoteSection = mainSceneComponent.noteSection.getWidth();
        auto heightNoteSection = mainSceneComponent.noteSection.getHeight();

        auto area = juce::Rectangle<int>(xNoteSection, yNoteSection, widthNoteSection, heightNoteSection);
        area = area.reduced(30);
        
        int componentWidth = area.getWidth() / notesToDisplay.size(); // Hauteur de chaque composant

        int width = getWidth() / numNotes; // Calcule la hauteur de chaque composant

        int index = 0;
        for (const auto& notePair : notesToDisplay)
        {
            auto* noteComponent = notePair.second.get();

            // Définit la taille et la position de chaque composant
            
            noteComponent->setBounds(area.removeFromRight(componentWidth)); // Positionne le composant
            
            index++;
        }
    }



    mainSceneComponent.setBounds(0,0, getWidth(), getHeight());
}

void MicrotoneAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &scaleBox)
    {
        juce::String selectedScale = scaleBox.getItemText(scaleBox.getSelectedId()-1);
        mpeProcessor.getDivisionsByScale(selectedScale);
        mainSceneComponent.circleComponent.onScaleChange(mpeProcessor.centsVector);
    }
    if (comboBox == &keyBox)
    {
        juce::String keyOfScale = keyBox.getItemText(keyBox.getSelectedId() - 1);
        mpeProcessor.setDataScale(keyOfScale);
        mainSceneComponent.circleComponent.onScaleChange(mpeProcessor.centsVector);
    }
}








void MicrotoneAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &mpeProcessor)
    {
        std::lock_guard<std::mutex> lock(mpeProcessor.mutex);
        
        //Lock while queue is not empty 
        while (!mpeProcessor.messageQueue.empty()) {
            auto message = mpeProcessor.messageQueue.front();
            mpeProcessor.messageQueue.pop();

            noteString = std::get<0>(message);
            centsString = std::get<1>(message);
            onOrOffString = std::get<2>(message);

            createNoteComponent(noteString, centsString, onOrOffString);
        }

    }
}

void MicrotoneAudioProcessorEditor::createNoteComponent(juce::String noteString, juce::String centsString, juce::String onOrOffString)
{
    std::pair<juce::String, juce::String> noteData = std::make_pair(noteString, centsString);
    
    if (onOrOffString == "Note On")
    {
        if (notesToDisplay.find(noteData) == notesToDisplay.end())
        {
            // Créer le composant et l'ajouter à la map
            auto noteComponent = std::make_unique<NoteComponent>(noteString, centsString);
            
            notesToDisplay[noteData] = std::move(noteComponent);
            addAndMakeVisible(notesToDisplay[noteData].get());

            //DBG("Added Note: " << noteData.first << ", " << noteData.second);
        }
        else
        {
            //DBG("Note already exists: " << noteData.first << ", " << noteData.second);
        }
    }
    else if (onOrOffString == "Note Off")
    {
       //DBG("Trying to remove: " << noteData.first << ", " << noteData.second);
       auto& noteComponent = notesToDisplay[noteData];
       removeChildComponent(noteComponent.get());
       notesToDisplay.erase(noteData);
       //DBG("Removed Note: " << noteData.first << ", " << noteData.second);
       
       
        
    }
    else {
        DBG("OULA GROS PROBLEME");
    }
    resized(); // Appelle resized() une seule fois après les ajouts/suppressions
}













// Affichage microtonal avec les symboles adéquats
