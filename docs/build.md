# Build Instructions

## Folder Structure
The project folder structure is organized as follows:
```
.
├── data                        // storing sample inputs, outputs & other data
│   ├── i1.txt                
│   └── o1.txt
├── docs                        // documentation
│   └── build.md
├── templates                   // templates to copy
│   ├── testc_template.c
│   ├── testh_template.c
│   └── unity_template.c
├── tests                       // unit / main tests for source files
│   ├── testc_.c
│   └── testh_.c
├── main.c
├── Makefile                    // general problemset Makefile
└── README.md
```

## Build paths
- When compiling programs entire base directory of each problem set is included when linking object files.
- To include files from `prog` root directory in a `.c` files, use `../` file specifiers.