#include "ICommand.h"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <sys/select.h>
#include "UInput.h"

using namespace std;
using namespace tinyxml2;

class KeyCommand : public ICommand {
public:
    KeyCommand(const char *name, int code, int value)
        : ICommand("key")
        , mCode(code)
        , mValue(value)
        , mName(name)
    {
        uinput();
    }
    UInputKeyboard *uinput() {
        if (!mKeyboard) {
            mKeyboard = new UInputKeyboard;
        }

        return mKeyboard;
    }

protected:
    int mCode;
    int mValue;
    string mName;
private:
    static UInputKeyboard *mKeyboard;
};
UInputKeyboard *KeyCommand::mKeyboard = 0;

class KeyDownCommand : public KeyCommand {
public:
    KeyDownCommand(const char *name, int code)
        : KeyCommand(name, code, 1)
    { }
    virtual int action(unsigned long data = 0);
    void dump(){
    }
};

class KeyUpCommand : public KeyCommand {
public:
    KeyUpCommand(const char *name, int code)
        : KeyCommand(name, code, 0)
    { }
    virtual int action(unsigned long data = 0);
    void dump(){
    }
};

class DelayCommand : public ICommand {
public:
    DelayCommand(int delayMs)
        : ICommand("delay")
        , mDelayMs(delayMs)
    { }
    virtual int action(unsigned long data = 0);
    void dump(){
    }
private:
    int mDelayMs;
};

int KeyDownCommand::action(unsigned long data)
{
    uinput()->ev_key(mCode, mValue);

    return 0;
}

int KeyUpCommand::action(unsigned long data)
{
    uinput()->ev_key(mCode, mValue);

    return 0;
}

int DelayCommand::action(unsigned long data)
{
    struct timeval timeout = {
        mDelayMs / 1000,
        (mDelayMs % 1000) * 1000,
    };
    select(0, 0, 0, 0, &timeout);

    return 0;
}

ICommand *CreateCommandFromXML(const XMLElement *xmlcmd = 0)
{
    if (!xmlcmd)
        return 0;

    if(strcmp(xmlcmd->Name(), KEYNODE_NAME) == 0) {//key command
        const XMLAttribute *nameA = 0;
        const XMLElement *codeE = 0, *valueE = 0;
        nameA = xmlcmd->FindAttribute(key_name_attribute);
        codeE = xmlcmd->FirstChildElement(key_code_element);
        valueE = xmlcmd->FirstChildElement(key_value_element);
        if (!codeE || !valueE)
        {
            return 0;
        }

        const char *name = 0;
        if (nameA)
            name = nameA->Value();
        int code = atoi(codeE->GetText());
        int value = atoi(valueE->GetText());
        if (value) {//key down
            return new KeyDownCommand(name, code);
        }
        else {
            return new KeyUpCommand(name, code);
        }
    }
    else if(strcmp(xmlcmd->Name(), DELAYNODE_NAME) == 0) {//delay command
        const XMLElement *msE = 0;
        msE = xmlcmd->FirstChildElement(delay_ms_element);
        if (!msE)
            return 0;

        int delay = atoi(msE->GetText());
        return new DelayCommand(delay);
    }

    return 0;
}
