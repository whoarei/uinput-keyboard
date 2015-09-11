#ifndef __KEYBOARD_H
#define __KEYBOARD_H

class UInputKeyboard {
public:
    UInputKeyboard();
    virtual ~UInputKeyboard();
    int ev_syn();
    int ev_key(int key, int value);
    int ev_key_click(int key);
private:
    virtual void init_dev();
    virtual void deinit_dev();
    void check_posix(intmax_t rc, const char *fmt, ...);
    int ev_emit(int type, int code, int value);

private:
    int mFd;//uinput fd
};

#endif  //__KEYBOARD_H

