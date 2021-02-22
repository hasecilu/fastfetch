#include "fastfetch.h"

#include <string.h>

void ffPrintHost(FFstate* state)
{
    if(ffPrintCachedValue(state, "Host"))
        return;

    char host[1024];

    FILE* nameFile = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
    if(nameFile == NULL)
    {
        ffPrintError(state, "Host", "fopen(\"/sys/devices/virtual/dmi/id/product_name\", \"r\") == NULL");
        return;
    }
    if(fscanf(nameFile, "%[^\n]", host) != 1)
    {
        ffPrintError(state, "Host", "fscanf(nameFile, \"%[^\\n]\", name) != 1");
        return;
    }
    fclose(nameFile);

    FILE* versionFile = fopen("/sys/devices/virtual/dmi/id/product_version", "r");
    if(versionFile != NULL)
    {
        ssize_t len = strlen(host);
        host[len] = ' ';
        if(fscanf(versionFile, "%[^\n]", host + len + 1) == 0); //fail silently, if reading of version fails, just show now version
        fclose(versionFile);
    }

    ffSaveCachedValue(state, "Host", host);

    ffPrintLogoAndKey(state, "Host");
    puts(host);
}