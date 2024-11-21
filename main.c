#include "open_csv.h"

int main(void)
{
    csvData_t df = loadCsv();

    DF_get_featureNames(df);
    DF_get_frameSize(df);
    DF_get_head(df);
    DF_get_tail(df);
    DF_get_randomSamples(df);

    return 0;
}
