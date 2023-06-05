/*
  ==============================================================================

    ListenerComponent.h
    Created: 5 Jun 2023 2:14:46pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ListenerComponent : public juce::Component, public juce::ChangeListener
{
public:
    ListenerComponent();
    void paint(juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:

};