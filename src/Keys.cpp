//
// Keys.cpp
//

#include "Keys.h"

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

static const std::vector<KeyTranslationEntry> g_dutchClassic = {
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
    { Key::AltGr, false, true, false, { { Key::E }, { Key::E }, { Key::R } }, "eer.wav" },  // In non-US keyboard layouts, Windows translates AltGr to LeftCtrl+AltGr
    { Key::OpenBracket, false, false, false, { { Key::O }, { Key::U } }, "ou.wav" },
    { Key::CloseBracket, false, false, false, { { Key::I }, { Key::J } }, "ij.wav" },
    { Key::OpenBracket, false, true, false, { { Key::OpenBracket, false, false, false } } },
    { Key::CloseBracket, false, true, false, { { Key::I }, { Key::J } }, "u.wav" },  // Not a mistake: Ctrl+] should give 'ij' with sound 'u'
    { Key::Semicolon, false, false, false, { { Key::N }, { Key::G } }, "ng.wav" },
    { Key::Semicolon, false, true, false, { { Key::Semicolon, false, false, false } } },
    { Key::Apostrophe, false, false, false, { { Key::N }, { Key::K } }, "nk.wav" },
    { Key::Apostrophe, false, true, false, { { Key::Apostrophe, false, false, false } } },
    { Key::Backslash, false, false, false, { { Key::C }, { Key::H } }, "ch.wav" },
    { Key::Backslash, false, true, false, { { Key::C }, { Key::H } }, "sj.wav" },  // Not a mistake: Ctrl+\ should give 'ch' with sound 'sj'
    { Key::Minus, false, false, false, { { Key::E }, { Key::U }, { Key::R } }, "eur.wav" },
    { Key::Minus, false, true, false, { { Key::Minus, false, false, false } } },
    { Key::Slash, false, false, false, { { Key::O }, { Key::O }, { Key::R } }, "oor.wav" },
    { Key::Slash, false, true, false, { { Key::Slash, false, false, false } } },
    { Key::Equal, false, false, false, { { Key::S }, { Key::C }, { Key::H } }, "sch.wav" },
    { Key::Equal, false, true, false, { { Key::Equal, false, false, false } } },
    { Key::Ins, false, false, false, { { Key::A }, { Key::A }, { Key::I } }, "aai.wav" },
    { Key::Ins, false, true, false, { { Key::Ins, false, false, false } } },
    { Key::Del, false, false, false, { { Key::E }, { Key::E }, { Key::U }, { Key::W } }, "eeuw.wav" },
    { Key::Del, false, true, false, { { Key::Del, false, false, false } } },
    { Key::Home, false, false, false, { { Key::O }, { Key::O }, { Key::I } }, "ooi.wav" },
    { Key::Home, false, true, false, { { Key::Home, false, false, false } } },
    { Key::End, false, false, false, { { Key::I }, { Key::E }, { Key::U }, { Key::W } }, "ieuw.wav" },
    { Key::End, false, true, false, { { Key::End, false, false, false } } },
    { Key::PageUp, false, false, false, { { Key::O }, { Key::E }, { Key::I } }, "oei.wav" },
    { Key::PageUp, false, true, false, { { Key::PageUp, false, false, false } } },
    { Key::PageDown, false, false, false, { { Key::U }, { Key::W } }, "uw.wav" },
    { Key::PageDown, false, true, false, { { Key::PageDown, false, false, false } } },
    { Key::A, false, false, false, { { Key::A, false, false, false } }, "a.wav" },
    { Key::A, true, false, false, { { Key::A, true, false, false } }, "a.wav" },
    { Key::A, true, false, true, { { Key::A }, { Key::A } }, "aa.wav" },
    { Key::B, false, false, false, { { Key::B, false, false, false } }, "b.wav" },
    { Key::B, true, false, false, { { Key::B, true, false, false } }, "b.wav" },
    { Key::C, false, false, false, { { Key::C, false, false, false } }, "c.wav" },
    { Key::C, true, false, false, { { Key::C, true, false, false } }, "c.wav" },
    { Key::C, false, true, false, { { Key::C } }, "k.wav" },  // Not a mistake: Ctrl+C should give 'c' with sound 'k'
    { Key::D, false, false, false, { { Key::D, false, false, false } }, "d.wav" },
    { Key::D, true, false, false, { { Key::D, true, false, false } }, "d.wav" },
    { Key::D, false, true, false, { { Key::D } }, "t.wav" },  // Not a mistake: Ctrl+D should give 'd' with sound 't'
    { Key::E, false, false, false, { { Key::E, false, false, false } }, "e.wav" },
    { Key::E, true, false, false, { { Key::E, true, false, false } }, "e.wav" },
    { Key::E, true, false, true, { { Key::E }, { Key::E } }, "ee.wav" },
    { Key::E, false, true, false, { { Key::E } }, "u.wav" },  // Not a mistake: Ctrl+E should give 'e' with sound 'u'
    { Key::F, false, false, false, { { Key::F, false, false, false } }, "f.wav" },
    { Key::F, true, false, false, { { Key::F, true, false, false } }, "f.wav" },
    { Key::G, false, false, false, { { Key::G, false, false, false } }, "g.wav" },
    { Key::G, true, false, false, { { Key::G, true, false, false } }, "g.wav" },
    { Key::H, false, false, false, { { Key::H, false, false, false } }, "h.wav" },
    { Key::H, true, false, false, { { Key::H, true, false, false } }, "h.wav" },
    { Key::I, false, false, false, { { Key::I, false, false, false } }, "i.wav" },
    { Key::I, true, false, false, { { Key::I, true, false, false } }, "i.wav" },
    { Key::I, true, false, true, { { Key::I }, { Key::E } }, "ie.wav" },
    { Key::I, false, true, false, { { Key::I } }, "u.wav" },  // Not a mistake: Ctrl+I should give 'i' with sound 'u'
    { Key::J, false, false, false, { { Key::J, false, false, false } }, "j.wav" },
    { Key::J, true, false, false, { { Key::J, true, false, false } }, "j.wav" },
    { Key::K, false, false, false, { { Key::K, false, false, false } }, "k.wav" },
    { Key::K, true, false, false, { { Key::K, true, false, false } }, "k.wav" },
    { Key::L, false, false, false, { { Key::L, false, false, false } }, "l.wav" },
    { Key::L, true, false, false, { { Key::L, true, false, false } }, "l.wav" },
    { Key::M, false, false, false, { { Key::M, false, false, false } }, "m.wav" },
    { Key::M, true, false, false, { { Key::M, true, false, false } }, "m.wav" },
    { Key::N, false, false, false, { { Key::N, false, false, false } }, "n.wav" },
    { Key::N, true, false, false, { { Key::N, true, false, false } }, "n.wav" },
    { Key::O, false, false, false, { { Key::O, false, false, false } }, "o.wav" },
    { Key::O, true, false, false, { { Key::O, true, false, false } }, "o.wav" },
    { Key::O, true, false, true, { { Key::O }, { Key::O } }, "oo.wav" },
    { Key::P, false, false, false, { { Key::P, false, false, false } }, "p.wav" },
    { Key::P, true, false, false, { { Key::P, true, false, false } }, "p.wav" },
    { Key::Q, false, false, false, { { Key::Q, false, false, false } }, "q.wav" },
    { Key::Q, true, false, false, { { Key::Q, true, false, false } }, "q.wav" },
    { Key::R, false, false, false, { { Key::R, false, false, false } }, "r.wav" },
    { Key::R, true, false, false, { { Key::R, true, false, false } }, "r.wav" },
    { Key::S, false, false, false, { { Key::S, false, false, false } }, "s.wav" },
    { Key::S, true, false, false, { { Key::S, true, false, false } }, "s.wav" },
    { Key::T, false, false, false, { { Key::T, false, false, false } }, "t.wav" },
    { Key::T, true, false, false, { { Key::T, true, false, false } }, "t.wav" },
    { Key::U, false, false, false, { { Key::U, false, false, false } }, "u.wav" },
    { Key::U, true, false, false, { { Key::U, true, false, false } }, "u.wav" },
    { Key::U, true, false, true, { { Key::U }, { Key::U } }, "uu.wav" },
    { Key::V, false, false, false, { { Key::V, false, false, false } }, "v.wav" },
    { Key::V, true, false, false, { { Key::V, true, false, false } }, "v.wav" },
    { Key::W, false, false, false, { { Key::W, false, false, false } }, "w.wav" },
    { Key::W, true, false, false, { { Key::W, true, false, false } }, "w.wav" },
    { Key::X, false, false, false, { { Key::X, false, false, false } }, "x.wav" },
    { Key::X, true, false, false, { { Key::X, true, false, false } }, "x.wav" },
    { Key::Y, false, false, false, { { Key::Y, false, false, false } }, "y.wav" },
    { Key::Y, true, false, false, { { Key::Y, true, false, false } }, "y.wav" },
    { Key::Y, false, true, false, { { Key::Y } }, "i.wav" },  // Not a mistake: Ctrl+Y should give 'y' with sound 'i'
    { Key::Z, false, false, false, { { Key::Z, false, false, false } }, "z.wav" },
    { Key::Z, true, false, false, { { Key::Z, true, false, false } }, "z.wav" },
};

static const std::vector<KeyTranslationEntry> g_dutchKWeC = {
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
    { Key::AltGr, false, true, false, { { Key::E }, { Key::E }, { Key::R } }, "eer.wav" },  // In non-US keyboard layouts, Windows translates AltGr to LeftCtrl+AltGr
    { Key::OpenBracket, false, false, false, { { Key::O }, { Key::U } }, "ou.wav" },
    { Key::CloseBracket, false, false, false, { { Key::U }, { Key::I } }, "ui.wav" },
    { Key::OpenBracket, false, true, false, { { Key::OpenBracket, false, false, false } } },
    { Key::CloseBracket, false, true, false, { { Key::CloseBracket, false, false, false } } },
    { Key::Semicolon, false, false, false, { { Key::N }, { Key::G } }, "ng.wav" },
    { Key::Semicolon, false, true, false, { { Key::Semicolon, false, false, false } } },
    { Key::Apostrophe, false, false, false, { { Key::N }, { Key::K } }, "nk.wav" },
    { Key::Apostrophe, false, true, false, { { Key::Apostrophe, false, false, false } } },
    { Key::Backslash, false, false, false, { { Key::C }, { Key::H } }, "ch.wav" },
    { Key::Backslash, false, true, false, { { Key::S }, { Key::J } }, "sj.wav" },
    { Key::Minus, false, false, false, { { Key::E }, { Key::U }, { Key::R } }, "eur.wav" },
    { Key::Minus, false, true, false, { { Key::Minus, false, false, false } } },
    { Key::Slash, false, false, false, { { Key::O }, { Key::O }, { Key::R } }, "oor.wav" },
    { Key::Slash, false, true, false, { { Key::Slash, false, false, false } } },
    { Key::Equal, false, false, false, { { Key::S }, { Key::C }, { Key::H } }, "sch.wav" },
    { Key::Equal, false, true, false, { { Key::Equal, false, false, false } } },
    { Key::Ins, false, false, false, { { Key::A }, { Key::A }, { Key::I } }, "aai.wav" },
    { Key::Ins, false, true, false, { { Key::Ins, false, false, false } } },
    { Key::Del, false, false, false, { { Key::E }, { Key::E }, { Key::U }, { Key::W } }, "eeuw.wav" },
    { Key::Del, false, true, false, { { Key::Del, false, false, false } } },
    { Key::Home, false, false, false, { { Key::O }, { Key::O }, { Key::I } }, "ooi.wav" },
    { Key::Home, false, true, false, { { Key::Home, false, false, false } } },
    { Key::End, false, false, false, { { Key::I }, { Key::E }, { Key::U }, { Key::W } }, "ieuw.wav" },
    { Key::End, false, true, false, { { Key::End, false, false, false } } },
    { Key::PageUp, false, false, false, { { Key::O }, { Key::E }, { Key::I } }, "oei.wav" },
    { Key::PageUp, false, true, false, { { Key::PageUp, false, false, false } } },
    { Key::PageDown, false, false, false, { { Key::U }, { Key::W } }, "uw.wav" },
    { Key::PageDown, false, true, false, { { Key::PageDown, false, false, false } } },
    { Key::A, false, false, false, { { Key::A, false, false, false } }, "a.wav" },
    { Key::A, true, false, false, { { Key::A, true, false, false } }, "a.wav" },
    { Key::B, false, false, false, { { Key::B, false, false, false } }, "b.wav" },
    { Key::B, true, false, false, { { Key::B, true, false, false } }, "b.wav" },
    { Key::C, false, false, false, { { Key::C, false, false, false } }, "c.wav" },
    { Key::C, true, false, false, { { Key::C, true, false, false } }, "c.wav" },
    { Key::D, false, false, false, { { Key::D, false, false, false } }, "d.wav" },
    { Key::D, true, false, false, { { Key::D, true, false, false } }, "d.wav" },
    { Key::E, false, false, false, { { Key::E, false, false, false } }, "e.wav" },
    { Key::E, true, false, false, { { Key::E, true, false, false } }, "e.wav" },
    { Key::F, false, false, false, { { Key::F, false, false, false } }, "f.wav" },
    { Key::F, true, false, false, { { Key::F, true, false, false } }, "f.wav" },
    { Key::G, false, false, false, { { Key::G, false, false, false } }, "g.wav" },
    { Key::G, true, false, false, { { Key::G, true, false, false } }, "g.wav" },
    { Key::H, false, false, false, { { Key::H, false, false, false } }, "h.wav" },
    { Key::H, true, false, false, { { Key::H, true, false, false } }, "h.wav" },
    { Key::I, false, false, false, { { Key::I, false, false, false } }, "i.wav" },
    { Key::I, true, false, false, { { Key::I, true, false, false } }, "i.wav" },
    { Key::J, false, false, false, { { Key::J, false, false, false } }, "j.wav" },
    { Key::J, true, false, false, { { Key::J, true, false, false } }, "j.wav" },
    { Key::K, false, false, false, { { Key::K, false, false, false } }, "k.wav" },
    { Key::K, true, false, false, { { Key::K, true, false, false } }, "k.wav" },
    { Key::L, false, false, false, { { Key::L, false, false, false } }, "l.wav" },
    { Key::L, true, false, false, { { Key::L, true, false, false } }, "l.wav" },
    { Key::M, false, false, false, { { Key::M, false, false, false } }, "m.wav" },
    { Key::M, true, false, false, { { Key::M, true, false, false } }, "m.wav" },
    { Key::N, false, false, false, { { Key::N, false, false, false } }, "n.wav" },
    { Key::N, true, false, false, { { Key::N, true, false, false } }, "n.wav" },
    { Key::O, false, false, false, { { Key::O, false, false, false } }, "o.wav" },
    { Key::O, true, false, false, { { Key::O, true, false, false } }, "o.wav" },
    { Key::P, false, false, false, { { Key::P, false, false, false } }, "p.wav" },
    { Key::P, true, false, false, { { Key::P, true, false, false } }, "p.wav" },
    { Key::Q, false, false, false, { { Key::Q, false, false, false } }, "q.wav" },
    { Key::Q, true, false, false, { { Key::Q, true, false, false } }, "q.wav" },
    { Key::R, false, false, false, { { Key::R, false, false, false } }, "r.wav" },
    { Key::R, true, false, false, { { Key::R, true, false, false } }, "r.wav" },
    { Key::S, false, false, false, { { Key::S, false, false, false } }, "s.wav" },
    { Key::S, true, false, false, { { Key::S, true, false, false } }, "s.wav" },
    { Key::T, false, false, false, { { Key::T, false, false, false } }, "t.wav" },
    { Key::T, true, false, false, { { Key::T, true, false, false } }, "t.wav" },
    { Key::U, false, false, false, { { Key::U, false, false, false } }, "u.wav" },
    { Key::U, true, false, false, { { Key::U, true, false, false } }, "u.wav" },
    { Key::V, false, false, false, { { Key::V, false, false, false } }, "v.wav" },
    { Key::V, true, false, false, { { Key::V, true, false, false } }, "v.wav" },
    { Key::W, false, false, false, { { Key::W, false, false, false } }, "w.wav" },
    { Key::W, true, false, false, { { Key::W, true, false, false } }, "w.wav" },
    { Key::X, false, false, false, { { Key::X, false, false, false } }, "x.wav" },
    { Key::X, true, false, false, { { Key::X, true, false, false } }, "x.wav" },
    { Key::Y, false, false, false, { { Key::Y, false, false, false } }, "y.wav" },
    { Key::Y, true, false, false, { { Key::Y, true, false, false } }, "y.wav" },
    { Key::Z, false, false, false, { { Key::Z, false, false, false } }, "z.wav" },
    { Key::Z, true, false, false, { { Key::Z, true, false, false } }, "z.wav" },
};

KeyTranslation FindTranslation(const std::vector<KeyTranslationEntry>& entries, Key key, bool shift, bool ctrl, bool alt)
{
    for (const KeyTranslationEntry& entry : entries)
    {
        if (entry.input == key && entry.shift == shift && entry.ctrl == ctrl && entry.alt == alt)
        {
            KeyTranslation kt;
            kt.keystrokes = entry.output;
            kt.sound = entry.sound;
            return kt;
        }
    }

    return KeyTranslation();
}

KeyTranslation TranslateKey(Key key, bool shift, bool ctrl, bool alt, Layout layout)
{
    switch (layout)
    {
    case Layout::DutchClassic:
        return FindTranslation(g_dutchClassic, key, shift, ctrl, alt);
    case Layout::DutchKWeC:
        return FindTranslation(g_dutchKWeC, key, shift, ctrl, alt);
    default:
        return KeyTranslation();
    }
}
