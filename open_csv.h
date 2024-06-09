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

#include <stdio.h>

#define CSV_PATH        ("../data/synthetic_data_elliptical.csv")
#define CSV_MODE        ("r")
#define CSV_DELIM       (", ")

#define HIGH_DATAFRAME_DETAIL       (0)     // turn this on to include more details about the dataframe

typedef enum {FALSE, TRUE, ERROR = -1} bool_t;

#if HIGH_DATAFRAME_DETAIL == 0
typedef struct
{
    char *delim;
    int rows;
    int cols;
    char *params;
    long DFSize;
    float **dataFrame;
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



void closeFile(FILE *filePtr);
int *getDFsize(FILE *filePtr);
csvData_t *createDataFrame(FILE *filePtr);
char *trimToken(char *token);
void getMinAndMaxFeatureValues(csvData_t *df);
csvData_t *loadCsv(FILE *filePtr);

#endif //DML_OPEN_CSV_H
