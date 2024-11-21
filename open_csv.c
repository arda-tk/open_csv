/**
 * Author(s):           Arda T. Kersu
 * File name:           open_csv.c
 * Date:                1st November 2023
 *
 * Description: Source code for the library "open_csv.h" that provides easier handling and manipulation of
 *              '.csv' files with the use of C programming language. See inline comments for further details
 *              regarding any specific function of interest.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "open_csv.h"

static FILE *csvPtr = NULL;

/**
 * @brief Close a file safely and report the status.
 *
 * This function attempts to close the specified file pointed to by 'filePtr'. It checks if the
 * file pointer is NULL and reports whether the file was closed successfully or if it could
 * not be found.
 *
 * @param filePtr A pointer to the file to be closed.
 *
 * @note This function reports the status of file closure using standard output and error streams.
 *
 * @code
 *   // Example usage:
 *   FILE *file = fopen("example.txt", "r");
 *   closeFile(file);
 *   // Attempt to close the file and report the status...
 * @endcode
 */
static void closeFile(FILE *filePtr)
{
    if(filePtr == NULL)
    {
        LOG_ERROR("closeFile(): No files were open. closeFile has failed to close any file(s).\n");
    }
    else
    {
        fclose(filePtr);
        SYS_MSG(stdout, "File has been closed safely by closeFile().\n");
    }
}

/**
 * @brief Trim a token by removing non-alphanumeric characters.
 *
 * This function takes a token as input and removes any non-alphanumeric characters from it.
 * It returns a dynamically allocated string containing the trimmed token.
 *
 * @param token A null-terminated string representing the token to be trimmed.
 * @return A dynamically allocated string containing the trimmed token, or NULL if an error occurs.
 *
 * @note The caller is responsible for freeing the memory allocated for the returned trimmed token
 *       when it is no longer needed to avoid memory leaks.
 *
 * @code
 *   // Example usage:
 *   char *originalToken = "abc!@123";
 *   char *trimmed = trimToken(originalToken);
 *   if (trimmed != NULL) {
 *       printf("Original Token: %s\n", originalToken);
 *       printf("Trimmed Token: %s\n", trimmed);
 *       free(trimmed); // Free the allocated memory
 *   }
 *   else
 *   {
 *       puts("Error occurred while trimming the token.");
 *   }
 *   // Trim a token by removing non-alphanumeric characters...
 * @endcode
 */
char *trimToken(char *token)
{
    char *trimmedToken = (char *)malloc(strlen(token) + 1);
    trimmedToken[0] = '\0';

    int loop = 0, innerLoop = 0;
    while(*(token + loop) != '\0')
    {
        if(isalnum(*(token + loop)))
        {
            *(trimmedToken + innerLoop) = *(token + loop);
            innerLoop++;
        }

        loop++;
    }

    return trimmedToken;
}
#if HIGH_DATAFRAME_DETAIL == 1
void getMinAndMaxFeatureValues(csvData_t *df)
{
    for(int colIndex=0; colIndex < df->cols; colIndex++)
    {
        float maxFeatureValue = df->dataFrame[0][colIndex];
        float minFeatureValue = df->dataFrame[0][colIndex];

        for(int rowIndex=1; rowIndex < df->rows; rowIndex++)
        {
            maxFeatureValue = (df->dataFrame[rowIndex][colIndex] > maxFeatureValue) ? df->dataFrame[rowIndex][colIndex] : maxFeatureValue;
            minFeatureValue = (df->dataFrame[rowIndex][colIndex] < minFeatureValue) ? df->dataFrame[rowIndex][colIndex] : minFeatureValue;
        }

        df->maxFeatureValues[colIndex] = maxFeatureValue;
        df->minFeatureValues[colIndex] = minFeatureValue;
    }
}
#endif

/**
 * @brief Load data from a '.csv' file into a CSV data frame.
 *
 * This function opens a '.csv' file pointed to by 'filePtr', reads the data from the file,
 * and loads it into a CSV data frame. It extracts feature names from the first row and stores
 * them in the data frame's 'params' member. Data points are read and stored in the 'dataFrame'
 * member of the data frame.
 *
 * @param filePtr A pointer to the '.csv' file to load data from.
 * @return A pointer to a dynamically allocated 'csvData_t' structure representing the loaded data frame.
 *
 * @note The caller is responsible for freeing the memory allocated for the returned data frame
 *       when it is no longer needed to avoid memory leaks.
 *
 * @code
 *   // Example usage:
 *   FILE *file = NULL;
 *   csvData_t *dataFrame = loadCsv(file);
 *   if (dataFrame != NULL)
 *   {
 *       // Use the loaded data frame...
 *       // Don't forget to free the allocated memory when done.
 *       free(dataFrame->params);
 *       for (int row = 0; row < dataFrame->rows; row++)
 *       {
 *           free(dataFrame->dataFrame[row]);
 *       }
 *       free(dataFrame->dataFrame);
 *       free(dataFrame);
 *   }
 *   else
 *   {
 *       puts("Error occurred while loading the '.csv' file.");
 *   }
 *   // Load data from a '.csv' file into a CSV data frame...
 * @endcode
 */

csvData_t loadCsv()
{
    char buffer[1024];

    csvPtr = fopen(CSV_PATH, CSV_MODE);

    if(csvPtr != NULL)
    {
        puts("the file has been opened\n");
    }

    csvData_t df = {
        .delim = CSV_DELIM,
        .rows = 0,
        .cols = 0,
        .size = 0l
    };

    //EXTRACT FEATURE NAMES ---------------------------------------------------

    fgets(buffer, 1024, csvPtr);       //get the first line of csv file
    char *tokens = strtok(buffer, CSV_DELIM);   //split into multiple tokens
    int tokenCount = 0;

    while(tokens)
    {
        if(MAX_ALLOWED_FEATURE_NAMES - 1 == tokenCount)
        {
            LOG_ERROR("The program has prevented a buffer overflow. "
                "Please increase the maximum allowed number of feature names\n");
            abort();
        }

        char *label = trimToken(tokens); //trim token of unwanted characters
        df.features[tokenCount] = (char *)malloc(sizeof(char) * (strlen(label) + 1));
        strncpy(df.features[tokenCount], label, sizeof(char) * strlen(label)); //write into dataframe
            /*
             * --> strncat(df.features[tokenCount], '\0', sizeof(char));
             *
             * do NOT try this bc this tries to cat yet another "string" to the existing string, which,
             * naturally, ends with a '\0'. so the '\0' you are trying to cat will be catted as "\0\0" lol -> SIGSEGV
             * bc you only allocated memory for one single '\0'
            */
        df.features[tokenCount][strlen(label)] = '\0';  //strncpy may also work. beware of the comment above though
        df.cols++;
        tokens = strtok(NULL, CSV_DELIM); //split the next token from source
        free(label);
        tokenCount++;
    }

    //EXTRACT DATA POINTS------------------------------------------------------
    int row = 0;
    while(fgets(buffer, 1024, csvPtr)) //get data from dataset row by row
    {
        int col = 0;
        char *tokens = strtok(buffer, CSV_DELIM); //split into tokens

        while(tokens)
        {
            df.dataFrame[row][col] = atof(tokens); //feed data into dataframe
            tokens = strtok(NULL, CSV_DELIM); //further break into tokens
            col++;
        }

        row++;
        df.rows++;
    }

    df.size = (long)df.rows * (long)df.cols;

#if HIGH_DATAFRAME_DETAIL == 1
    getMinAndMaxFeatureValues(df); // if dataframe is highly detailed, pull min/max feature values
#endif

    fclose(csvPtr);

    return df;
}

void DF_get_featureNames(csvData_t df)
{
    printf("Features:\n\t[\t");
    for(int index=0; index < df.cols; index++)
    {
        printf("~\"%s\"~   ", df.features[index]);
    }
    printf("]\n\n");
}

void DF_get_frameSize(csvData_t df)
{
    printf("The dataset consists of:\n"
           "\t%d rows,\n"
           "\t%d columns,\n"
           "\tthat is a total of %ld cells.\n\n", df.rows, df.cols, df.size);
}

void DF_get_head(csvData_t df)
{
    printf("Head: \n");

    for (int rindex=0; rindex < CSV_NUM_OF_ROWS_AT_HEAD; rindex++)
    {
        for (int cindex=0; cindex < df.cols; cindex++)
        {
            printf("\t%6.3f", df.dataFrame[rindex][cindex]);
        }
        printf("\n");
    }

    printf("\n");
}

void DF_get_tail(csvData_t df)
{
    printf("Tail: \n");

    for (int rindex=df.rows - 1; rindex >= df.rows - CSV_NUM_OF_ROWS_AT_HEAD; rindex--)
    {
        for (int cindex=0; cindex < df.cols; cindex++)
        {
            printf("\t%6.3f", df.dataFrame[rindex][cindex]);
        }
        printf("\n");
    }

    printf("\n");
}

void DF_get_randomSamples(csvData_t df)
{
    int sample_indexes[CSV_NUM_OF_ROWS_AT_RANDOM] = {0};

    srand(time(NULL));

    for (int index=0; index < CSV_NUM_OF_ROWS_AT_RANDOM; index++)
    {
        sample_indexes[index] = rand() % df.rows;
    }

printf("Random Samples: \n");

    for (int rindex=0; rindex < CSV_NUM_OF_ROWS_AT_RANDOM; rindex++)
    {
        for (int cindex=0; cindex < df.cols; cindex++)
        {
            if (cindex == 0)
            {
                printf("\t%d)\t\t%6.3f",sample_indexes[rindex], df.dataFrame[sample_indexes[rindex]][cindex]);
            }
            else
            {
                printf("\t%6.3f", df.dataFrame[sample_indexes[rindex]][cindex]);
            }
        }
        printf("\n");
    }

    printf("\n");
}
