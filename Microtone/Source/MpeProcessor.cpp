#include "MpeProcessor.h"

MpeProcessor::MpeProcessor()
{
	layout.setLowerZone(2, 15);
	setZoneLayout(layout);
	enableLegacyMode(15);
    channelToUse = 1;
    channelToFree = 0;
    samplePos = 0;
    
    

}

MpeProcessor::~MpeProcessor()
{
}

void MpeProcessor::process(juce::MidiBuffer& midiMessages)
{
	processedBuffer.clear();
	processMPEInput(midiMessages);
	midiMessages.swapWith(processedBuffer);
}



void MpeProcessor::processMPEInput(juce::MidiBuffer& midiMessages)
{
	juce::MidiBuffer::Iterator it(midiMessages);
	juce::MidiMessage currentMessage;
	

	while (it.getNextEvent(currentMessage, samplePos))
	{
        if (currentMessage.isNoteOn())
        {
            //midiAndPitchbend[0] = Microtonal MIDI Note Number 
            //midiAndPitchbend[1] = PitchBend Value 
            //midiAndPitchbend[2] = Cents Value
            auto midiAndPitchbend = calculateMidiAndPitchbend(currentMessage);

            //Channel Operations
            channelToUse = channelAssigner.findMidiChannelForNewNote(midiAndPitchbend[0]);

            int channel = channelToUse;
            

            //Retrieve Original Velocity
            juce::MPEValue noteVelocity;
            noteVelocity.from7BitInt(currentMessage.getVelocity());

            //PitchBend Operations
            juce::MPEValue pitchbendValue;
            pitchbendValue.from14BitInt(midiAndPitchbend[1]);
            
            //Midi Messages To Send To Buffer
            juce::MidiMessage noteOnMessage = juce::MidiMessage::noteOn(channelToUse, midiAndPitchbend[0], currentMessage.getVelocity());
            juce::MidiMessage pitchWheelMessage = juce::MidiMessage::pitchWheel(channelToUse, midiAndPitchbend[1]);

            //DBG("PitchWheel channel : " << " " << pitchWheelMessage.getChannel());

            //Initialize key
            std::pair<int, float> noteData = std::make_pair(midiAndPitchbend[0], midiAndPitchbend[2]);

            detectorOnOff = 0;
            sendUpdateToEditor(noteData,detectorOnOff);
            sendChangeMessage();
            
            /*
            juce::MidiMessage sysExMessage = createSysExMessage((uint8_t)midiAndPitchbend[0], midiAndPitchbend[1], (uint8_t)channelToUse);

            auto messageData = sysExMessage.getSysExData();
            
            DBG("SysExMessage : " << " " << messageData[0] << " " << messageData[1] << " " << messageData[2] << " " << messageData[3] << " " << messageData[4] << " " << messageData[5] << " " << messageData[6] << " " << messageData[7]);
            for (int i = 0; i < sysExMessage.getSysExDataSize(); i++)
            {
                DBG(juce::String::toHexString(messageData[i]));
            }

            if (sysExMessage.isSysEx())
            {
                DBG("is Sysex");
            }

            juce::uint8 sysexdata[10] = { 0x47, 0x00, 0x7B, 0x60, 0x00, 0x04, 0x41, 0x08, 0x04, 0x01 };
            juce::MidiMessage mensaje = juce::MidiMessage::createSysExMessage(sysexdata, 10);


            processedBuffer.addEvent(mensaje, samplePos);
            processedBuffer.addEvent(noteOnMessage, samplePos);
            */
            
            if (pitchWheelMessage.getChannel() == noteOnMessage.getChannel())
            {
                DBG(noteOnMessage.getChannel());
            }
            

            //Add Into Buffer
            processedBuffer.addEvent(noteOnMessage, samplePos);
            processedBuffer.addEvent(pitchWheelMessage, samplePos);



            


            //ADD TO THE MPE INSTRUMENT ?
            juce::MPEInstrument::noteOn(channelToUse,midiAndPitchbend[0],noteVelocity);
            juce::MPEInstrument::pitchbend(channelToUse, pitchbendValue);
            //--------------DEBUG-----------------
            // Vérifie la note la plus récente
            auto note = juce::MPEInstrument::getMostRecentNote(channelToUse);
            if (note.isValid())
            {
                
            }
            // Affiche le nombre de notes jouées
            DBG("Number of Notes : " << " " << juce::MPEInstrument::getNumPlayingNotes());
            
        }
        else if(currentMessage.isNoteOff())
        {
            //Same as midiAndPitchBend
            auto midiAndPitchbendToErase = calculateMidiAndPitchbend(currentMessage);
            //Retrieve Original Velocity
            juce::MPEValue noteVelocity;
            noteVelocity.from7BitInt(currentMessage.getVelocity());

            //Channel Operations
            channelToUse = channelAssigner.findMidiChannelForExistingNote(midiAndPitchbendToErase[0]);
            if (channelToUse == -1)
            {
                DBG("Channel of this note Not found");
            }
            channelAssigner.noteOff(midiAndPitchbendToErase[0], channelToUse);

            //Midi Messages To Send To Buffer
            juce::MidiMessage noteOffMessage = juce::MidiMessage::noteOff(channelToUse, midiAndPitchbendToErase[0]);

            //Add Into Buffer
            processedBuffer.addEvent(noteOffMessage, samplePos);

            //Initialize key
            std::pair<int, float> noteData = std::make_pair(midiAndPitchbendToErase[0], midiAndPitchbendToErase[2]);

            //Remove note from map
            detectorOnOff = 1;
            sendUpdateToEditor(noteData, detectorOnOff);
            sendChangeMessage();
            
            



            //ADD TO THE MPE INSTRUMENT ?
            juce::MPEInstrument::noteOff(channelToUse, midiAndPitchbendToErase[0], noteVelocity);
            //--------------DEBUG-----------------
            //Track Number of notes
            //DBG("Number of Notes : " << " " << juce::MPEInstrument::getNumPlayingNotes());
        }
        
        
	}


}

//------------------------------------------------------------------------------------------------
//----------------------------------------CALCULUS------------------------------------------------
//------------------------------------------------------------------------------------------------

//Get divisions number by comboBox
int MpeProcessor::getDivisionsByScale(juce::String scale)
{
    if (scale == (juce::String)"19-TET Scale")
    {
        divisions = 19;
        
    }
    if (scale == (juce::String)"24-TET Scale")
    {
        divisions = 24;
        
    }
    if (scale == (juce::String)"31-TET Scale")
    {
        divisions = 31;
        
    }
    
    centsVector = scaleCircle.setEDOCentsVector(divisions);



    return divisions;
}

void MpeProcessor::setDataScale(juce::String keyOfScale)
{
    for (std::tuple trio : scaleData)
    {
        if (std::get<0>(trio) == keyOfScale)
        {
            baseNoteOfScaleFrequency = std::get<1>(trio);
            baseNoteMidiNum = std::get<2>(trio);
            
            scaleCircle.setBaseFrequency(baseNoteOfScaleFrequency);
            scaleCircle.calculateFrequencies();
            frequenciesVector = scaleCircle.getFrequencies();
            
            
            

        }

    }

}



//------------------------------------------------------------------------------------------------
//Frequency from the number of EDO we want 
float MpeProcessor::getMicrotonalFrequency(int midiNote, float baseFrequency)
{
    												
    float newFrequency = baseFrequency * std::pow(2.0f, static_cast<float>(midiNote) / divisions);
    return newFrequency;
}
//From the new frequency we determine new Midi note
float MpeProcessor::getMidiNoteFromFrequency(float frequency)
{
    const float A4_frequency = 440.0f;
    float fraction = 12.0f * log2(frequency / A4_frequency);
    return 69.0f + fraction;
}
//And get the cents variation by the fraction : microtonalNote_frequency/originalNote_Frequency
float MpeProcessor::getCentsVariation(float newFrequency, float baseNoteFrequency)
{
    float centsVariation = 1200 * log2(newFrequency / baseNoteFrequency);

    return centsVariation;
}
//------------------------------------------------------------------------------------------------


//Everything happens here, we calculate new midi number, pitchbend value and cents variation then store
//them inside a 3-component vector 
std::vector<int> MpeProcessor::calculateMidiAndPitchbend(juce::MidiMessage& message)
{
    std::vector<int> result(3);


    juce::MidiMessage messageToModify = message;
    //Scale
    //const char* scale = "24-TET Scale";
    //juce::String scale = "19-TET Scale";
    
    
    

    //Root info of scale
    /*
    juce::String baseNoteOfScale = "A4";
    //A intégrer méthode qui détermine la base note selon baseNoteOfScale d'une comboBox
    
    float baseNoteOfScaleFrequency = 440.0f;
    int baseNoteMidiNum = 69;
    */
    
    //Played Note Info
    auto NoteNum = messageToModify.getNoteNumber();
    auto NoteFrequency = messageToModify.getMidiNoteInHertz(NoteNum, (double)baseNoteOfScaleFrequency);


    //Calculus
    int deltaMidi = NoteNum - baseNoteMidiNum;
    float newNoteFrequency = getMicrotonalFrequency(deltaMidi, baseNoteOfScaleFrequency);
    float newNoteMidiNum = getMidiNoteFromFrequency(newNoteFrequency);
    int newMidiNoteRounded = round(newNoteMidiNum);

    
    float centsVariationF = getCentsVariation(newNoteFrequency, messageToModify.getMidiNoteInHertz(newMidiNoteRounded));
    //DBG(centsVariationF);
    int centsVariation = std::round(centsVariationF);
    float semitoneVariation = centsVariationF / 100;

    /*
    //Update Display
    juce::String noteString = midiNoteToString(newMidiNoteRounded);
    updateDisplay(noteString, centsVariation);
    */

    auto pitchBendValue = juce::MidiMessage::pitchbendToPitchwheelPos(semitoneVariation, 2.0f);

    result[0] = newMidiNoteRounded; // MIDI Number
    result[1] = pitchBendValue; // Pitchbend Value
    result[2] = centsVariation; // Cents variation value


    return result;
}

juce::MidiMessage MpeProcessor::createSysExMessage(uint8_t note, int pitchbend, uint8_t channel)
{
    int midiPitchBend = pitchbend;
    uint8_t pitchLSB = midiPitchBend & 0x7f;
    uint8_t pitchMSB = (midiPitchBend >> 7) & 0x7F;

    std::vector<uint8_t> sysExData;
    sysExData.push_back(0x01);  //ID
    sysExData.push_back(0x10);  //Ope code

    sysExData.push_back(note);
    sysExData.push_back(pitchLSB);
    sysExData.push_back(pitchMSB);
    sysExData.push_back(channel);
    

    return juce::MidiMessage::createSysExMessage(sysExData.data(), sysExData.size());
}

//------------------------------------------------------------------------------------------------
//-----------------------------------------DISPLAY------------------------------------------------
//------------------------------------------------------------------------------------------------



juce::String MpeProcessor::midiNoteToString(int midiNote)
{
    const char* noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    int noteIndex = midiNote % 12;
    int octave = (midiNote / 12) - 1;
    juce::String midiNoteString = juce::String(noteNames[noteIndex]) + juce::String(octave);
    return  midiNoteString;
}

void MpeProcessor::sendUpdateToEditor(std::pair<int, int> noteData, int detectorOnOff)
{

    std::lock_guard<std::mutex> lock(mutex);

    noteString = midiNoteToString(noteData.first);
    centsString = juce::String(noteData.second) + " cents";
    if (detectorOnOff == 0)
    {
        onOrOffString = "Note On";
    }
    else if (detectorOnOff == 1)
    {
        onOrOffString = "Note Off";
    }
    else 
    {
        onOrOffString = "Unknown";
    }
    messageQueue.emplace(std::make_tuple(noteString, centsString, onOrOffString));

    sendChangeMessage();
    

    
    
    

}


