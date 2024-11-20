/**
 * Author(s):           Arda T. Kersu
 * File name:           open_csv.c
 * Date:                1st November 2023
 *
 * Description: Header file for the library "open_csv.h" that provides easier handling and manipulation of
 *              '.csv' files with the use of C programming language. See inline comments on the source file
 *              for further details regarding any specific function of interest.
 *
 * Disclaimer: This open-source project is provided "as is" without any warranty, expressed or implied.
 *             The contributors and maintainers disclaim any an all unintended consequences or issues
 *             that may arise from its use. Users are encouraged to review and understand the software's
 *             characteristics before implementation.
 *
 *
 * Copyright @ [Arda T. Kersu]
 *
 */

#ifndef DML_OPEN_CSV_H
#define DML_OPEN_CSV_H

#include "inc/utils.h"

#define CSV_PATH        "../datasets/weather_forecast_data.csv"
#define CSV_MODE        "r"
#define CSV_DELIM       ","

#define HIGH_DATAFRAME_DETAIL               (0)     //turn this on to include more details about the dataframe

#define MAX_ALLOWED_FEATURE_NAMES           (20)    //maximum of 20 features per dataset by default
#define MAX_ALLOWED_FEATURE_VALUE           (25000) //vector size

typedef enum {FALSE, TRUE, ERROR = -1} bool_t;

#if HIGH_DATAFRAME_DETAIL == 0
typedef struct
{
    char delim;
    int rows;
    int cols;
    long size;
    char *features[MAX_ALLOWED_FEATURE_NAMES];
    float dataFrame[MAX_ALLOWED_FEATURE_VALUE][MAX_ALLOWED_FEATURE_NAMES];
}csvData_t;

#else
typedef struct
{
    char *delim;
    int rows;
    int cols;
    char *params;
    long DFSize;
    float *maxFeatureValues;
    float *minFeatureValues;
    float **dataFrame;
}csvData_t;

#endif



static void closeFile(FILE *filePtr);
static void getFrameSize(csvData_t df);
static csvData_t createDataFrame(void);


char *trimToken(char *token);
void getMinAndMaxFeatureValues(csvData_t *df);
csvData_t loadCsv(void);

void DF_get_featureNames(csvData_t df);

#endif //DML_OPEN_CSV_H
