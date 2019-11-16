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
    Key output[7];
    char sound[7];
};

static KeyTranslationEntry g_dutchClassic[] = {
    { Key::One, false, false, false, { Key::A, Key::A }, "aa.wav" },
    { Key::Two, false, false, false, { Key::U, Key::U }, "uu.wav" },
    { Key::Three, false, false, false, { Key::O, Key::O }, "oo.wav" },
    { Key::Four, false, false, false, { Key::E, Key::E }, "ee.wav" },
    { Key::Five, false, false, false, { Key::E, Key::U }, "eu.wav" },
    { Key::Six, false, false, false, { Key::A, Key::U }, "au.wav" },
    { Key::Seven, false, false, false, { Key::U, Key::I }, "ui.wav" },
    { Key::Eight, false, false, false, { Key::I, Key::E }, "ie.wav" },
    { Key::Nine, false, false, false, { Key::O, Key::E }, "oe.wav" },
    { Key::Zero, false, false, false, { Key::E, Key::I }, "ei.wav" },
};

static KeyTranslationEntry g_dutchKWeC[] = {
    { Key::One, false, false, false, { Key::A, Key::A }, "aa.wav" },
    { Key::Two, false, false, false, { Key::E, Key::E }, "ee.wav" },
    { Key::Three, false, false, false, { Key::O, Key::O }, "oo.wav" },
    { Key::Four, false, false, false, { Key::U, Key::U }, "uu.wav" },
    { Key::Five, false, false, false, { Key::A, Key::U }, "au.wav" },
    { Key::Six, false, false, false, { Key::E, Key::I }, "ei.wav" },
    { Key::Seven, false, false, false, { Key::E, Key::U }, "eu.wav" },
    { Key::Eight, false, false, false, { Key::I, Key::E }, "ie.wav" },
    { Key::Nine, false, false, false, { Key::I, Key::J }, "ij.wav" },
    { Key::Zero, false, false, false, { Key::O, Key::E }, "oe.wav" },
};

KeyTranslation MakeKeyTranslation(Key* keys, char* sound)
{
    KeyTranslation kt;
    kt.keys = keys;
    kt.sound = sound;
    return kt;
}

KeyTranslation FindTranslation(KeyTranslationEntry* pEntries, int nEntryCount, Key key, bool shift, bool ctrl, bool alt)
{
    for (int i = 0; i < nEntryCount; ++i)
    {
        KeyTranslationEntry entry = pEntries[i];
        if (entry.input == key && entry.shift == shift && entry.ctrl == ctrl && entry.alt == alt)
        {
            return MakeKeyTranslation(entry.output, entry.sound);
        }
    }

    return MakeKeyTranslation(nullptr, nullptr);
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
