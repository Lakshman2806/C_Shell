# OSN Assignment 3

## Assumptions and behaviour
* At start ``` cd - ``` stays in the home directory
* The code uses ``` /proc ``` files hence if it does not exist the ``` pinfo ``` command failsteh
* The ``` discover ``` command may fail in large folders as it uses recursion which may cause stack smashing
* Some commands may print it wrong when a ```/``` has been given at the last of the folder
* The file ```ShellHistory``` would be created in the folder if it is not present.
* The total time of a command will be dsiplayed if it exceeds 1 second
* The shell can crash if abnormally large files or file_names are present
* The shell did not handle directories with space in the names of it

## Executing file
* run the command ``` make ``` which produces an executable a.out
* Note that quit and clear files have also been implmented.


## Functions in files:
### cdc.c:
* This file contains the functions that are required to execute the command "cd"

### cmd.c:
* This file contains the functions that are reuqired to parse the commani.e split the command into individual components

### colours.c :
* This file contains the functions that are used in colour coding the output

### discover.c :
* This file contains the functions that are used in implementing "discover" command. 

### echo.c :
* This file contains the functions that are used in implementing "echo" command

### fg.c :
* This file contains the fg handler that is used in implementing fg

### head.h:
* This file will contain all the header files used in this C-shell

### history.c:
* This file contains the functions that are used in implementing history command

### jobs.c:
* This file contains the functions that are used in implementing the job command

### ls.c:
* This file contains the functions that are required to implement the ls function

### pwd.c:
* This file has the function that gives the present working directory

### raw.c:
* This file contains the function that autocompletes

### redirect.c:
* This file contains the functions that takes care of the redirection of input and output

### ShellHistory:
* This file contains the history of the shell

## Others:
* All other files are test files that can be used for testing
