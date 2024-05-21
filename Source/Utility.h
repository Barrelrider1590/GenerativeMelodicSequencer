/*
  ==============================================================================

    Utility.h
    Created: 16 Jun 2023 8:05:05am
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#pragma region Custom Components
struct RotaryKnob : public juce::Slider,
                    private juce::OSCReceiver,
                    private juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>
{
    RotaryKnob(const juce::String& label = "Label",
        const juce::String& oscReceiver = "/juce/",
        int valueStep = 1,
        int port = 1000) :
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxAbove),
        m_label(label),
        m_valueStep(valueStep)
    {
        if (!connect(port))
            showConnectionErrorMessage("Error: could not connect to UDP port");

        juce::OSCReceiver::addListener(this, oscReceiver);
    }
    ~RotaryKnob() {}

private:
    void oscMessageReceived(const juce::OSCMessage& message)
    {
        //DBG(m_label);
        if (message[0].getFloat32() != 0) // check if sensor value is below threshold
        {
            if (message[1].getFloat32() == 1) // check if value should be changed based on nr of fingers bent
            {
                DBG(m_label);
                if (message[2].getFloat32() > 0 && getValue() < getMaximum()) // check if value should be altered
                {
                    DBG("Accelerometer threshold crossed");
                    if(message[3].getFloat32() > 0 && getValue() < getMaximum()) // check if value should be incr or decr
                        if (getMaximum() <= 1.0f)
                        {
                            double scaledValue{ getValue() * 100 };
                            scaledValue += m_valueStep;
                            setValue(scaledValue * .01);
                        }
                        else
                        {
                            DBG("Value increasing");
                            setValue(getValue() + m_valueStep);
                        }
                    else
                    {
                        if (getMaximum() <= 1.0f)
                        {
                            double scaledValue{ getValue() * 100 };
                            scaledValue -= m_valueStep;
                            setValue(scaledValue * .01);
                        }
                        else
                        {
                            DBG("Value decreasing");
                            setValue(getValue() - m_valueStep);
                        }
                    }
                }
            }
        }
    }

    void showConnectionErrorMessage(const juce::String& messageText)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            "Connection error",
            messageText,
            "OK");
    }

    juce::String m_label;

    double m_valueStep;
};

class ComponentLabel : public juce::Label
{
public:
    ComponentLabel(juce::Component& comp, const juce::String& labelText = "Name") :
        m_comp(comp),
        juce::Label::Label(labelText + "Label", labelText) { }
    ~ComponentLabel() {}

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
#pragma endregion

#pragma region Data Structures
struct NoteVisual
{
    NoteVisual() :
        m_noteNr(0),
        m_index(0),
        m_isActive(false),
        m_offsetTotal(0),
        m_colourActive(juce::Colours::green),
        m_colourInactive(juce::Colours::red),
        m_rect() {}
    ~NoteVisual() {}

    //==============================================================================
    int m_noteNr;
    int m_index;
    bool m_isActive;
    float m_offsetTotal;
    juce::Colour m_colourActive;
    juce::Colour m_colourInactive;
    juce::Rectangle<int> m_rect;
};

struct SequencerSettings
{
    int scale{ 0 }, bpm{ 0 }, loopLength{ 0 };
    float gate{ 0 }, density{ 0 }, mutate{ 0 };
};

struct Vector2f
{
    float x;
    float y;
};
#pragma endregion