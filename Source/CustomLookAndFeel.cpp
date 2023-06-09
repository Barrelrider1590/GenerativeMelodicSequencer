/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 16 Jun 2023 8:49:02am
    Author:  Phoenix

  ==============================================================================
*/

#include "CustomLookAndFeel.h"
CustomLookAndFeel::CustomLookAndFeel() :
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

    m_font.setTypefaceName("NovaMono");
    m_font.setTypefaceStyle("Light");
    m_font.setFallbackFontName("Arial");
}
CustomLookAndFeel::~CustomLookAndFeel() {}

//==============================================================================
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g,
                                         int x, int y,
                                         int width, int height,
                                         float sliderPos,
                                         float rotaryStartAngle, float rotaryEndAngle,
                                         juce::Slider& s)
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

    float lineThickness{ 5.f };
    g.setColour(knobAccentClr);
    g.drawEllipse(ellipseCentre.x, ellipseCentre.y, radius, radius, lineThickness);

    // drawing pointer
    juce::Path pointer{};
    float pointerLength{ radius * .25f };
    float pointerThickness{ 10.f };
    float cornerSize{ 2.f };

    pointer.addRoundedRectangle(-pointerThickness * .5f, -radius * .5f, pointerThickness, pointerLength, cornerSize);
    pointer.applyTransform(juce::AffineTransform::rotation(angle).scaled(1.2f).translated(containerCentre.x, containerCentre.y));

    g.setColour(knobBaseClr);
    g.fillPath(pointer);
    g.setColour(knobAccentClr);
    g.strokePath(pointer, m_strokeType);

    // drawing arc around knob
    juce::Path arc{};
    juce::Path arcDashed{};
    const int nrOfDashLengths{ 2 };
    const float dashLengths[nrOfDashLengths] = { 10.f, 15.f };
    float rotation{ 0 };

    arc.addCentredArc(containerCentre.x, containerCentre.y,
                      radius * .75f, radius * .75f,
                      rotation,
                      rotaryStartAngle, rotaryEndAngle,
                      true);
    m_strokeType.createDashedStroke(arcDashed, arc, dashLengths, nrOfDashLengths);

    g.setColour(knobBaseClr);
    g.strokePath(arcDashed, m_strokeType);
}
void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
    bool isMouseOverButton, bool isButtonDown)
{
    float cornerSize{ 5.f };
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
    g.fillRoundedRectangle(buttonBnds, cornerSize);
    g.setColour(btnCenterClr);
    buttonBnds.reduce(buttonBnds.getWidth() * .05f, buttonBnds.getHeight() * .05f);
    g.fillRoundedRectangle(buttonBnds, cornerSize);
}
void CustomLookAndFeel::drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool) {    }
void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& l)
{
    Vector2f position{ 0,0 };
    int nrOfLines{ 1 };

    g.setColour(juce::Colour(200, 200, 200));
    g.setFont(m_font);
    g.drawFittedText(l.getText(), position.x, position.y, l.getWidth(), l.getHeight(), l.getJustificationType(), nrOfLines);
}

//==============================================================================
const juce::Colour& CustomLookAndFeel::GetBackgroundColour()
{
    return m_backgroundClr;
}

const juce::ColourGradient& CustomLookAndFeel::GetGradient()
{
    return m_gradient;
}