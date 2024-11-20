#pragma once
#include <JuceHeader.h>
#include "MidiProcessor.h"
#include "NoteComponent.h"
#include "CircleComponent.h"
#include "Scale.h"


class MpeProcessor : public juce::MPEInstrument, public juce::ChangeBroadcaster
{
public:
	MpeProcessor();
	~MpeProcessor() override;

	void process(juce::MidiBuffer& midiMessages);

	void processMPEInput(juce::MidiBuffer& midiMessages);

	//CALCULUS 
	int getDivisionsByScale(juce::String scale);
	void setDataScale(juce::String keyOfScale);
	float getMicrotonalFrequency(int midiNote, float baseFrequency);
	float getMidiNoteFromFrequency(float frequency);
	float getCentsVariation(float newFrequency, float baseNoteFrequency);

	//CORE FUNCTION
	std::vector<int> calculateMidiAndPitchbend(juce::MidiMessage& message);

	//SysEx FUNCTION

	juce::MidiMessage createSysExMessage(uint8_t note, int pitchbend, uint8_t channel);


	//DISPLAY FUNCTIONS
	//For the editor
	juce::String noteString;
	juce::String centsString;
	juce::String onOrOffString;
	int detectorOnOff = -1;


	juce::String midiNoteToString(int midiNote);
	void sendUpdateToEditor(std::pair<int, int> noteData , int detectorOnOff);

	
	//Ensure synchro between threads
	std::mutex mutex;
	std::queue<std::tuple<juce::String, juce::String, juce::String>> messageQueue;
	std::condition_variable cv;

	

	int channelToUse;
	int channelToFree;
	int samplePos;

	std::vector<double> centsVector;
	std::vector<double> frequenciesVector;
	
	Scale scaleCircle;




private:
	juce::MidiBuffer processedBuffer;
	juce::MPEZoneLayout layout;
	
	
	juce::MPEChannelAssigner channelAssigner;
	
	int divisions = 12;

	std::vector<std::tuple<std::string, double, int>> scaleData = {
	{"C4",     261.63, 60},
	{"C#4", 277.18, 61},
	{"D4",     293.66, 62},
	{"D#4", 311.13, 63},
	{"E4",     329.63, 64},
	{"F4",     349.23, 65},
	{"F#4", 369.99, 66},
	{"G4",     392.00, 67},
	{"G#4", 415.30, 68},
	{"A4",     440.00, 69},
	{"A#4", 466.16, 70},
	{"B4",     493.88, 71}
	};

	double baseNoteOfScaleFrequency;
	int baseNoteMidiNum;

};