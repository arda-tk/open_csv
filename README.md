# open_csv
Open-source C library that allows for easier importing and usage of .csv files.

Usage:

```FILE *fp = NULL;```
```csvData_t *df = loadCsv(fp);```

Dataset's file name/open mode can be modified in the header file, defined as:

```#define CSV_PATH        ("../data/synthetic_data_elliptical.csv")```
```#define CSV_MODE        ("r")```
