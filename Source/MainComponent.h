/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    /** implement Button::Listener */
   void buttonClicked (Button *) override;
    
private:
    void addNewDeck();

private:
    //==============================================================================
    // Your private member variables go here...
     
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100}; 
    
    std::vector<std::unique_ptr<DJAudioPlayer>> players;
    std::vector<std::unique_ptr<DeckGUI>> deckGUIs;
    TextButton addTrackButton{"ADD  TRACK"};

    MixerAudioSource mixerSource;
    
    int samplesPerBlockExpected_{0};
    double sampleRate_{0.0};
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
