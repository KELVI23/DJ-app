#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }


    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats();
    
    
    startTimer(500);
 
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
   
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);



}


void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{

    mixerSource.getNextAudioBlock(bufferToFill);

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!


}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    deckGUI1.setBounds(0,0,getWidth()/2, getHeight()/2);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight()/2);

    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);

   
}

void MainComponent::timerCallback()
{
    std::ifstream file("library.txt");
    if (!file.is_open())
    {
        DBG("Failed to open library.txt");
        return;
    }

    std::string str;
    std::vector<std::string> lines;
    
    while (std::getline(file, str)){
        lines.push_back(str);
    }
    file.close();


    if (lines.size() < 2)
    {
        DBG("Invalid library.txt format");
        return;
    }

    int deck = std::stoi(lines[1]);

    if (deck == 1)
    {
        deckGUI1.playlistToDeck();
    }
    
    else if (deck == 2)
    {
        deckGUI2.playlistToDeck();
    }

    // Reset the library.txt file
    std::ofstream myfile("library.txt");
    myfile << "" << std::endl << "0" << std::endl;
    myfile.close();
}






