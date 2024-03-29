/*
  ==============================================================================

    AdsrData.h
    Created: 4 Jan 2022 4:11:21pm
    Author:  Ryvenay

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
class AdsrData : public juce::ADSR
{
public:
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

private:
    juce::ADSR::Parameters adsrParams;

};
