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

struct RotaryKnob : public juce::Slider
{
    RotaryKnob(const juce::String& label = "Label") : 
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox) {}
};

class ComponentLabel : public juce::Label
{
public:
    ComponentLabel( juce::Component& comp, const juce::String& labelText = "Name") :
        m_comp( comp ),
        juce::Label::Label(labelText+"Label", labelText)
    {

    }
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

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel() :
        m_backgroundClr(juce::Colour(20, 20, 20)),
        m_gradient(),
        m_strokeType(juce::PathStrokeType::PathStrokeType(1.0f,
            juce::PathStrokeType::JointStyle::curved,
            juce::PathStrokeType::EndCapStyle::rounded)),
        m_font(20)
    {
        m_gradient.addColour(0, juce::Colours::rebeccapurple);
        m_gradient.addColour(.25, juce::Colours::blueviolet);
        m_gradient.addColour(.5, juce::Colours::deeppink);
        m_gradient.addColour(.75, juce::Colours::darkorange);
        m_gradient.addColour(1, juce::Colours::goldenrod);

        juce::Array<juce::Font> results;
        juce::Font::findFonts(results);

        m_font.setTypefaceName("Nunito");
        m_font.setTypefaceStyle("Light");
        m_font.setFallbackFontName("Arial");
    }
    //==============================================================================
    void drawRotarySlider(juce::Graphics& g,
        int x, int y,
        int width, int height,
        float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle,
        juce::Slider&) override
    {
        float radius{ static_cast<float>(juce::jmin(width, height) * .6f) };
        float angle{ rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle) };
        Vector2f containerCentre{ x + width * .5f, y + height * .5f };
        Vector2f ellipseCentre{ containerCentre.x - radius * .5f, containerCentre.y - radius * .5f };
        juce::Colour knobBaseClr{ juce::Colour::Colour(100, 100, 100) };
        juce::Colour knobAccentClr{ m_gradient.getColourAtPosition((angle - rotaryStartAngle) / (rotaryEndAngle - rotaryStartAngle)) };

        // drawing knob
        g.setColour(knobBaseClr);
        g.fillEllipse(ellipseCentre.x, ellipseCentre.y, radius, radius);

        g.setColour(knobAccentClr);
        g.drawEllipse(ellipseCentre.x, ellipseCentre.y, radius, radius, 5.f);

        // drawing pointer
        juce::Path pointer{};
        float pointerLength{ radius * .25f };
        float pointerThickness{ 10.f };

        pointer.addRoundedRectangle(-pointerThickness * .5f, -radius * .5f, pointerThickness, pointerLength, 2.f);
        pointer.applyTransform(juce::AffineTransform::rotation(angle).scaled(1.2f).translated(containerCentre.x, containerCentre.y));

        g.setColour(knobBaseClr);
        g.fillPath(pointer);
        g.setColour(knobAccentClr);
        g.strokePath(pointer, m_strokeType);

        // drawing arc around knob
        juce::Path arc{};
        juce::Path arcDashed{};
        const float dashLengths[2] = { 10.f, 15.f };

        arc.addCentredArc(containerCentre.x, containerCentre.y,
            radius * .75f, radius * .75f,
            0.f,
            rotaryStartAngle, rotaryEndAngle,
            true);
        m_strokeType.createDashedStroke(arcDashed, arc, dashLengths, 2);

        g.setColour(knobAccentClr);
        g.strokePath(arcDashed, m_strokeType);
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override

    {

        //g.fillAll(juce::Colours::green);
        juce::Colour colourA{ juce::Colours::crimson };
        juce::Colour btnBorderClr(isButtonDown ? colourA.brighter(0.1f) : colourA);
        
        juce::Colour colourB{ juce::Colours::goldenrod };
        juce::Colour btnCenterClr(isMouseOverButton ? (isButtonDown ? colourB.darker(.2f) : colourB.brighter(.2f)) : colourB);

        juce::Rectangle<int> bounds = button.getLocalBounds();
        juce::Rectangle<float> buttonBnds{ static_cast<float>(bounds.getTopLeft().getX()),
                                           static_cast<float>(bounds.getTopLeft().getY()),
                                           static_cast<float>(bounds.getWidth()),
                                           static_cast<float>(bounds.getHeight()) };
        g.setColour(btnBorderClr);
        buttonBnds.reduce(buttonBnds.getWidth() * .05f, buttonBnds.getHeight() * .05f);
        g.fillRoundedRectangle(buttonBnds, 5.f);
        g.setColour(btnCenterClr);
        buttonBnds.reduce(buttonBnds.getWidth() * .05f, buttonBnds.getHeight() * .05f);
        g.fillRoundedRectangle(buttonBnds, 5.f);

    }
    void drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool) {    }
    void drawLabel(juce::Graphics& g, juce::Label& l)
    {
        g.setColour(m_gradient.getColourAtPosition((l.getY() % 360) / 360.0));
        g.setFont(m_font);
        g.drawFittedText(l.getText(), 0, 0, l.getWidth(), l.getHeight(), l.getJustificationType(), 1);
    }
    //==============================================================================
    const juce::Colour& GetBackgroundColour()
    {
        return m_backgroundClr;
    }

    const juce::ColourGradient& GetGradient()
    {
        return m_gradient;
    }

private:
    juce::Colour const m_backgroundClr;
    juce::ColourGradient m_gradient;
    juce::PathStrokeType const m_strokeType;
    juce::Font m_font;
};

