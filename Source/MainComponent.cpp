/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
    
    addNewDeck();
    addNewDeck();

    addAndMakeVisible(addTrackButton);
    addTrackButton.addListener(this);

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    for (auto& player : players)
    {
        player->prepareToPlay(samplesPerBlockExpected, sampleRate);
        mixerSource.addInputSource(player.get(), false);
    }
    samplesPerBlockExpected_ = samplesPerBlockExpected;
    sampleRate_ = sampleRate;

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    for (auto& player : players)
    {
        player->releaseResources();
    }
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    int numDecks = deckGUIs.size();
    int deckHeight = 200; // Fixed height for each DeckGUI
    int buttonHeight = 50; // Fixed height for the AddTrack button

    int totalHeight = numDecks * deckHeight + buttonHeight; // Calculate the new total height of the component

    setSize(getWidth(), totalHeight); // **Dynamically adjust the main window height**

    for (size_t i = 0; i < deckGUIs.size(); ++i)
    {
        deckGUIs[i]->setBounds(0, i * deckHeight, getWidth(), deckHeight);
    }

    addTrackButton.setBounds(0, numDecks * deckHeight, getWidth(), buttonHeight);
}


void MainComponent::buttonClicked(Button* button)
{
    if (button == &addTrackButton)
    {
        std::cout << "AddTrack button was clicked" << std::endl;
        addNewDeck();
    }
}

void MainComponent::addNewDeck()
{
    players.push_back(std::make_unique<DJAudioPlayer>(formatManager));
    deckGUIs.push_back(std::make_unique<DeckGUI>(players.back().get(), formatManager, thumbCache));
    
    prepareToPlay(samplesPerBlockExpected_, sampleRate_);

    addAndMakeVisible(*deckGUIs.back());

    resized(); // 重新布局
}

