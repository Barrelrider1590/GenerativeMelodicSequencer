/*
  ==============================================================================

    BroadcasterComponent.h
    Created: 5 Jun 2023 2:14:19pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class BroadcasterComponent : public juce::Component, public juce::ChangeBroadcaster
{
public: 
    BroadcasterComponent();
    ~BroadcasterComponent();
    void paint(juce::Graphics& g) override;

private:

};
