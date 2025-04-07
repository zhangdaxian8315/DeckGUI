/*
  ==============================================================================

 EchoEffect.h
    Created: 05 March 2025 2:20:20pm
    Author:  Daxian

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class EchoEffect {
public:
    EchoEffect() {
        SetDelayTime(0.5f);
        SetFeedback(0.5f);
    }

    /**
     * @brief Sets the echo delay time.
     * @param new_delay_time Delay time in seconds.
     */
    void SetDelayTime(float new_delay_time) {
        delay_time_ = new_delay_time;
        int delay_samples = static_cast<int>(delay_time_ * sample_rate_);
        delay_line_.setDelay(delay_samples);
    }

    /**
     * @brief Sets the feedback level of the echo effect.
     * @param new_feedback Feedback coefficient (0.0 to 1.0).
     */
    void SetFeedback(float new_feedback) {
        feedback_ = new_feedback;
    }

    /**
     * @brief Prepares the effect with the given sample rate and block size.
     * @param new_sample_rate The sample rate of the audio stream.
     * @param samples_per_block The number of samples per processing block.
     */
    void Prepare(double new_sample_rate, int samples_per_block) {
        sample_rate_ = new_sample_rate;
        delay_line_.prepare({ sample_rate_, static_cast<juce::uint32>(samples_per_block), 2 });
    }

    /**
     * @brief Processes the audio buffer to apply the echo effect.
     * @param buffer The audio buffer to process.
     */
    void Process(juce::AudioBuffer<float>& buffer) {
        auto num_samples = buffer.getNumSamples();
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
            auto* channel_data = buffer.getWritePointer(channel);
            for (int i = 0; i < num_samples; ++i) {
                auto delayed_sample = delay_line_.popSample(channel);
                auto input_sample = channel_data[i];
                auto output_sample = input_sample + delayed_sample * feedback_;
                delay_line_.pushSample(channel, output_sample);
                channel_data[i] = output_sample;
            }
        }
    }

private:
    float delay_time_ = 0.5f;       ///< Echo delay time in seconds.
    float feedback_ = 0.5f;         ///< Echo feedback level (0.0 to 1.0).
    double sample_rate_ = 44100.0;  ///< Sample rate of the audio stream.
    juce::dsp::DelayLine<float> delay_line_{ 44100 }; ///< Delay line for storing past samples.
};



