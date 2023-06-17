/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 7 Jun 2023 5:12:30pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utility.h"

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();

    //==============================================================================
    void drawRotarySlider(juce::Graphics& g,
                          int x, int y,
                          int width, int height,
                          float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider&) override;

    void drawButtonBackground(juce::Graphics& g, 
                              juce::Button& button, 
                              const juce::Colour& backgroundColour,
                              bool isMouseOverButton, bool isButtonDown) override;
    void drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool) override;
    void drawLabel(juce::Graphics& g, juce::Label& l) override;

    //==============================================================================
    const juce::Colour& GetBackgroundColour();
    const juce::ColourGradient& GetGradient();

private:
    juce::Colour const m_backgroundClr;
    juce::ColourGradient m_gradient;
    juce::PathStrokeType const m_strokeType;
    juce::Font m_font;
};

