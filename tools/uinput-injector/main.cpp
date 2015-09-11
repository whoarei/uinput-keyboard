#include <utils/Log.h>
#include <unistd.h>

#include "ICommand.h"
#include "Parser.h"

using namespace tinyxml2;
using namespace android;

int main(int argc, char **argv)
{
    int infinite = 0;

    if (argc > 1 && (strcmp(argv[1], "-i") == 0))
        infinite = 1;

    Parser::CommandVector commands;
    Parser parser;
    parser.parse("cmd.xml", commands);
    if (commands.size() == 0) {
        printf("no command found.\n");
        return 0;
    }

    printf("initializing uinput...\n");
    sleep(2);
    printf("initiaze uinput done.\n");

    Parser::CommandVector::iterator it = commands.begin();
    while (1) {
        if(it == commands.end()) {
            if(infinite)
                it = commands.begin();
            else
                break;
        }


        if (*it)
            (*it)->action();

        it++;
    }

    return 0;
}

