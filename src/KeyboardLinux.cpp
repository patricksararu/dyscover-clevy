//
// KeyboardLinux.cpp
//

#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

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

    for (int fd : fds)
    {
        ioctl(fd, EVIOCGRAB, 1);
    }

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

            if (ie.type == EV_KEY && (ie.value == 0 || ie.value == 1))
            {
                KeyEventType ket = ie.value ? KeyEventType::KeyDown : KeyEventType::KeyUp;
                wxLogDebug("Key press detected on %d: %d %d", fd, ie.code, ie.value);
                InvokeCallback(ket, ie.code, ie.code, 0, ie.time.tv_sec);
            }
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
