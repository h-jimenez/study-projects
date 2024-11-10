#include "ofApp.h"
#include <vector>
#include "RingBuffer.h"


RingBuffer* m_pRingBuffer;

float m_maxDelayS = 1.0f;


float currentmodulation = 0.0f;
float maxmodulation = 0.0f;
float minmodulation = 0.0f;

float currentdelay = 0.0f;
float maxdelay = 0.0f;
float mindelay = 1.0f;

int globalvalue = 0;
float color_r = 0.0f;
float color_g = 0.0f;
float color_b = 0.0f;
float color_x = 0.0f;
//--------------------------------------------------------------
void ofApp::setup(){

	vector<ofSoundDevice> devices = m_soundStream.getDeviceList();

	for (int i = 0; i < devices.size(); i++) {
		cout << "device[" << i << "] : " << devices[i].name << "\n";
	}

	int DeviceIndex = -1;

	
	while (DeviceIndex < 0 || DeviceIndex > devices.size()) {
		cout << "Select an audio device \n";
		cin >> DeviceIndex;
	}

	cout << "You've selected " << devices[DeviceIndex].name << "\n";
	

	ofSoundStreamSettings settings;
	settings.bufferSize = BUFFER_SIZE;
	settings.numOutputChannels = OUTPUT_CHANNELS;
	settings.sampleRate = SAMPLE_RATE;
	settings.setOutDevice(devices[DeviceIndex]);
	settings.setOutListener(this);

	m_soundStream.setup(settings);

	m_pRingBuffer = new RingBuffer((int)(m_maxDelayS * SAMPLE_RATE) + 1);

	buffer.assign(BUFFER_SIZE, 0.0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	//------Display useful variables------

	
	currentmodulation = modulationAtomic.load();

	if (currentmodulation > maxmodulation) {
		maxmodulation = currentmodulation;
	}

	if (currentmodulation < minmodulation) {
		minmodulation = currentmodulation;
	}

	currentdelay = delayAtomic.load();

	if (currentdelay > maxdelay) {
		maxdelay = currentdelay;
	}

	if (currentdelay < mindelay) {
		mindelay = currentdelay;
	}


	//------Amazing background-------


	ofNoFill();

	ofSetBackgroundColor((int)color_r, (int)color_g, (int)color_b);
	
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(362, 314, 0);
			ofFill();
			ofSetColor((int)color_r, (int)color_g, (int)color_b);
			ofDrawCircle(color_r * 2.8 + 20, color_g * 1.6 - 50, color_b / 2);

			ofDrawCircle(color_g * 3 + 200, color_r * 2 - 50, color_r / 2);

			ofDrawCircle(color_b * 1.3 + 100, color_b * 2.3, color_g * 0.7);

			ofDrawCircle(color_g * 1.5 + 300, color_r * 3 - 0, color_g * 0.9);

			ofDrawCircle(color_r * 2 + 350, color_g * 1.8 + 100, color_b / 3);

			ofDrawCircle(sqrt(color_g * color_r), color_r * 2 - 50, color_b * 1.2);

			ofDrawCircle(color_g * 1.5 + 3, color_r * 3, color_g * 0.65);

			ofDrawCircle(color_g * 2 + 48, color_r * 1.5, color_g * 0.9);

			ofDrawCircle(color_r * 2 + 48, color_r * 2 + 48, color_r * 0.9);

			ofDrawCircle(color_b * 2.2 + 3, color_r * 2 + 20, color_r * 0.9);

			ofDrawCircle(color_g * 2 + 48, color_b * 4 + 48, color_r * 0.9);

			ofDrawCircle(color_r * 3 + 0, color_b * 2 + 48, color_r * 0.9);

			ofDrawCircle(color_g * 1.23 + 50, color_r * 2 + 48, color_r * 0.9);
			
			

			ofSetColor(50);
			ofSetLineWidth(1);
			ofDrawRectangle(0, 0, 300  , 90);

			ofSetColor(255,255,255);
			ofDrawBitmapString("Modulation : " + ofToString(currentmodulation), 20, 20);
			ofDrawBitmapString("Max modulation : " + ofToString(maxmodulation), 60, 60);
			ofDrawBitmapString("Min modulation : " + ofToString(minmodulation), 60, 80);
			ofSetColor(50);
			ofDrawRectangle(0, 90, 300, 90);
			ofSetColor(255, 255, 255);
			ofDrawBitmapString("Delay : " + ofToString(currentdelay), 20, 110);
			ofDrawBitmapString("Max delay : " + ofToString(maxdelay), 60, 150);
			ofDrawBitmapString("Min delay : " + ofToString(mindelay), 60, 170);
			
			

		ofPopMatrix();
	ofPopStyle();
	
	ofSetLineWidth(2);
	ofSetColor((int)color_r, (int)color_g, (int)color_b);
	ofNoFill();
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 5);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5+10);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 15);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5+20);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 40);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 60);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 80);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 100);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 120);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 140);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 160);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 180);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 200);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 220);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 240);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 260);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 280);
	ofDrawCircle(1024 / 2, 768 / 2, color_x * 5 + 300);
	
	
	
	
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------



//------Déclaration des variables------

float m_modulationAmplitude = 0.1f;
float m_modulationFrequency = 1.0f;

float m_Frequency = 200;

float Te = 1.0f / SAMPLE_RATE;
float T0 = 1.0f / m_Frequency;

float m_time = 0.0f;

float m_phase = 0.0f;
float m_phaseStep = TWO_PI * m_modulationFrequency / SAMPLE_RATE;

float m_sinPhase = 0.0f;
float m_sinPhaseStep = TWO_PI * m_Frequency / SAMPLE_RATE;

float m_reduce = 1.0f / 1.4f; //Réduire le gain pour le garder à 1. 1/(0.5+0.9)

float colorfreq = 0.01f;
float colorphase = 0.0f;
float colorstep = TWO_PI * colorfreq / SAMPLE_RATE;

float backCircleFreq = 200;
float tColor = 1.0f / backCircleFreq;


void ofApp::audioOut(ofSoundBuffer& buffer) {


	

	//------Reinitialize phase------
	while (m_phase > TWO_PI) {
		m_phase -= TWO_PI;
	}

	while (m_sinPhase > TWO_PI) {
		m_sinPhase -= TWO_PI;
	}

	while (colorphase > TWO_PI) {
		colorphase -= TWO_PI;
	}

	//------Loop on samples------
	for (int sampleIndex = 0; sampleIndex < buffer.size(); sampleIndex += buffer.getNumChannels()) {

		//------New delay------
		float modulation = m_modulationAmplitude * sin(m_phase);
		modulationAtomic.store(modulation);
		
		//-----Color sin for background-----
		
		color_r = 127 * sin(colorphase);
		color_g = 127 * sin(colorphase+3 * PI /4);
		color_b = 127 * sin(colorphase+PI);

		color_x = 127 * m_time / tColor ;
		//

		float m_delay = 0.9f + modulation;

		//------Storage------
		
		float delaySample = m_delay * SAMPLE_RATE;
		delayAtomic.store(m_delay);

		
		//------Direct sample------
		
		//Dent de scie
		float sample_in = m_time / T0;

		//Sinusoide
		//float sample_in = 0.5*sin(m_sinPhase);
		
		//------Delayed sample------        
		float sample_out = 0.5f * sample_in + 0.9f * m_pRingBuffer->getSample(delaySample);

		//------Sample_in goes into ring buffer------
		m_pRingBuffer->feed(sample_in);
		
		

		//------Loop on channels------
		for (int channelIndex = 0; channelIndex < buffer.getNumChannels(); channelIndex++) {
			//------Multiply by 1.4 to keep gain at 1------
			buffer[sampleIndex + channelIndex] = 0 * sample_out;

			
		}
		
		//------Increments------
		m_time += Te;
		m_phase += m_phaseStep;
		m_sinPhase += m_sinPhaseStep;

		//color
		colorphase += colorstep;

		//------Reinitialize time------
		if (m_time > T0) {
			m_time -= T0;
		}
	}

}