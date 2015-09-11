#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#ifdef __BIONIC__
#include "linux/uinput.h"
#else
#include <linux/uinput.h>
#endif
#include "UInput.h"

#define KEYMASK 0xfff
#define SHIFT 0x10000
#define MAX_KEYCODE 248 //from input.h
#define MIN_KEYCODE 0   //from input.h

UInputKeyboard::UInputKeyboard()
    : mFd(-1)
{
    if (mFd < 0) {
        init_dev();
    }
}

UInputKeyboard::~UInputKeyboard()
{
    if (mFd > 0) {
        deinit_dev();
    }
}

void UInputKeyboard::check_posix(intmax_t rc, const char *fmt, ...)
{
    if (rc < 0) {
        va_list args;
        va_start(args, fmt);
        verr(EXIT_FAILURE, fmt, args);
        va_end(args);
    }
}

int UInputKeyboard::ev_emit(int type, int code, int value)
{
    struct input_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = type;
    ev.code = code;
    ev.value = value;

    gettimeofday(&ev.time, 0);
    return write(mFd, &ev, sizeof(ev));
}

int UInputKeyboard::ev_syn()
{
    return ev_emit(EV_SYN, SYN_REPORT, 0);
}

int UInputKeyboard::ev_key(int key, int value)
{
    int ret;

    if (value && (SHIFT & key)) {
        if (ev_key(KEY_LEFTSHIFT, 1) < 0)
            return -1;
    }

    ret = ev_emit(EV_KEY, key & KEYMASK, value);
    if (ev_syn() < 0)
        return -1;

    if (!value && (SHIFT & key)) {
        if (ev_key(KEY_LEFTSHIFT, 0) < 0)
            return -1;
    }

    return ret;
}

int UInputKeyboard::ev_key_click(int key)
{
    if (ev_key(key, 1) < 0)
        return -1;
    return ev_key(key, 0);
}

void UInputKeyboard::init_dev()
{
    if (mFd >= 0)
        return;

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    check_posix(fd, "failed to open uinput");

    check_posix(ioctl(fd, UI_SET_EVBIT, EV_KEY), "failed to set EV_KEY");
    check_posix(ioctl(fd, UI_SET_KEYBIT, KEY_LEFTSHIFT), "failed to set UI_SET_KEYBIT");

    for (int i = MIN_KEYCODE; i <= MAX_KEYCODE; i++) {
        check_posix(ioctl(fd, UI_SET_KEYBIT, i & KEYMASK),
                    "failed to set UI_SET_KEYBIT");
    }


    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof (uidev));
    uidev.id.bustype = BUS_VIRTUAL;
    uidev.id.vendor = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    printf("uinput_user_dev.ff_effects_max: %d\n", uidev.ff_effects_max);
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Virtual Injector Keyboard");
    check_posix(write(fd, &uidev, sizeof(uidev)),
                "failed to write uinput_user_dev struct");
    check_posix(ioctl(fd, UI_DEV_CREATE),
                "failed to create uinput device");

    mFd = fd;
}

void UInputKeyboard::deinit_dev()
{
    ioctl(mFd, UI_DEV_DESTROY);
    close(mFd);
}

