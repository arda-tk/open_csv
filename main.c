#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "open_csv.h"

int main(void)
{
    csvData_t df = loadCsv();

    DF_get_featureNames(df);
    DF_get_frameSize(df);

    return 0;
}
