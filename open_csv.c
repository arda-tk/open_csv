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
        fprintf(stdout, "File has been closed safely by closeFile().\n");
        fclose(filePtr);
    }
}

/**
 * @brief Get the size of a data frame from a '.csv' file.
 *
 * This function reads a '.csv' file pointed to by 'filePtr' and determines the number of rows and columns
 * in the data frame. It skips the first row (usually containing feature names) and counts the rows
 * and columns in the dataset.
 *
 * @param filePtr A pointer to the '.csv' file to analyze.
 * @return An integer array containing the number of rows and columns, or NULL if an error occurs.
 *
 * @note This function dynamically allocates memory for the integer array 'retVal,' which should be
 *       freed by the caller when no longer needed to avoid memory leaks.
 *
 * @code
 *   // Example usage:
 *   FILE *file = fopen("data.csv", "r");
 *   int *size = getDFsize(file);
 *   if (size != NULL) {
 *       printf("Number of rows: %d\n", size[0]);
 *       printf("Number of columns: %d\n", size[1]);
 *       free(size); // Free the allocated memory
 *   }
 *   else
 *   {
 *       puts("Error occurred while getting data frame size.");
 *   }
 *   // Analyze the '.csv' file and retrieve the data frame size...
 * @endcode
 */
static int *getFrameSize(void)
{
    char buffer[1024];      //buffer
    int skipFirstRow = 0;   //skips the "feature names" row, commonly the first row in the file
    int dfRows = 0, dfCols = 0;  //number of rows and cols
    int *retVal = (int *)malloc(sizeof(int) * 3);  //return value

    /*if(csvPtr != NULL) //check file validity
    {
        printf("The file has been opened by getFrameSize().\n");
    }
    else
    {
        csvPtr = fopen(CSV_PATH, CSV_MODE);

        if(csvPtr == NULL)
        {
            puts("getFrameSize(): Could not open the file.");
            return NULL;
        }
    }*/

    bool_t lock = FALSE;
    while(fgets(buffer, 1024, csvPtr)) //pull file contents row by row
    {
        if(MAX_ALLOWED_FEATURE_VALUE - 1 == dfRows)
        {
            LOG_ERROR("The program prevented itself from segfault (via array overflow).\n");
            abort();
        }

        char *tokens = strtok(buffer, CSV_DELIM); //split rows into tokens separated by deliminator

        if(skipFirstRow == 0) //this condition locks itself out once it has executed for the value 0
        {
            skipFirstRow = 1;
            continue;
        }

        if(lock == FALSE) //also runs once and locks itself out. counts the number of columns in dataset
        {
            while(tokens) //count tokens
            {
                tokens = strtok(NULL, CSV_DELIM);
                dfCols++;
            }
            lock = TRUE;
        }

        dfRows++; //count rows
    }

    retVal[0] = dfRows;
    retVal[1] = dfCols;

    //closeFile(csvPtr);

    return retVal;
}

/**
 * @brief Create a CSV data frame structure based on file information.
 *
 * This function analyzes a '.csv' file pointed to by 'filePtr' to determine its size and creates
 * a CSV data frame structure accordingly. The data frame structure includes the number of rows,
 * columns, delimiter, and feature parameters.
 *
 * @param filePtr A pointer to the '.csv' file to create the data frame from.
 * @return A pointer to a dynamically allocated 'csvData_t' structure representing the data frame.
 *
 * @note The caller is responsible for freeing the memory allocated for the 'csvData_t' structure
 *       and its members when it is no longer needed to avoid memory leaks.
 *
 * @code
 *   // Example usage:
 *   FILE *file = fopen("data.csv", "r");
 *   csvData_t *dataFrame = createDataFrame(FILE);
 *   if (dataFrame != NULL) {
 *       // Use the data frame...
 *       free(dataFrame->delim);
 *       free(dataFrame->params);
 *       free(dataFrame);
 *   }
 *   else
 *   {
 *       puts("Error occurred while creating the data frame.");
 *   }
 *   // Create a data frame based on the '.csv' file...
 * @endcode
 */
static csvData_t createDataFrame(void)
{
    int *dfSize = getFrameSize(); //get dataframe size info

    csvData_t dataFrame = {
        .delim = CSV_DELIM,
        .rows = dfSize[0],
        .cols = dfSize[1],
        .size = dfSize[0] * dfSize[1]
    };

    //free(dfSize);

#if HIGH_DATAFRAME_DETAIL == 1
    dataFrame->maxFeatureValues = (float *)malloc(sizeof(float) * dataFrame->cols);
    dataFrame->minFeatureValues = (float *)malloc(sizeof(float) * dataFrame->cols);
#endif

    return dataFrame;
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
    puts("called trimToken");

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

    puts(trimmedToken);

    return trimmedToken;
}
/**
 * ...
 */
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

    puts("test1");

    int *dfSize = getFrameSize(); //get dataframe size info

    csvData_t df = {
        .delim = CSV_DELIM,
        .rows = dfSize[0],
        .cols = dfSize[1],
        .size = dfSize[0] * dfSize[1]
    };

    free(dfSize);
    //csvData_t df = createDataFrame(); //create and initialize dataframe
    puts("test2");

    //EXTRACT FEATURE NAMES ---------------------------------------------------

    {
        fgets(buffer, 1024, csvPtr);       //get the first line of csv file


        puts("before-op");
        while (fgets(buffer, 80, csvPtr))
        {
            puts("aaaaa");

            // If you need all the values in a row
            char *token = strtok(buffer, ",");
            while (token)
            {
                printf("tokens inside: %s\n", token);

                token = strtok(NULL, ",");
            }
        }
        puts("op-done");


        puts("test3");
        char *tokens = strtok(buffer, &df.delim);   //split into multiple tokens
        puts("test4");
        int tokenCount = 0;

        if(tokens == NULL)
        {
            puts("lol");
        }

        while(tokens)
        {
            if(MAX_ALLOWED_FEATURE_NAMES - 1 == tokenCount)
            {
                LOG_ERROR("The program has prevented a buffer overflow. "
                          "Please increase the maximum allowed number of feature names\n");
                abort();
            }

            puts("test5");

            char *label = trimToken(tokens); //trim token of unwanted characters

            puts("test6");

            df.features[tokenCount] = (char *)malloc(sizeof(char) * (strlen(label) + 1));
            strncat(df.features[tokenCount], label, sizeof(char) * strlen(label)); //write into dataframe
            strncat(df.features[tokenCount], '\0', sizeof(char));
            printf("\"%s\", \n", trimToken(tokens)); //print dataset features, can be commented out
            tokens = strtok(NULL, &df.delim); //split the next token from source
            //free(label);
            puts("test7");
            tokenCount++;
        }
    }

    //EXTRACT DATA POINTS------------------------------------------------------

    {
        int row = 0;
        while(fgets(buffer, 1024, csvPtr)) //get data from dataset row by row
        {
            int col = 0;
            char *tokens = strtok(buffer, &df.delim); //split into tokens

            while(tokens)
            {
                df.dataFrame[row][col] = atof(tokens); //feed data into dataframe
                tokens = strtok(NULL, &df.delim); //further break into tokens
                col++;
            }

            row++;
        }
    }
    printf("feature names on the inside: %s\n", df.features[0]);

#if HIGH_DATAFRAME_DETAIL == 1
    getMinAndMaxFeatureValues(df); // if dataframe is highly detailed, pull min/max feature values
#endif

    fclose(csvPtr);

    return df;
}

void DF_get_featureNames(csvData_t df)
{
    for(int index=0; index < df.cols; index++)
    {
        printf("%s\t", df.features[index]);
    }
    puts(" ");

    printf("rows: %d\n", df.rows);
    printf("cols: %d\n", df.cols);
}
