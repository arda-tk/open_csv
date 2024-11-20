#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "open_csv.h"

int main(void)
{
    csvData_t df = loadCsv();
    printf("feature names: %s\n", df.features[0]);

    DF_get_featureNames(df);

    return 0;
}
