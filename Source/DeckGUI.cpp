/*
  ==============================================================================

    DeckGUI.cpp
    Created: 15 Aug 2023 9:30:03am
    Author:  kmusodza

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "CustomLookAndFeel.h"


//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                juce::AudioFormatManager& formatManagerToUse,
                juce::AudioThumbnailCache& cacheToUse
                ) : player(_player),
                    waveformDisplay(formatManagerToUse, cacheToUse)
{
        //loading pause and play button images
    auto pauseImg = juce::ImageCache::getFromMemory(BinaryData::pausebutton_png, BinaryData::pausebutton_pngSize);
    auto playImg = juce::ImageCache::getFromMemory(BinaryData::playbutton_png, BinaryData::playbutton_pngSize);
    
    //loading load into deck image
    auto loadImg = juce::ImageCache::getFromMemory(BinaryData::load_png , BinaryData::load_pngSize);
    
    //load loop button
    auto loopImg = juce::ImageCache::getFromMemory(BinaryData::loop_png , BinaryData::loop_pngSize);

    auto stopLoopImg = juce::ImageCache::getFromMemory(BinaryData::stopLoop_png, BinaryData::stopLoop_pngSize);
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);
    //addAndMakeVisible(stopLoopButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(trackDetails);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
    //stopLoopButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    
    

    volSlider.setRange(0.0, 1.0);
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical); // Use LinearVertical style



    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50,16);
    volSlider.setRange(0.0, 1.0, 0.01); // Set the range of the slider
    volSlider.setValue(0.5); // Set the initial value
    volSlider.setSkewFactorFromMidPoint(0.3); // Adjust skew factor for better control
    volSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red); // Set thumb color
    volSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey); // Set background color
    volSlider.setColour(juce::Slider::trackColourId, juce::Colours::lightblue); // Set track color
    
    
    
    speedSlider.setRange(0.0, 100);
    speedSlider.setLookAndFeel(&customLookAndFeel);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 19);
    
    //label speed slider
    speedSliderLabel.setText("Speed", juce::NotificationType::dontSendNotification);
    speedSliderLabel.attachToComponent(&speedSlider, false);

    posSlider.setRange(0.0, 1.0);
    posSlider.setSliderStyle(juce::Slider::LinearBar);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox,true,0,0);
    //make posSlider transparent
    posSlider.setColour(juce::Slider::trackColourId,juce::Colours::transparentBlack);
    
    
     //Setting stop and play button with images
    stopButton.setImages(true,true,true,pauseImg, 0.5f, juce::Colours::red, pauseImg, 1.0f, juce::Colours::transparentBlack, pauseImg, 0.5f, juce::Colours::transparentWhite, 0.0f);
    playButton.setImages(true, true, true, playImg, 0.5f, juce::Colours::red, playImg, 1.0f, juce::Colours::transparentBlack, playImg, 0.5f, juce::Colours::transparentWhite, 0.0f);

    //set load button with image
    loadButton.setImages(true, true, true, loadImg, 0.5f, juce::Colours::darkred, loadImg, 1.0f, juce::Colours::transparentWhite,loadImg, 0.5f, juce::Colours::transparentWhite, 0.0f);
    //set loop button with image
    loopButton.setImages(true, true, true, loopImg, 0.5f, juce::Colours::darkred, loopImg, 1.0f, juce::Colours::transparentWhite,loopImg, 0.5f, juce::Colours::transparentWhite, 0.0f);
    
    
    //stopLoopButton.setImages(true, true, true, stopLoopImg, 0.5f, juce::Colours::darkred, stopLoopImg, 1.0f, juce::Colours::transparentWhite, stopLoopImg, 0.5f, juce::Colours::transparentWhite, 0.0f);

    
     //track name displayed on top of the deck
    trackDetails.setReadOnly(true);
    trackDetails.setTextToShowWhenEmpty("Please load track", juce::Colours::orange);



    startTimer(100);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
 

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

}

void DeckGUI::resized()
{
    
    double rowH = getHeight() / 8;
    playButton.setBounds(0, 0, getWidth()/8, rowH);
    stopButton.setBounds(getWidth() / 8, 0, getWidth() / 7, rowH);
    loadButton.setBounds((getWidth() / 7)*6, 0, getWidth()/7, rowH);
    loopButton.setBounds((getWidth() / 7)*5, 0, getWidth()/6, rowH);

    volSlider.setBounds(0, rowH * 2, getWidth()* 1.4, rowH * 3);

    speedSlider.setBounds(0, rowH * 2, getWidth()/2, rowH * 3);

    
//    posSlider.setBounds(0, rowH * 5, getWidth(), rowH * 2); //overlay slider on waveform
   
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    posSlider.setBounds(0, rowH * 4.6, getWidth()*2, rowH*2);
    speedSliderLabel.setCentreRelative(0.44f,0.4f);
    //volSliderLabel.setCentreRelative(0.44f,0.4f);
    
    trackDetails.setBounds(0, rowH * 7, getWidth(), rowH);
    trackDetails.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    trackDetails.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
    


}

void DeckGUI::buttonClicked(juce::Button* button) {
    
    if (button == &playButton) {
        std::cout << "Play button was clicked" << std::endl;
        //player->setPosition(0);
        player->start();
    }

    if (button == &stopButton) {
        std::cout << "Stop button was clicked" << std::endl;
        player->stop();
    }

    if (button == &loadButton) {

        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser){
            auto chosenFile = chooser.getResult();
            waveformDisplay.loadURL(juce::URL{chosenFile});
            player->loadURL(juce::URL{chosenFile});
            
            trackDetails.setText(juce::URL{ chooser.getResult() }.getFileName(), juce::NotificationType::dontSendNotification);

            trackDetails.setText(juce::URL{ chooser.getResult() }.getFileName(), juce::NotificationType::dontSendNotification);
        });
    }

    if (button == &loopButton) {
        
        player->setLoop(button->getToggleState());
        DBG("Looping clicked" );
  }
        
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volSlider) {
         DBG("MainComponent::sliderValueChanged:gainSlider" << volSlider.getValue());

         player->setGain(slider->getValue());

     }

     if (slider == &speedSlider) {
         player->setSpeed(slider->getValue());
     }

     if (slider == &posSlider) {
         player->setPositionRelative(slider->getValue());
     }
     
}


bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}



void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y)
{
   for (juce::String filename : files) {
        std::cout << "DeckGUI::filesDropped" << filename << std::endl;
        juce::URL fileURL = juce::URL{ juce::File{filename} };
        waveformDisplay.loadURL(fileURL);
        player->loadURL(fileURL);
        return;
    }
}

void DeckGUI::timerCallback()
{
    if (loopButton.getToggleState()) { // If the loop button is checked
        if (player->getPositionRelative() >= 1) {
            player->setPositionRelative(0);
            player->setLoop(loopButton.getToggleState());
            DBG("Looping clicked" );
        }
    }
    else { // If the loop button is not checked
        if (player->getPositionRelative() >= 1) {
            player->setPositionRelative(0);
            player->stop();
            player->setLoop(loopButton.getToggleState());
            DBG("Looping unclicked" );
        }
    }
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::playlistToDeck() {

        std::ifstream file("library.txt");
        std::string str;
        std::getline(file, str);
        std::string URL = "file:///" + str;
        DBG(URL);
    
        juce::URL audioURL{ URL };
        player->loadURL(audioURL);
        waveformDisplay.loadURL(audioURL);
        DBG(audioURL.getFileName());

        track = audioURL.getFileName();
        trackDetails.setText(track, juce::NotificationType::dontSendNotification);


}






