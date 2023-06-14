/*
  ==============================================================================

    CustomComponents.h
    Created: 14 Jun 2023 9:50:40am
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
struct RotaryKnob : public juce::Slider
{
    RotaryKnob(const juce::String& label = "Label") :
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox) {}
};

class ComponentLabel : public juce::Label
{
public:
    ComponentLabel(juce::Component& comp, const juce::String& labelText = "Name") :
        m_comp(comp),
        juce::Label::Label(labelText + "Label", labelText) { }

    //==============================================================================
    void InitialiseLabel()
    {
        auto lookAndFeel{ &m_comp.getLookAndFeel() };
        setLookAndFeel(lookAndFeel);
        setJustificationType(juce::Justification::centredTop);
        attachToComponent(&m_comp, false);
    }
private:
    juce::Component& m_comp;
};