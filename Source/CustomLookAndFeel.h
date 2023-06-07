/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 7 Jun 2023 5:12:30pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct Vector2f
{
    float x;
    float y;
};

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        m_gradient.addColour(0, juce::Colours::rebeccapurple);
        m_gradient.addColour(.25, juce::Colours::blueviolet);
        m_gradient.addColour(.5, juce::Colours::deeppink);
        m_gradient.addColour(.75, juce::Colours::darkorange);
        m_gradient.addColour(1, juce::Colours::goldenrod);
    }
    //==============================================================================
    void drawRotarySlider(juce::Graphics& g,
        int x, int y,
        int width, int height,
        float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle,
        juce::Slider&) override
    {
        float radius{ static_cast<float>(juce::jmin(width, height) * .5f) };
        Vector2f containerCentre{ x + width * .5f, y + height * .5f };
        Vector2f ellipseCentre{ containerCentre.x - radius * .5f, containerCentre.y - radius * .5f };

        float angle{ rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle) };

        juce::Colour knobBaseClr{ juce::Colour::Colour(100, 100, 100) };
        juce::Colour knobAccentClr{ m_gradient.getColourAtPosition((angle - rotaryStartAngle) / (rotaryEndAngle - rotaryStartAngle)) };

        // drawing knob
        g.setColour(knobBaseClr);
        g.fillEllipse(ellipseCentre.x, ellipseCentre.y, radius, radius);

        g.setColour(knobAccentClr);
        g.drawEllipse(ellipseCentre.x, ellipseCentre.y, radius, radius, 5.f);

        // drawing pointer
        juce::PathStrokeType strokeType{ juce::PathStrokeType::PathStrokeType(radius * .05f,
                                              juce::PathStrokeType::JointStyle::curved,
                                              juce::PathStrokeType::EndCapStyle::rounded) };

        juce::Path pointer{};
        float pointerLength{ radius * .25f };
        float pointerThickness{ 10.f };

        pointer.addRoundedRectangle(-pointerThickness * .5f, -radius * .5f, pointerThickness, pointerLength, 2.f);
        pointer.applyTransform(juce::AffineTransform::rotation(angle).scaled(1.2f).translated(containerCentre.x, containerCentre.y));

        g.setColour(knobBaseClr);
        g.fillPath(pointer);
        g.setColour(knobAccentClr);
        g.strokePath(pointer, strokeType);

        // drawing arc around knob
        juce::Path pArc{};
        juce::Path pDashed{};
        const float dashLengths[2] = { 10.f, 15.f };

        pArc.addCentredArc(containerCentre.x, containerCentre.y,
            radius * .75f, radius * .75f,
            0.f,
            rotaryStartAngle, rotaryEndAngle,
            true);
        strokeType.createDashedStroke(pDashed, pArc, dashLengths, 2);

        g.setColour(knobAccentClr);
        g.strokePath(pDashed, strokeType);
    }

private:
    juce::ColourGradient m_gradient;
};