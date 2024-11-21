/*
  ==============================================================================

    Parameters.cpp
    Created: 16 Nov 2024 11:53:20am
    Author:  Hugo

  ==============================================================================
*/

#include "Parameters.h"






template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
}



Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{

    castParameter<juce::AudioParameterFloat*>(apvts, gainParamID, gainParam);
    castParameter<juce::AudioParameterFloat*>(apvts, mixParamID, mixParam);
    castParameter<juce::AudioParameterFloat*>(apvts, feedbackParamID, feedbackParam);
    castParameter<juce::AudioParameterFloat*>(apvts, lfoFreqParamID, lfoFreqParam);
    castParameter<juce::AudioParameterFloat*>(apvts, delayTimeParamID, delayTimeParam);
    castParameter<juce::AudioParameterFloat*>(apvts, depthParamID, depthParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID,
        "Gain",
        juce::NormalisableRange<float>{-60.0f, 12.0f },
        0.0f,
        juce::AudioParameterFloatAttributes()
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        mixParamID,
        "Mix",
        juce::NormalisableRange<float>{0.0f, 100.0f },
        50.0f,
        juce::AudioParameterFloatAttributes()
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        feedbackParamID,
        "Feedback",
        juce::NormalisableRange<float>{0.0f,100.0f },
        25.0f,
        juce::AudioParameterFloatAttributes()
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        lfoFreqParamID,
        "Frequency",
        juce::NormalisableRange<float>{0.1f,5.0f },
        0.5f,
        juce::AudioParameterFloatAttributes()
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        delayTimeParamID,
        "Delay Time",
        juce::NormalisableRange<float>{minDelayTime,maxDelayTime/2,0.001f,0.2f},
        1.0f,
        juce::AudioParameterFloatAttributes()
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        depthParamID,
        "Depth",
        juce::NormalisableRange<float>{0.0f,100.0f},
        5.0f,
        juce::AudioParameterFloatAttributes()
    ));
    

    return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    mixSmoother.reset(sampleRate, duration);
    feedbackSmoother.reset(sampleRate, duration);
    lfoFreqSmoother.reset(sampleRate, duration);
    coeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));
    depthSmoother.reset(sampleRate, duration);
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    feedbackSmoother.setTargetValue(feedbackParam->get() * 0.01f);
    lfoFreqSmoother.setTargetValue(lfoFreqParam->get());
    targetDelayTime = delayTimeParam->get();
    if (delayTime == 0.0f)
    {
        delayTime = targetDelayTime;
    }
    depthSmoother.setTargetValue(depthParam->get()*0.01f);
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));

    mix = 50.0f;
    mixSmoother.setCurrentAndTargetValue(mixParam->get());

    feedback = 25.0f;
    feedbackSmoother.setCurrentAndTargetValue(feedbackParam->get());

    lfoFreq = 0.5f;
    lfoFreqSmoother.setCurrentAndTargetValue(lfoFreqParam->get());

    
    delayTime = 0.001f;
    
    depth = 5.0f;
    depthSmoother.setCurrentAndTargetValue(depthParam->get());

}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    mix = mixSmoother.getNextValue();
    feedback = feedbackSmoother.getNextValue();
    lfoFreq = lfoFreqSmoother.getNextValue();
    depth = depthSmoother.getNextValue();

    delayTime = delayTime + (targetDelayTime - delayTime) * coeff;
}
