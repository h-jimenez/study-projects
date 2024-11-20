#pragma once

#include <JuceHeader.h>








class MidiProcessor : public juce::ChangeBroadcaster
{
public:
	juce::MidiBuffer processedBuffer;




	MidiProcessor();

	
	
	//PROCESS
	void process(juce::MidiBuffer& midiMessages); //Sends midi messages 
	void processMidiInput(const juce::MidiBuffer& midiMessages); //Do all the operation on midi messages


	//CALCULUS 
	void createMicrotonalMidiNote(juce::MidiMessage messageToModify, int samplePos, juce::MidiBuffer& processedBuffer);
	float getMicrotonalFrequency(int midiNote, float baseFrequency, const char* scale);
	int getDivisionsByScale(juce::String scale);
	float getMidiNoteFromFrequency(float frequency);
	float getCentsVariation(float newFrequency, float baseNoteOfScaleFrequency);



	
	//DISPLAY
	void updateDisplay(juce::String newNote, float newCents);
	juce::String getNoteValue() const { return noteToDisplay; }
	float getCentVariation() const { return centsToDisplay; }
	juce::String midiNoteToString(int midiNote);



	//DEBUG
	void displayNoteFrequencyWithPitchBend(juce::MidiMessage midiMessage, int pitchWheelPos, float pitchBendRangeInSemitones);

	
private:

 

	//Microtonal variable
	juce::String noteToDisplay;
	float centsToDisplay;
	int divisions = 12;


};