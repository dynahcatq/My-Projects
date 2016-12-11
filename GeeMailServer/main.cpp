#include "geemailserver.h"
#include <iostream>
#include <sqlite3.h>

int main(int argc, char *argv[])
{
    CGeeMailServer GeeMail;
    GeeMail.SetupListeningSocket();

    while (1)
    {
        if (GeeMail.WaitingConnection())
        {
            GeeMail.ReceiveAndParseRequest();
        }
    }

    return 0;
}

