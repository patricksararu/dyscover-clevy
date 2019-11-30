//
// Keys.cpp
//

#include "Keys.h"

#define ARRAY_LENGTH(a)  (sizeof(a) / sizeof(a[0]))

struct KeyTranslationEntry
{
    // Inputs
    Key input;
    bool shift;
    bool ctrl;
    bool alt;

    // Outputs
    std::vector<KeyStroke> output;
    std::string sound;
};

static KeyTranslationEntry g_dutchClassic[] = {
    { Key::One, false, false, false, { { Key::A }, { Key::A } }, "aa.wav" },
    { Key::Two, false, false, false, { { Key::U }, { Key::U } }, "uu.wav" },
    { Key::Three, false, false, false, { { Key::O }, { Key::O } }, "oo.wav" },
    { Key::Four, false, false, false, { { Key::E }, { Key::E } }, "ee.wav" },
    { Key::Five, false, false, false, { { Key::E }, { Key::U } }, "eu.wav" },
    { Key::Six, false, false, false, { { Key::A }, { Key::U } }, "au.wav" },
    { Key::Seven, false, false, false, { { Key::U }, { Key::I } }, "ui.wav" },
    { Key::Eight, false, false, false, { { Key::I }, { Key::E } }, "ie.wav" },
    { Key::Nine, false, false, false, { { Key::O }, { Key::E } }, "oe.wav" },
    { Key::Zero, false, false, false, { { Key::E }, { Key::I } }, "ei.wav" },
    { Key::One, false, true, false, { { Key::One, false, false, false } }, "1.wav" },
    { Key::Two, false, true, false, { { Key::Two, false, false, false } }, "2.wav" },
    { Key::Three, false, true, false, { { Key::Three, false, false, false } }, "3.wav" },
    { Key::Four, false, true, false, { { Key::Four, false, false, false } }, "4.wav" },
    { Key::Five, false, true, false, { { Key::Five, false, false, false } }, "5.wav" },
    { Key::Six, false, true, false, { { Key::Six, false, false, false } }, "6.wav" },
    { Key::Seven, false, true, false, { { Key::Seven, false, false, false } }, "7.wav" },
    { Key::Eight, false, true, false, { { Key::Eight, false, false, false } }, "8.wav" },
    { Key::Nine, false, true, false, { { Key::Nine, false, false, false } }, "9.wav" },
    { Key::Zero, false, true, false, { { Key::Zero, false, false, false } }, "0.wav" },
    { Key::Five, false, true, true, { { Key::E, false, true, true } } },
    { Key::OpenBracket, false, false, false, { { Key::O }, { Key::U } }, "ou.wav" },
    { Key::CloseBracket, false, false, false, { { Key::I }, { Key::J } }, "ij.wav" },
};

static KeyTranslationEntry g_dutchKWeC[] = {
    { Key::One, false, false, false, { { Key::A }, { Key::A } }, "aa.wav" },
    { Key::Two, false, false, false, { { Key::E }, { Key::E } }, "ee.wav" },
    { Key::Three, false, false, false, { { Key::O }, { Key::O } }, "oo.wav" },
    { Key::Four, false, false, false, { { Key::U }, { Key::U } }, "uu.wav" },
    { Key::Five, false, false, false, { { Key::A }, { Key::U } }, "au.wav" },
    { Key::Six, false, false, false, { { Key::E }, { Key::I } }, "ei.wav" },
    { Key::Seven, false, false, false, { { Key::E }, { Key::U } }, "eu.wav" },
    { Key::Eight, false, false, false, { { Key::I }, { Key::E } }, "ie.wav" },
    { Key::Nine, false, false, false, { { Key::I }, { Key::J } }, "ij.wav" },
    { Key::Zero, false, false, false, { { Key::O }, { Key::E } }, "oe.wav" },
    { Key::One, false, true, false, { { Key::One, false, false, false } }, "1.wav" },
    { Key::Two, false, true, false, { { Key::Two, false, false, false } }, "2.wav" },
    { Key::Three, false, true, false, { { Key::Three, false, false, false } }, "3.wav" },
    { Key::Four, false, true, false, { { Key::Four, false, false, false } }, "4.wav" },
    { Key::Five, false, true, false, { { Key::Five, false, false, false } }, "5.wav" },
    { Key::Six, false, true, false, { { Key::Six, false, false, false } }, "6.wav" },
    { Key::Seven, false, true, false, { { Key::Seven, false, false, false } }, "7.wav" },
    { Key::Eight, false, true, false, { { Key::Eight, false, false, false } }, "8.wav" },
    { Key::Nine, false, true, false, { { Key::Nine, false, false, false } }, "9.wav" },
    { Key::Zero, false, true, false, { { Key::Zero, false, false, false } }, "0.wav" },
    { Key::Five, false, true, true, { { Key::E, false, true, true } } },
    { Key::OpenBracket, false, false, false, { { Key::O }, { Key::U } }, "ou.wav" },
    { Key::CloseBracket, false, false, false, { { Key::U }, { Key::I } }, "ui.wav" },
};

/*
KeyTranslation MakeKeyTranslation(std::vector<Key> keys, std::string sound)
{
    KeyTranslation kt;
    kt.keys = keys;
    kt.sound = sound;
    return kt;
}
*/

KeyTranslation FindTranslation(KeyTranslationEntry* pEntries, int nEntryCount, Key key, bool shift, bool ctrl, bool alt)
{
    for (int i = 0; i < nEntryCount; ++i)
    {
        KeyTranslationEntry entry = pEntries[i];
        if (entry.input == key && entry.shift == shift && entry.ctrl == ctrl && entry.alt == alt)
        {
            //return MakeKeyTranslation(entry.output, entry.sound);
            KeyTranslation kt;
            kt.keys = entry.output;
            kt.sound = entry.sound;
            return kt;
        }
    }

    return KeyTranslation();
}

KeyTranslation TranslateKey(Key key, bool shift, bool ctrl, bool alt, Layout layout)
{
    KeyTranslationEntry* pEntries = nullptr;
    int nNumEntries = 0;

    switch (layout)
    {
    case Layout::DutchClassic:
        pEntries = g_dutchClassic;
        nNumEntries = ARRAY_LENGTH(g_dutchClassic);
        break;
    case Layout::DutchKWeC:
        pEntries = g_dutchKWeC;
        nNumEntries = ARRAY_LENGTH(g_dutchKWeC);
        break;
    }

    return FindTranslation(pEntries, nNumEntries, key, shift, ctrl, alt);
}
