/*
  ==============================================================================

    DeckGUI.h
    Created: 15 Aug 2023 9:30:03am
    Author:  kmusodza

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "PlaylistComponent.h"
#include "WaveformDisplay.h"


//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager & formatManagerToUse,
            juce::AudioThumbnailCache & cacheToUse);

    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /*Implement Vutton:Listener*/
    void buttonClicked(juce::Button*) override;

    /*Implement slider listener*/
    void sliderValueChanged(juce::Slider* slider) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void timerCallback() override;
    juce::FileChooser fChooser{"Select a file..."};
    
    void playlistToDeck(); // reads which audio file user has selected to be added into Deck 1 or 2
    

private:
    juce::ImageButton playButton{ "PLAY" };
    juce::ImageButton stopButton{ "STOP" };
    juce::ImageButton loadButton{ "LOAD" };
    juce::ImageButton loopButton{ "LOOP" };
    juce::ImageButton stopLoopButton{ "STOP LOOP" };
    juce::Image stopLoopImg;
    juce::Image loopImg;



    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::TextEditor trackDetails;
    juce::String track;
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    
    juce::Label volSliderLabel;
    juce::Label speedSliderLabel;
    
    class CustomFontLookAndFeel : public juce::LookAndFeel_V4
    {public:}
    
    customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
