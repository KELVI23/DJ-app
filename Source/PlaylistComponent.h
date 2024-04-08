/*
  ==============================================================================

    PlaylistComponent.h
    Created: 21 Aug 2023 12:04:19pm
    Author:  kmusodza

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent(); 
    
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics&,
                            int     rowNumber,
                            int     width,
                            int     height,
                            bool     rowIsSelected) override;

    void paintCell(juce::Graphics&,
        int     rowNumber,
        int     columnId,
        int     width,
        int     height,
        bool     rowIsSelected) override;

    Component* refreshComponentForCell(int     rowNumber, int columnId, bool isRowSelected,
                                        Component* existingComponentToUpdate) override;
                                        

    void buttonClicked(juce::Button* button) override;
    
    void textEditorTextChanged(juce::TextEditor& editor) override;

    juce::String trackToDeck;
    std::string str;
    
    
    
    void deleteTrack();
    void loadDeck1();
    void loadDeck2();
    
    void updateHistoryAndTableComponent();


private:
    juce::TableListBox tableComponent;
    std::vector<std::string> trackTitles; //existingFiles
    
    juce::AudioFormatManager formatManager;
    juce::Array<juce::File> library; //myFiles
    
    
    double duration;
    juce::String trackName; //filename
    juce::Array<double> libraryDuration; //myFilesDuration
    juce::Array<juce::String> trackNames; //fileNames
    juce::ImageButton libraryLoadButton{ "Load into playlist" };
    juce::FileChooser fChooser{"Select file to add to the library..."};
  
    juce::TextButton DeleteButton;
    juce::TextButton deck1;
    juce::TextButton deck2;
    juce::TextEditor playlistSearch{ "Search playlist..." };
    juce::String searchInput;

    bool isHistory= false;
    int duplicateIdx = -1;
    double clickedRow;
    DJAudioPlayer player{ formatManager };
    //WaveformDisplay wave;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
