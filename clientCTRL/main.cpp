#include "src/adminclient.h"
#include "src/guinf.h"

int main()
{
    Controller erik;
    AdminClient LOL(&erik);

    LOL.listen();
}
