
#include "logger.h"

int main()
{
    LOGG_CHANN_OPS channop;
    logg_init();
    channop = logg_channop_lookup("a");
    DEBUG(      "PRUEBA DE MENSAJE EN DOS LÍNEAS\nCON UN NÚMERO (%d)\n", 36);
    INFO(       "PRUEBA DE MENSAJE EN DOS LÍNEAS\nCON UN NÚMERO (%d)\n", 36);
    WARNING(    "PRUEBA DE MENSAJE EN DOS LÍNEAS\nCON UN NÚMERO (%d)\n", 36);
    ERROR(      "PRUEBA DE MENSAJE EN DOS LÍNEAS\nCON UN NÚMERO (%d)\n", 36);
    CRITICAL(   "PRUEBA DE MENSAJE EN DOS LÍNEAS\nCON UN NÚMERO (%d)\n", 36);
    EMERG(      "PRUEBA DE MENSAJE EN DOS LÍNEAS\nCON UN NÚMERO (%d)\n", 36);
    FATAL(      "PRUEBA DE MENSAJE EN DOS LÍNEAS\nCON UN NÚMERO (%d)\n", 36);
    logg(50, __FILE__, __LINE__, __func__, "PRUEBA DE MENSAJE EN DOS LÍNEAS\nCON UN NÚMERO (%d)\n", 36);
    logg(37, __FILE__, __LINE__, __func__, "ULTIMA PRUEBA\n(en dos lineas también)\n\n");
    logg(5000, __FILE__, __LINE__, __func__, "Otra prueba más\n(en dos lineas también)\n\n");

} /* main */
