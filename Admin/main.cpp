#include "src/admin.h"
#include "src/dbif.h"
#include "src/log.h"
#include "src/guidummy.h"
#include "string.h"

using namespace std;

int main()
{
   Controller tmp;

   Admin LOL(&tmp);
   LOL.clean();
   LOL.getErrorPT(22);

}


