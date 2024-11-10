#pragma once

#include "ofMain.h"

constexpr auto SAMPLE_RATE = 48000;
constexpr auto BUFFER_SIZE = 256;
constexpr auto OUTPUT_CHANNELS = 2;

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void mouseMoved(int x, int y);
	void draw();
	void audioOut(ofSoundBuffer& buffer);

	ofSoundStream m_soundStream;
	
	std::atomic<float> modulationAtomic;
	std::atomic<float> delayAtomic;

	vector <float> buffer;
};
