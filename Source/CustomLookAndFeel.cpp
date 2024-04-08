/*
  ============================================================================
    CustomLookAndFeel
    Created: 15 Sep 2023 8:19:4
    Author:  Kelvin Musoda
  ===========================================================================
**/

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"


    void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider)
    {
        // Calculate the center of the jog wheel
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;

        // Calculate the radius of the jog wheel
        const float radius = juce::jmin(width, height) * 0.5f;

        // Set up the colors for the jog wheel
        const juce::Colour backgroundColour = juce::Colours::black; // Background color
        const juce::Colour knobColour = juce::Colours::lightgrey;   // Color of the jog wheel knob
        const juce::Colour glossyColour = juce::Colours::white;     // Color for the glossy highlight

        // Draw the background
        g.setColour(backgroundColour);
        g.fillEllipse(x, y, width, height);

        // Draw the jog wheel knob
        g.setColour(knobColour);
        g.fillEllipse(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f);

        // Draw a glossy highlight
        const float glossyRadius = radius * 0.75f;
        const float glossyPosX = centerX - glossyRadius;
        const float glossyPosY = centerY - glossyRadius;
        const juce::Colour glossyGradientColour = glossyColour.withAlpha(0.7f);

        juce::Path glossyPath;
        glossyPath.addEllipse(glossyPosX, glossyPosY, glossyRadius * 2.0f, glossyRadius * 2.0f);

        juce::ColourGradient glossyGradient(glossyGradientColour, glossyPosX, glossyPosY,
                                            glossyColour, glossyPosX, glossyPosY + glossyRadius * 2.0f, false);

        g.setGradientFill(glossyGradient);
        g.fillPath(glossyPath);

    }

