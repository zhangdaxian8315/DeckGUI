/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2025 6:44:48pm
    Author:  Daxian

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(echoButton);
    
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    echoButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);


    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0001, 100.0);
    posSlider.setRange(0.0, 1.0);
    
    volSlider.setValue(1.0);
    speedSlider.setValue(1.0);
    posSlider.setValue(0.0);
    
    volSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setNumDecimalPlacesToDisplay(2);

    startTimer(500);
    
    volLabel.setText("Volume", juce::dontSendNotification);
    speedLabel.setText("Speed", juce::dontSendNotification);
    posLabel.setText("Position", juce::dontSendNotification);

    volLabel.setJustificationType(juce::Justification::right);
    speedLabel.setJustificationType(juce::Justification::right);
    posLabel.setJustificationType(juce::Justification::right);

    volLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    speedLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    posLabel.setColour(juce::Label::textColourId, juce::Colours::white);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
//    g.drawText ("DeckGUI", getLocalBounds(),
//                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    int totalWidth = getWidth();
    int totalHeight = getHeight();

    // Group C (combination of buttons and sliders) takes up the top half
    int groupCHeight = totalHeight * 0.5; // Group C takes 50% of the height
    int buttonHeight = groupCHeight;
    int sliderHeight = groupCHeight / 3;

    int groupWidth = totalWidth / 2; // Width of Group C (buttons and sliders side by side)
    int buttonWidth = groupWidth / 4; // Four buttons, divide width equally
    int labelWidth = groupWidth / 5; // Width of the labels
    int sliderWidth = groupWidth - labelWidth; // Width of the sliders

    // Group A (button group) - arrange three buttons horizontally
    playButton.setBounds(0, 0, buttonWidth, buttonHeight);
    stopButton.setBounds(buttonWidth, 0, buttonWidth, buttonHeight);
    loadButton.setBounds(buttonWidth * 2, 0, buttonWidth, buttonHeight);
    echoButton.setBounds(buttonWidth * 3, 0, buttonWidth, buttonHeight);

    // Group B (slider group) - arrange sliders vertically
    int sliderStartY = 0; // Starting Y position for the sliders

    volLabel.setBounds(groupWidth, sliderStartY, labelWidth, sliderHeight);
    volSlider.setBounds(groupWidth + labelWidth, sliderStartY, sliderWidth, sliderHeight);

    speedLabel.setBounds(groupWidth, sliderStartY + sliderHeight, labelWidth, sliderHeight);
    speedSlider.setBounds(groupWidth + labelWidth, sliderStartY + sliderHeight, sliderWidth, sliderHeight);

    posLabel.setBounds(groupWidth, sliderStartY + sliderHeight * 2, labelWidth, sliderHeight);
    posSlider.setBounds(groupWidth + labelWidth, sliderStartY + sliderHeight * 2, sliderWidth, sliderHeight);

    // Group D (waveformDisplay) takes up the bottom half
    waveformDisplay.setBounds(0, groupCHeight, totalWidth, totalHeight - groupCHeight);
}


void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)
    {
       auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            if (chosenFile.exists()){
                player->loadURL(URL{chooser.getResult()});
                waveformDisplay.loadURL(URL{chooser.getResult()});
            }
        });
    }
    if (button == &echoButton) {
        player->echo();
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
    
    double pos = player->getPositionRelative();
    if (!std::isnan(pos)) {
        posSlider.setValue(pos);
    } else {
        posSlider.setValue(0.0);
    }
}


    

