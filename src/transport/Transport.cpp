#include "Transport.h"

Transport::Transport(juce::AudioDeviceManager& dm)
    : deviceManager(dm)
{
    startTimerHz(30);
}

Transport::~Transport()
{
    stopTimer();
}

void Transport::play()
{
    if (!playing)
    {
        playing = true;
        changeListeners.sendChangeMessage();
    }
}

void Transport::stop()
{
    if (playing)
    {
        playing = false;
        changeListeners.sendChangeMessage();
    }
}

void Transport::togglePlayStop()
{
    if (playing)
        stop();
    else
        play();
}

void Transport::timerCallback()
{
    if (playing)
    {
        double now = juce::Time::getMillisecondCounterHiRes() * 0.001;
        if (lastUpdateTime > 0.0)
        {
            double delta = now - lastUpdateTime;
            currentPosition += delta;
        }
        lastUpdateTime = now;
    }
    else
    {
        lastUpdateTime = 0.0;
    }
}
