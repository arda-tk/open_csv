# open_csv
Open-source C library that allows for easier importing and usage of .csv files.


## 🏆 Usage 🏆

- Set your dataset path, open-mode, and deliminator in ```open_csv.h```:
```
#define CSV_PATH        "../datasets/heart.csv"
#define CSV_MODE        "r"
#define CSV_DELIM       ","
```

- Create your dataset object in main:
```
csvData_t df = loadCsv();
```

- You are all set.


## ‼️ Examples ‼️

--> ```DF_get_head(df);```

Output:
```
Head: 
	63.000	 1.000	 3.000	145.000	233.000	 1.000	 0.000	150.000	 0.000	 2.300	 0.000	 0.000	 1.000	 1.000
	37.000	 1.000	 2.000	130.000	250.000	 0.000	 1.000	187.000	 0.000	 3.500	 0.000	 0.000	 2.000	 1.000
	41.000	 0.000	 1.000	130.000	204.000	 0.000	 0.000	172.000	 0.000	 1.400	 2.000	 0.000	 2.000	 1.000
	56.000	 1.000	 1.000	120.000	236.000	 0.000	 1.000	178.000	 0.000	 0.800	 2.000	 0.000	 2.000	 1.000
	57.000	 0.000	 0.000	120.000	354.000	 0.000	 1.000	163.000	 1.000	 0.600	 2.000	 0.000	 2.000	 1.000
```

--> ```DF_get_frameSize(df);```

Output:
```
The dataset consists of:
	303 rows,
	14 columns,
	that is a total of 4242 cells.
```
