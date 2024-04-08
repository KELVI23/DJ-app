/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 14 Aug 2023 3:22:19pm
    Author:  kmusodza

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource {
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();
    /*get the relative position of the playhead*/
    double getPositionRelative();
    
    /* set audio loop */
    void setLoop(bool b);


private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr < juce::AudioFormatReaderSource> readerSource; //smart pointer with auto memory management
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
