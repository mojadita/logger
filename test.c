#include <stdlib.h>
#include "logger.h"

int main()
{
    LOGG_CHANN_OPS channop;
    int i;
    char *test_module = "sample";

    logg_init();
    channop = logg_channop_lookup(test_module);

    LOGG(4500, "logg_channop_lookup(\"%s\") ==> %#p", test_module, channop);
    logg(5000, __FILE__, __LINE__, __func__, "Prueba de mensaje verboso");
    DEBUG(      "MESSAGE TEST IN TWO LINES\nWITH A NUMBER(%d)\n", 36);
    INFO(       "MESSAGE TEST IN TWO LINES\nWITH A NUMBER(%d)\n", 36);
    WARNING(    "MESSAGE TEST IN TWO LINES\nWITH A NUMBER(%d)\n", 36);
    ERROR(      "MESSAGE TEST IN TWO LINES\nWITH A NUMBER(%d)\n", 36);
    CRITICAL(   "MESSAGE TEST IN TWO LINES\nWITH A NUMBER(%d)\n", 36);
    EMERG(      "MESSAGE TEST IN TWO LINES\nWITH A NUMBER(%d)\n", 36);
    FATAL(      "MESSAGE TEST IN TWO LINES\nWITH A NUMBER(%d)\n", 36);
    
    LOGG(50,    "MESSAGE TEST IN TWO LINES\nWITH A NUMBER(%d)\n", 36);
    LOGG(37,    "LAST TEST\n(also in two lines)\n\n");
    for(i = 0; i < 128; i++)
        LOGG(i, "LOGG line to see crit values, this time i == %d", i);
    DEBUG(      "logg_unregister_channops(channop) ==> %d",
            logg_unregister_channops(channop));
    DEBUG(      "logg_unregister_channops(channop) ==> %d",
            logg_unregister_channops(channop));

    return EXIT_SUCCESS;
} /* main */
