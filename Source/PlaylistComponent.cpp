/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 21 Aug 2023 12:04:19pm
    Author:  kmusodza

  ==============================================================================
*/

#include "PlaylistComponent.h"
#include <string>
#include <time.h>

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    //loading add to library button
    auto playListImg = juce::ImageCache::getFromMemory(BinaryData::playlist_png , BinaryData::playlist_pngSize);
    
    tableComponent.getHeader().addColumn("Track Title",1, 400);
    tableComponent.getHeader().addColumn("Duration(mm:ss)", 2, 200);
    tableComponent.getHeader().addColumn("Load into", 3, 150);
    tableComponent.getHeader().addColumn("Load into", 4, 150);
    tableComponent.getHeader().addColumn("Delete", 5, 100);
    tableComponent.setModel(this);
    
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(libraryLoadButton);
    addAndMakeVisible(playlistSearch);
    
    
    libraryLoadButton.addListener(this);
    playlistSearch.addListener(this);
    
    formatManager.registerBasicFormats();
    
    libraryLoadButton.setImages(true, true, true, playListImg, 0.5f, juce::Colours::darkred, playListImg, 1.0f, juce::Colours::transparentBlack,playListImg, 0.5f, juce::Colours::transparentBlack, 0.0f);
    
    
    //create library.txt and insert a placeholder "0"
    std::ofstream myLibrary("library.txt");
    myLibrary << str << std::endl << "0" << std::endl;
    myLibrary.close();
    
    //creats libraryHistory.txt to store current playlist audio file URLs
    std::ifstream file("libraryHistory.txt");
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
    
    //add playlist history
    for (int i = 0; i < lines.size(); ++i) {
        trackTitles.push_back(lines[i]);
    }
    
    //If playlist history exists
    if (trackTitles.size() != 0) {
        
        //add tracks to files
        for (int i = 0; i < trackTitles.size(); ++i) {
            library.add(juce::File{ trackTitles[i] });
            
        }
        
        for (int i = 0; i < library.size(); ++i) {
            
            trackName = juce::URL::removeEscapeChars(juce::URL{ library[i] }.getFileName()); //get track name
            trackNames.add(trackName);
            juce::AudioFormatReader* reader = formatManager.createReaderFor(library[i]);
            duration = reader->lengthInSamples / reader->sampleRate; //get duration of track in the library
            libraryDuration.add(duration);
            
        }
        tableComponent.updateContent();
    }
}


PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    double rowH = getHeight() / 8;
    libraryLoadButton.setBounds(0, 0, getWidth() / 7, rowH);
    playlistSearch.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    tableComponent.setBounds(0, rowH, getWidth(), getHeight());
    
    
    //playlist header columns
    tableComponent.getHeader().setColumnWidth(1, (getWidth() / 8) * 3);
    tableComponent.getHeader().setColumnWidth(2, getWidth() / 8);
    tableComponent.getHeader().setColumnWidth(3, (getWidth() / 8) * 1.5);
    tableComponent.getHeader().setColumnWidth(4, (getWidth() / 8) * 1.5);
    tableComponent.getHeader().setColumnWidth(5, (getWidth() / 8));
    
    //search bar
    playlistSearch.setTextToShowWhenEmpty("Search for tracks...", juce::Colours::orange);
    playlistSearch.setFont(20.0f);
    
    tableComponent.setColour(juce::ListBox::backgroundColourId, juce::Colours::lightblue);
    
}

int PlaylistComponent::getNumRows()
{
//    return trackTitles.size();
    return trackNames.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber,int width,
                                            int height, bool rowIsSelected)
{
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    }
    else {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId,
                                    int width, int height, bool rowIsSelected)
{

                
    // display track name
    if (columnId == 1) {
        g.drawText(trackNames[rowNumber], 2, 0, width - 4, height,
                   juce::Justification::centredLeft, true);
    }
    
    //display track length in mm:ss
     if (columnId == 2) {
                std::time_t seconds(libraryDuration[rowNumber]);
                tm* p = gmtime(&seconds);
                std::string mmss;
                std::string mins;
                std::string secs;

                if (p->tm_min < 10) {
                    mins = "0" + std::to_string(p->tm_min);
                }
                else {
                    mins = std::to_string(p->tm_min);
                }
                if (p->tm_sec < 10) {
                    secs = "0" + std::to_string(p->tm_sec);
                }
                else {
                    secs = std::to_string(p->tm_sec);
                }

                mmss =  mins + ":" + secs;
                g.drawText(mmss,
                    2, 0,
                    width - 4, height,
                    juce::Justification::centredLeft,
                    true);
            }
}

juce::Component* PlaylistComponent::refreshComponentForCell (int     rowNumber, int columnId, bool isRowSelected,
                                        juce::Component* existingComponentToUpdate)
{
    // load the audio file into deck 1
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton{ "Deck 1" };
            juce::String id{ std::to_string(rowNumber) };
            
            //id for button clicked
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {loadDeck1(); };
        }
    }
    
    // load the audio file into deck 1
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton{ "Deck 2" };
            juce::String id{ std::to_string(rowNumber) };
            
            //button id
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {loadDeck2(); };
        }
    }
    
    //delete audio file
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton{ "Delete" };
            juce::String id{ std::to_string(rowNumber) };
            
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {deleteTrack(); };
        }
    }
    return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(juce::Button* button) {
    if (button == &libraryLoadButton) {
        std::cout<<"Load library buton clicked"<<std::endl;

        
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser){
            auto selectedFiles = chooser.getResults();
            
        for (const juce::File& chosenFile : selectedFiles) {
            // Process each selected file here
            DBG("Selected File: " + chosenFile.getFullPathName());

            // Check for duplicates
            bool isDuplicate = false;

            for (int j = 0; j < trackTitles.size(); ++j) {
                if (chosenFile.getFullPathName().toStdString() == trackTitles[j]) {
                    DBG("There are duplicates");

                    // Creates an alert window alerting the user they have added a duplicate file, and it will not be added
                    int result = juce::AlertWindow::showOkCancelBox(juce::AlertWindow::WarningIcon,
                                                                     "Duplicate file added",
                                                                     "Duplicated file " + juce::URL::removeEscapeChars(juce::URL{chosenFile}.getFileName())
                                                                    + " has not been added",
                                                                     "Continue",
                                                                     "Cancel",
                                                                     this,
                                                                    nullptr);

                    if (result == 0) {
                        isDuplicate = true;
                    }

                    if (isDuplicate) {
                        break;
                    }
                }
            }

            if (!isDuplicate) {
                // Add the track tolibrary
                trackName = juce::URL::removeEscapeChars(juce::URL{chosenFile}.getFileName());
                trackNames.add(trackName);

                juce::AudioFormatReader* reader = formatManager.createReaderFor(chosenFile);
                duration = reader->lengthInSamples / reader->sampleRate;
                libraryDuration.add(duration);
                DBG(trackNames.getLast());
                DBG(duration);

                trackTitles.push_back(chosenFile.getFullPathName().toStdString());
            }
        }

        // Update library history
        std::ofstream history("libraryHistory.txt");

        for (int i = 0; i < trackTitles.size(); ++i) {
            history << trackTitles[i] << std::endl;
        }
        history.close();

         //Update table component
        tableComponent.updateContent();
        });
        
        clickedRow = button->getComponentID().getDoubleValue();
    }
}


// Handle search bar input
void PlaylistComponent::textEditorTextChanged(juce::TextEditor& editor) {
    
    searchInput = playlistSearch.getText();

    for (int i = 0; i < trackNames.size(); ++i) {
        if (trackNames[i].containsIgnoreCase(searchInput) && searchInput != "") {
            tableComponent.selectRow(i, false, true);
                tableComponent.updateContent();
                repaint();
        } else{
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Invalid search", "No tracks were found");

        }
    }
    
}

   


void PlaylistComponent::deleteTrack()
{
    DBG(clickedRow);

    for (int i = 0; i < library.size(); ++i)
    {
        if (clickedRow == i)
        {
            // Create an alert window for deleting the track
            int result = juce::AlertWindow::showOkCancelBox(
                juce::AlertWindow::QuestionIcon,
               "Delete track",
                "Are you sure you want to delete this track?",
                "OK",    // Button 1 text
                "Cancel", // Button 2 text
                 this, nullptr
                 );

            if (result == 0)
            {
                // delete the track
                DBG("delete track");
                trackNames.remove(i);
                libraryDuration.remove(i);
                trackTitles.erase(trackTitles.begin() + i);
            }
        }
    }


    // Opens LibraryHistory.txt and deletes track
    std::ofstream history("LibraryHistory.txt");
    for (int i = 0; i < trackTitles.size(); ++i)
    {
        history << trackTitles[i] << std::endl;
    }
    history.close();

    tableComponent.updateContent();
}


void PlaylistComponent::loadDeck1() {
    
    for (int i = 0; i < library.size(); ++i) {
        juce::AudioFormatReader* reader = formatManager.createReaderFor(library[i]);
        
        if (clickedRow == i) {
            DBG("load deck1");
            DBG(library[i].getFileName());
            DBG(reader->getFormatName());
            
            trackToDeck = library[i].getFullPathName();
            str = trackToDeck.toStdString();
            
            std::replace(str.begin(), str.end(), '\\', '/');
            DBG(str);
            
            std::ofstream myLibrary("library.txt");
            myLibrary << str << std::endl << "1" << std::endl;
            myLibrary.close();
            
        }
    }
}

void PlaylistComponent::loadDeck2() {
    for (int i = 0; i < library.size(); ++i) {
        juce::AudioFormatReader* reader = formatManager.createReaderFor(library[i]);
        
        if (clickedRow == i) {
            DBG("load deck2");
            DBG(library[i].getFileName());
            DBG(reader->getFormatName());
            
            trackToDeck = library[i].getFullPathName();
            str= trackToDeck.toStdString();
            
            std::replace(str.begin(), str.end(), '\\', '/');
            DBG(str);
            
            std::ofstream myLibrary("library.txt");
            myLibrary<< str << std::endl << "2" << std::endl;
            myLibrary.close();
        }
    }
}












