#ifndef __ICOMMAND_H
#define __ICOMMAND_H

#include <tinyxml2.h>
#include <string>
#include <iostream>

const char KEYNODE_NAME[] =  "key";
const char DELAYNODE_NAME[] = "delay";
const char key_name_attribute[] = "name";
const char key_code_element[] = "code";
const char key_value_element[] = "down";
const char delay_ms_element[] = "ms";


class ICommand {
public:
    ICommand(const char *t = 0) {
        mType = t;
    }
    virtual ~ICommand() {}
    virtual int action(unsigned long data = 0) = 0;
    virtual void dump(){ }
private:
    std::string mType;
};

ICommand *CreateCommandFromXML(const tinyxml2::XMLElement *);

#endif  //__ICOMMAND_H

