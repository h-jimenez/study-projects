/*
  ==============================================================================

    Parameters.h
    Created: 16 Nov 2024 11:53:20am
    Author:  Hugo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

const juce::ParameterID gainParamID{ "gain",1 };
const juce::ParameterID feedbackParamID{ "feedback", 1 };
const juce::ParameterID lfoFreqParamID{ "lfoFreq",1 };
const juce::ParameterID mixParamID{ "mix",1 };
const juce::ParameterID delayTimeParamID{ "delayTime",1 };
const juce::ParameterID depthParamID{ "depth",1 };

class Parameters
{
public:

    Parameters(juce::AudioProcessorValueTreeState& apvts);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepareToPlay(double sampleRate) noexcept;
    void update() noexcept;
    void reset() noexcept;
    void smoothen() noexcept;

    //-------------------------------

    static constexpr float minDelayTime = 0.1f;
    static constexpr float maxDelayTime = 150.0f;

    float gain = 0.0f;
    float mix = 50.0f;
    float feedback = 25.0f;
    float lfoFreq = 0.5f;
    float delayTime = 0.0f;
    float depth = 5.0f;

private:

    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;

    juce::AudioParameterFloat* mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;

    juce::AudioParameterFloat* feedbackParam;
    juce::LinearSmoothedValue<float> feedbackSmoother;

    juce::AudioParameterFloat* lfoFreqParam;
    juce::LinearSmoothedValue<float> lfoFreqSmoother;

    juce::AudioParameterFloat* delayTimeParam;
    
    juce::AudioParameterFloat* depthParam;
    juce::LinearSmoothedValue<float> depthSmoother;


    float targetDelayTime = 0.0f;
    float coeff = 0.0f; // one-pole smoothing
};