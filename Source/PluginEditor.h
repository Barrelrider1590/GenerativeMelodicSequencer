/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct NoteVisual 
{
    NoteVisual(int idx, juce::Colour clr, juce::Rectangle<int> bounds)
        : index(idx), colour(clr), rect(bounds) {}
    
    int index;
    juce::Colour colour;
    juce::Rectangle<int> rect;
};

struct RotaryKnob : public juce::Slider
{
    RotaryKnob() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::NoTextBox) {}
};

struct Vector2f
{
    float x;
    float y;
};

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel(){}

    void drawRotarySlider(juce::Graphics& g,
        int x, int y,
        int width, int height,
        float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle,
        juce::Slider&) override
    {
        float radius{ static_cast<float>(juce::jmin(width, height) * .5f) };
        Vector2f containerCentre{ x + width * .5f, y + height * .5f };
        Vector2f ellipseCentre{ containerCentre.x - radius * .5f, containerCentre.y - radius * .5f};

        float angle{ rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle) };

        juce::ColourGradient myBeautifulGradient;
        myBeautifulGradient.addColour(0, juce::Colours::rebeccapurple);
        myBeautifulGradient.addColour(.25, juce::Colours::blueviolet);
        myBeautifulGradient.addColour(.5, juce::Colours::deeppink);
        myBeautifulGradient.addColour(.75, juce::Colours::darkorange);
        myBeautifulGradient.addColour(1, juce::Colours::goldenrod);

        // drawing knob
        g.setColour(juce::Colour::Colour(100, 100, 100));
        g.fillEllipse(ellipseCentre.x, ellipseCentre.y, radius, radius);

        g.setColour(myBeautifulGradient.getColourAtPosition((angle - rotaryStartAngle) / (rotaryEndAngle - rotaryStartAngle)));
        g.drawEllipse(ellipseCentre.x, ellipseCentre.y, radius, radius, 5.f);

        // drawing pointer
        juce::Path p{};
        float pointerLength{ radius * .25f };
        float pointerThickness{ 10.f };
        p.addRoundedRectangle(-pointerThickness, -radius * .5f, pointerThickness, pointerLength, 5.f);
        p.applyTransform(juce::AffineTransform::rotation(angle).scaled(1.2f).translated(containerCentre.x, containerCentre.y));

        g.setColour(myBeautifulGradient.getColourAtPosition((angle - rotaryStartAngle) / (rotaryEndAngle - rotaryStartAngle)));
        g.fillPath(p);

        juce::Path p2{};
        p2.addCentredArc(containerCentre.x, containerCentre.y,
            radius * .75f, radius * .75f,
            0.f,
            rotaryStartAngle, rotaryEndAngle,
            true);

        juce::PathStrokeType strokeType{ juce::PathStrokeType::PathStrokeType(radius * .05f,
                                                              juce::PathStrokeType::JointStyle::curved,
                                                              juce::PathStrokeType::EndCapStyle::rounded) };

        
        juce::Path pDashed{};
        const float dashLengths[2] = { 10.f, 15.f };
        strokeType.createDashedStroke(pDashed, p2, dashLengths, 2);
        g.setColour(myBeautifulGradient.getColourAtPosition((angle - rotaryStartAngle) / (rotaryEndAngle - rotaryStartAngle)));
        g.strokePath(pDashed, strokeType);
    }
};
//==============================================================================
/**
*/
class GenerativeMelodicSequencerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::ChangeListener
{
public:
    GenerativeMelodicSequencerAudioProcessorEditor (GenerativeMelodicSequencerAudioProcessor&);
    ~GenerativeMelodicSequencerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:

    std::vector<juce::Component*> GetComponents();

    juce::Colour RandomColour();

    //==============================================================================
    static int const m_maxNrOfNotes;
    static juce::Colour const m_backgroundClr;
    static CustomLookAndFeel m_lookAndFeel;
    
    GenerativeMelodicSequencerAudioProcessor& m_audioProcessor;

    std::vector<NoteVisual> m_notes;

    RotaryKnob m_bpmKnob;
    RotaryKnob m_loopLengthKnob;
    RotaryKnob m_gateKnob;
    RotaryKnob m_densityKnob;
    RotaryKnob m_mutateKnob;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment m_bpmKnobAttachment;
    Attachment m_loopLengthKnobAttachment;
    Attachment m_gateKnobAttachment;
    Attachment m_densityKnobAttachment;
    Attachment m_mutateKnobAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerativeMelodicSequencerAudioProcessorEditor)
};
