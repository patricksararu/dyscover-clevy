//
// Config.h
//

#pragma once

enum class Layout
{
    DutchClassic,
    DutchKWeC,
};

class Config
{
public:
    bool sound;
    bool sounds;
    bool tts;
    int volume;
    bool word;
    bool selection;
    bool readalong;
    int speed;
    bool startwithsystem;
    bool paused;
    Layout layout;
};
