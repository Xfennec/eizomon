#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"
#include "eizo_ctrl.h"

eizo_ctrl_monitor *monitor;

int main(int argc, char *argv[])
{

monitor = eizo_ctrl_open(EM_EIZO_FG_2421_VENDORID, EM_EIZO_FG_2421_PRODUCTID, NULL);
if (!monitor) {
    fprintf(stderr, "Unable to find monitor.\n");
}


return 0;
}
