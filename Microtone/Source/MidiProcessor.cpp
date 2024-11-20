#include <JuceHeader.h>
#include "MidiProcessor.h"


constexpr int interval = 4;





MidiProcessor::MidiProcessor()
{

	
}

void MidiProcessor::process(juce::MidiBuffer& midiMessages)
{
	processedBuffer.clear();
	
	processMidiInput(midiMessages);
	

	midiMessages.swapWith(processedBuffer);

}




void MidiProcessor::processMidiInput(const juce::MidiBuffer& midiMessages)
{
	
	juce::MidiBuffer::Iterator it(midiMessages);
	juce::MidiMessage currentMessage;
	int samplePos;

	while (it.getNextEvent(currentMessage, samplePos))
	{
		

		
		if (currentMessage.isNoteOnOrOff())
		{
			createMicrotonalMidiNote(currentMessage, samplePos, processedBuffer);
			
			
		}

	}
}


void MidiProcessor::createMicrotonalMidiNote(juce::MidiMessage messageToModify, int samplePos, juce::MidiBuffer& processedBuffer)
{
	

	


	//Scale
	//const char* scale = "24-TET Scale";
	const char* scale = "19-TET Scale";

	//Root info of scale
	const char* baseNoteOfScale = "A4";
	float baseNoteOfScaleFrequency = 440.0f;
	int baseNoteMidiNum = 69;

	//Played Note Info
	auto NoteNum = messageToModify.getNoteNumber();
	auto NoteFrequency = messageToModify.getMidiNoteInHertz(NoteNum, (double)baseNoteOfScaleFrequency);




	//Calculus
	int deltaMidi = NoteNum - baseNoteMidiNum;
	float newNoteFrequency = getMicrotonalFrequency(deltaMidi, baseNoteOfScaleFrequency, scale);

	float newNoteMidiNum = getMidiNoteFromFrequency(newNoteFrequency);

	int newMidiNoteRounded = round(newNoteMidiNum);


	//Determine String of MidiNote
	juce::String noteString = midiNoteToString(newMidiNoteRounded);


	float centsVariation = getCentsVariation(newNoteFrequency, messageToModify.getMidiNoteInHertz(newMidiNoteRounded));

	//Update Display Labels
	updateDisplay(noteString, centsVariation);

	float semitoneVariation = centsVariation / 100;

	auto pitchWheelPos = juce::MidiMessage::pitchbendToPitchwheelPos(semitoneVariation, 2.0f);

	//Update Midi Note Number 
	messageToModify.setNoteNumber(newMidiNoteRounded);
	//Create Midi Message for pitchbend
	juce::MidiMessage pitchBendMessage = juce::MidiMessage::pitchWheel(messageToModify.getChannel(), pitchWheelPos);
	

	//Add To Buffer
	processedBuffer.addEvent(messageToModify, samplePos);
	processedBuffer.addEvent(pitchBendMessage, samplePos);
	

	//-------------------------DEBUG LOG---------------------------------//
	/*
	DBG("New Note MIDI Number : " << " " << newNoteMidiNum << " " << "New Note Frequency : " << " " << newNoteFrequency << " " << "Cents Variation : " << centsVariation << " " << "Semitones Variation : " << semitoneVariation << " " << "PitchBendValue : " << (int)pitchWheelPos);
	DBG("     ");
	*/

	/*
	DBG("DESCRIPTION OF MESSAGE SENT");
	DBG(messageToModify.getDescription());
	DBG(pitchBendMessage.getDescription());
	*/
	if (messageToModify.isNoteOn())
	{
		//displayNoteFrequencyWithPitchBend(messageToModify, pitchWheelPos, 2.0f); //Print in console frequency of output midi note with pitchbend
		


	}
	if (messageToModify.isNoteOff())
	{



	}


}


float MidiProcessor::getMicrotonalFrequency(int midiNote, float baseFrequency, const char* scale)
{
	
	//divisions = getDivisionsByScale(scale);												//get number of divisions depending on chose scale
	float newFrequency = baseFrequency * std::pow(2.0f, static_cast<float>(midiNote) / divisions);
	return newFrequency;
}

int MidiProcessor::getDivisionsByScale(juce::String scale)
{
	if (scale == (juce::String)"19-TET Scale")
	{
		divisions = 19;
		return divisions;
	}
	if (scale == (juce::String)"24-TET Scale")
	{
		divisions = 24;
		return divisions;
	}
	if (scale == (juce::String)"31-TET Scale")
	{
		divisions = 31;
		return divisions;
	}
	return divisions;
}

float MidiProcessor::getMidiNoteFromFrequency(float newFrequency)
{
	const float A4_frequency = 440.0f;
	float fraction = 12.0f * log2(newFrequency / A4_frequency);
	return 69.0f + fraction;
}



float MidiProcessor::getCentsVariation(float newFrequency, float baseNoteOfScaleFrequency)
{
	float centsVariation = 1200 * log2(newFrequency / baseNoteOfScaleFrequency);

	return centsVariation;
}

//--------------------------DISPLAY FUNCTIONS---------------------------------------


void MidiProcessor::updateDisplay(juce::String newNote, float newCents)
{
	if (noteToDisplay != newNote)
	{
		noteToDisplay = newNote;
	}
	if (centsToDisplay != newCents)
	{
		centsToDisplay = newCents;
	}
	sendChangeMessage(); // Notify editor
}







juce::String MidiProcessor::midiNoteToString(int midiNote)
{
	const char* noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	int noteIndex = midiNote % 12; 
	int octave = (midiNote / 12) - 1; 
	noteToDisplay = juce::String(noteNames[noteIndex]) + juce::String(octave);
	return  noteToDisplay;
}




//--------------------------DEBUG FUNCTIONS---------------------------------------

static float getPitchbendInSemitones(int pitchWheelPos, float pitchbendRangeInSemitones)
{
	int pitchBendAmount = pitchWheelPos - 8192;
	return (pitchBendAmount / 8192.0f) * pitchbendRangeInSemitones;
}

static float getFrequencyWithPitchbend(float baseFrequency, float pitchbendInSemitones)
{
	return baseFrequency * std::pow(2.0f, pitchbendInSemitones / 12.0f);
}


void MidiProcessor::displayNoteFrequencyWithPitchBend(juce::MidiMessage midiMessage, int pitchWheelPos, float pitchBendRangeInSemitones)
{
	float baseFrequency = midiMessage.getMidiNoteInHertz(midiMessage.getNoteNumber());
	float pitchBendInSemitones = getPitchbendInSemitones(pitchWheelPos, pitchBendRangeInSemitones);
	float totalFrequency = getFrequencyWithPitchbend(baseFrequency, pitchBendInSemitones);
	DBG("Fréquence Totale de la Note jouée : " << totalFrequency);

}




