#ifndef __PARSER_H
#define __PARSER_H

#include <utils/Vector.h>
#include "ICommand.h"

class Parser {
public:
    virtual ~Parser(){}
    typedef android::Vector<ICommand *> CommandVector;
    virtual int parse(const char *file, CommandVector &commands);
};

#endif
