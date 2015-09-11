#include "Parser.h"
#include <tinyxml2.h>

using namespace tinyxml2;

int Parser::parse(const char *file, CommandVector &commands)
{
    if (access(file, R_OK)) {
        printf("file %s cannot be accessed.\n", file);
        return 0;
    }

    XMLDocument doc;
    doc.LoadFile(file);

    XMLElement *root = 0;
    root = doc.RootElement();

    XMLElement *curr_key = 0;
    curr_key = root->FirstChildElement();
    while (curr_key) {
        ICommand *cmd = CreateCommandFromXML(curr_key);
        curr_key = curr_key->NextSiblingElement();
        if (cmd) {
            commands.push_back(cmd);
        }
    }

    return 0;
}

