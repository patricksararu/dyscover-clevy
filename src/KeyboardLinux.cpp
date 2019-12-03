//
// KeyboardLinux.cpp
//

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#include <sstream>
#include <vector>

#include "KeyboardLinux.h"

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(array, bit) ((array[LONG(bit)] >> OFF(bit)) & 1)

KeyboardLinux::KeyboardLinux() : wxThread(wxTHREAD_JOINABLE)
{
    wxLogDebug("KeyboardLinux::KeyboardLinux()");

    m_eventfd = eventfd(0, EFD_NONBLOCK);

    if (m_eventfd != -1)
    {
        Run();
    }
    else
    {
        // TODO: Report error.
    }
}

KeyboardLinux::~KeyboardLinux()
{
    wxLogDebug("KeyboardLinux::~KeyboardLinux()");

    // Send termination signal thru eventfd
    eventfd_t x = 1337;
    eventfd_write(m_eventfd, x);

    //printf("Killing thread ...\n");
    //Kill();
    //Delete();

    Wait();

    //printf("Closing device file ...\n");
    //sclose(m_eventfd);
}

int is_event_device(const struct dirent* de)
{
    return strncmp("event", de->d_name, 5) == 0;
}

int emit_event(int fd, int type, int code, int value)
{
    struct input_event ie;
    ie.type = type;
    ie.code = code;
    ie.value = value;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    ssize_t written = write(fd, &ie, sizeof(ie));
    return written > 0 ? 0 : errno;
}

void* KeyboardLinux::Entry()
{
    wxLogDebug("KeyboardLinux::Entry()");

#if 0
    int fd = open("/dev/input/event8", O_RDONLY);
    if (fd == -1)
    {
        // TODO: Report error.
        printf("Error opening file!\n");
        return nullptr;
    }
#endif

    int epollfd = epoll_create1(0);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = m_eventfd;
    int result = epoll_ctl(epollfd, EPOLL_CTL_ADD, m_eventfd, &ev);
    if (result != 0)
    {
        wxLogError("Error occured.\n");
        //close(fd);
        close(epollfd);
        return nullptr;
    }

    // Open /dev/uinput
    int uifd = open("/dev/uinput", O_RDWR);
    if (uifd == -1)
    {
        wxLogError("Error opening file /dev/uinput.");
        close(epollfd);
        return nullptr;
    }

    result = ioctl(uifd, UI_SET_EVBIT, EV_KEY);
    if (result != 0)
    {
        wxLogError("Error configuring /dev/uinput.");
        close(uifd);
        close(epollfd);
        return nullptr;
    }

    // TODO: Open all /dev/input/event* that support KEY_SPACE.
    struct dirent** entries;
    int entry_count = scandir("/dev/input", &entries, is_event_device, alphasort);
    std::vector<int> fds;
    for (int i = 0; i < entry_count; ++i)
    {
        char filename[267];
        snprintf(filename, 267, "/dev/input/%s", entries[i]->d_name);

        int fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            wxLogError("Error opening file %s.", filename);
            continue;
        }

        unsigned int events = 0;
        ioctl(fd, EVIOCGBIT(0, sizeof(events)), &events);

        if (events & BIT(EV_KEY))
        {
            // Check if device has KEY_SPACE.
            const size_t bits_count = NBITS(KEY_MAX);
            unsigned long bits[bits_count];
            ioctl(fd, EVIOCGBIT(EV_KEY, KEY_MAX), bits);
            int has_key_space = test_bit(bits, KEY_SPACE);
            if (has_key_space)
            {
                wxLogDebug("Adding %s (fd = %d)  Events mask = 0x%08x to epoll ...", filename, fd, events);

                ev.data.fd = fd;
                result = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
                if (result != 0)
                {
                    wxLogError("Error adding file %s to epoll.", filename);
                    continue;
                }

                fds.push_back(fd);

                for (int key = 0; key < KEY_MAX; ++key)
                {
                    if (test_bit(bits, key))
                    {
                        result = ioctl(uifd, UI_SET_KEYBIT, key);
                        if (result != 0)
                        {
                            wxLogError("Error enabling key %d '/dev/uinput' through ioctl(). %s", key, strerror(errno));
                        }
                    }
                }
            }
            else
            {
                close(fd);
            }
        }
        else
        {
            close(fd);
        }
    }

    struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;
    strcpy(usetup.name, "Clevy uinput loopback device");
    // TODO: Add error handling.
    ioctl(uifd, UI_DEV_SETUP, &usetup);
    ioctl(uifd, UI_DEV_CREATE);

#if 1
    for (int fd : fds)
    {
        int rc = ioctl(fd, EVIOCGRAB, 1);
        if (rc != 0)
        {
            wxLogError("Can't grab keyboard open as fd %d. %s", fd, strerror(errno));
        }
    }
#endif

    bool shift = false;
    bool ctrl = false;
    bool alt = false;

    while (!TestDestroy())
    {
        struct epoll_event event;
        int nfds = epoll_wait(epollfd, &event, 1, -1);
        if (nfds != 1)
        {
            wxLogDebug("Received %d event(s) from epoll", nfds);
        }

        int fd = event.data.fd;

        if (fd == m_eventfd)
        {
            eventfd_t x;
            eventfd_read(m_eventfd, &x);
            wxLogDebug("Received %ld from eventfd", x);
            break;
        }
        else  // if (event.data.fd == fd)
        {
            struct input_event ie;
            ssize_t bytesread = read(fd, &ie, sizeof(struct input_event));
            if (bytesread == -1)
            {
                wxLogError("read() failed");
                break;
            }

            wxLogDebug("Fd %d  Time %ld.%ld  Type %d  Code %d  Value %d", fd, ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code, ie.value);

            if (ie.type == EV_KEY && (ie.value == 0 || ie.value == 1))
            {
                if (ie.code == KEY_LEFTSHIFT || ie.code == KEY_RIGHTSHIFT)
                {
                    shift = ie.value;
                }
                else if (ie.code == KEY_LEFTCTRL || ie.code == KEY_RIGHTCTRL)
                {
                    ctrl = ie.value;
                }
                else if (ie.code == KEY_LEFTALT /*|| ie.code == KEY_RIGHTALT*/)
                {
                    alt = ie.value;
                }

                KeyEventType ket = ie.value ? KeyEventType::KeyDown : KeyEventType::KeyUp;
                //wxLogDebug("Key press detected on %d: %d %d", fd, ie.code, ie.value);
                InvokeCallback(ket, ie.code, ie.code, 0, ie.time.tv_sec);
            }

            // Do key translation
            if (ie.type == EV_KEY)
            {
                bool processed = false;

                Key key = KeyFromKeyCode(ie.code);
                if (key != Key::Unknown)
                {
                    KeyTranslation kt = TranslateKey(key, shift, ctrl, alt, Layout::DutchClassic);
                    if (!kt.keys.empty())
                    {
                        if (ie.value == 1 || ie.value == 2)
                        {
                            std::stringstream ss;
                            for (KeyStroke ks : kt.keys)
                            {
                                if (ks.shift)  ss << "Shift+";
                                if (ks.ctrl)  ss << "Ctrl+";
                                if (ks.alt)  ss << "Alt+";
                                ss << (int)ks.key << " ";
                            }
                            wxLogDebug("Translated key strokes: %s", ss.str());

                            for (KeyStroke ks : kt.keys)
                            {
                                if (shift && !ks.shift)
                                {
                                    // Release Shift
                                    emit_event(uifd, EV_KEY, KEY_LEFTSHIFT, 0);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }
                                else if (!shift && ks.shift)
                                {
                                    // Press Shift
                                    emit_event(uifd, EV_KEY, KEY_LEFTSHIFT, 1);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }

                                if (ctrl && !ks.ctrl)
                                {
                                    // Release Ctrl
                                    emit_event(uifd, EV_KEY, KEY_LEFTCTRL, 0);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }
                                else if (!ctrl && ks.ctrl)
                                {
                                    // Press Ctrl
                                    emit_event(uifd, EV_KEY, KEY_LEFTCTRL, 1);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }

                                if (alt && !ks.alt)
                                {
                                    // Release Alt
                                    emit_event(uifd, EV_KEY, KEY_LEFTALT, 0);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }
                                else if (!alt && ks.alt)
                                {
                                    // Press Alt
                                    emit_event(uifd, EV_KEY, KEY_LEFTALT, 1);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }

                                int keycode = KeyCodeFromKey(ks.key);

                                emit_event(uifd, EV_KEY, keycode, 1);
                                emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                emit_event(uifd, EV_KEY, keycode, 0);
                                emit_event(uifd, EV_SYN, SYN_REPORT, 0);

                                if (shift && !ks.shift)
                                {
                                    // Re-press Shift
                                    emit_event(uifd, EV_KEY, KEY_LEFTSHIFT, 1);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }
                                else if (!shift && ks.shift)
                                {
                                    // Release Shift
                                    emit_event(uifd, EV_KEY, KEY_LEFTSHIFT, 0);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }

                                if (ctrl && !ks.ctrl)
                                {
                                    // Re-press Ctrl
                                    emit_event(uifd, EV_KEY, KEY_LEFTCTRL, 1);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }
                                else if (!ctrl && ks.ctrl)
                                {
                                    // Release Ctrl
                                    emit_event(uifd, EV_KEY, KEY_LEFTCTRL, 0);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }

                                if (alt && !ks.alt)
                                {
                                    // Re-press Alt
                                    emit_event(uifd, EV_KEY, KEY_LEFTALT, 1);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }
                                else if (!alt && ks.alt)
                                {
                                    // Release Alt
                                    emit_event(uifd, EV_KEY, KEY_LEFTALT, 0);
                                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                                }
                            }
                        }

                        processed = true;
                    }
                }

                if (!processed)
                {
                    emit_event(uifd, EV_KEY, ie.code, ie.value);
                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                }
            }

#if 0
            if (ie.type == EV_KEY)
            {
                if (ie.code == KEY_1)
                {
                    if (ie.value == 1)
                    {
                        emit_event(uifd, EV_KEY, KEY_A, 1);
                        emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                        emit_event(uifd, EV_KEY, KEY_A, 0);
                        emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                        emit_event(uifd, EV_KEY, KEY_A, 1);
                        emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                    }
                    else if (ie.value == 2)
                    {
                        emit_event(uifd, EV_KEY, KEY_A, 2);
                        emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                    }
                    else if (ie.value == 0)
                    {
                        emit_event(uifd, EV_KEY, KEY_A, 0);
                        emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                    }
                }
                else
                {
                    emit_event(uifd, EV_KEY, ie.code, ie.value);
                    emit_event(uifd, EV_SYN, SYN_REPORT, 0);
                }
            }

            if (ie.type == EV_KEY && )
            {
            }
            else
            {
                size_t written = write(uifd, &ie, sizeof(ie));
                if (written <= 0)
                {
                    wxLogError("write() failed");
                    break;
                }
            }
#endif
        }
    }

    // TODO: Close all open device files.
    for (int fd : fds)
    {
        wxLogDebug("Closing %d", fd);

        ioctl(fd, EVIOCGRAB, 0);

        epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);

        close(fd);
    }

    //close(fd);

    ioctl(uifd, UI_DEV_DESTROY);

    close(uifd);

    close(epollfd);

    return nullptr;
}

static constexpr KeyMapping s_keyMappings[] = {
    { Key::Esc, KEY_ESC },
    { Key::One, KEY_1 },
    { Key::Two, KEY_2 },
    { Key::Three, KEY_3 },
    { Key::Four, KEY_4 },
    { Key::Five, KEY_5 },
    { Key::Six, KEY_6 },
    { Key::Seven, KEY_7 },
    { Key::Eight, KEY_8 },
    { Key::Nine, KEY_9 },
    { Key::Zero, KEY_0 },
    { Key::Minus, KEY_MINUS },
    { Key::Equal, KEY_EQUAL },
    { Key::Backspace, KEY_BACKSPACE },
    { Key::Tab, KEY_TAB },
    { Key::Q, KEY_Q },
    { Key::W, KEY_W },
    { Key::E, KEY_E },
    { Key::R, KEY_R },
    { Key::T, KEY_T },
    { Key::Y, KEY_Y },
    { Key::U, KEY_U },
    { Key::I, KEY_I },
    { Key::O, KEY_O },
    { Key::P, KEY_P },
    { Key::OpenBracket, KEY_LEFTBRACE },
    { Key::CloseBracket, KEY_RIGHTBRACE },
    { Key::Enter, KEY_ENTER },
    { Key::Ctrl, KEY_LEFTCTRL },
    { Key::A, KEY_A },
    { Key::S, KEY_S },
    { Key::D, KEY_D },
    { Key::F, KEY_F },
    { Key::G, KEY_G },
    { Key::H, KEY_H },
    { Key::J, KEY_J },
    { Key::K, KEY_K },
    { Key::L, KEY_L },
    { Key::Semicolon, KEY_SEMICOLON },
    { Key::Apostrophe, KEY_APOSTROPHE },
    { Key::Shift, KEY_LEFTSHIFT },
    { Key::Backslash, KEY_BACKSLASH },
    { Key::Z, KEY_Z },
    { Key::X, KEY_X },
    { Key::C, KEY_C },
    { Key::V, KEY_V },
    { Key::B, KEY_B },
    { Key::N, KEY_N },
    { Key::M, KEY_M },
    { Key::Comma, KEY_COMMA },
    { Key::Dot, KEY_DOT },
    { Key::Slash, KEY_SLASH },
    { Key::Shift, KEY_RIGHTSHIFT },
    { Key::Alt, KEY_LEFTALT },
    { Key::Space, KEY_SPACE },
    { Key::CapsLock, KEY_CAPSLOCK },
    { Key::AltGr, KEY_RIGHTALT },
    { Key::Ins, KEY_INSERT },
    { Key::Del, KEY_DELETE },
    { Key::Home, KEY_HOME },
    { Key::End, KEY_END },
    { Key::PageUp, KEY_PAGEUP },
    { Key::PageDown, KEY_PAGEDOWN },
};

Key KeyboardLinux::KeyFromKeyCode(int keyCode)
{
    for (KeyMapping mapping : s_keyMappings)
    {
        if (mapping.code == keyCode)
        {
            return mapping.key;
        }
    }
    return Key::Unknown;
}

int KeyboardLinux::KeyCodeFromKey(Key key)
{
    for (KeyMapping mapping : s_keyMappings)
    {
        if (mapping.key == key)
        {
            return mapping.code;
        }
    }
    return -1;
}
