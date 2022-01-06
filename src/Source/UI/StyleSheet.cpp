/*
  ==============================================================================

    StyleSheet.cpp
    Created: 4 Jan 2022 6:11:56pm
    Author:  FCsaba

  ==============================================================================
*/

#include "StyleSheet.h"
#include "Colours.h"

namespace juce {

    void CustomLNF::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
    {
        auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
        auto fill = slider.findColour(Slider::rotarySliderFillColourId);

        auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);

        auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = jmin(8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;


        //dial background path
        Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            rotaryEndAngle,
            true);

        g.setColour(OurColours::BaseColourBrighter);
        g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            Path valueArc;
            valueArc.addCentredArc(bounds.getCentreX(),
                bounds.getCentreY(),
                arcRadius,
                arcRadius,
                0.0f,
                rotaryStartAngle,
                toAngle,
                true);

            g.setColour(OurColours::HighlightColor);
            g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));
        }

        Point<float> thumbPoint(bounds.getCentreX() + (arcRadius - 10) * std::cos(toAngle - MathConstants<float>::halfPi),
            bounds.getCentreY() + (arcRadius - 10) * std::sin(toAngle - MathConstants<float>::halfPi));

        g.setColour(OurColours::HighlightColor);
        g.drawLine(
            backgroundArc.getBounds().getCentreX(),
            backgroundArc.getBounds().getCentreY(),
            thumbPoint.getX(), thumbPoint.getY(), lineW);

    }

}