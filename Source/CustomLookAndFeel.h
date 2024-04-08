/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 15 Sep 2023 10:43:06pm
    Author:  Kelvin Musodza

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomLookAndFeel: public juce::LookAndFeel_V4
{
public:
    //CustomLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;


};
