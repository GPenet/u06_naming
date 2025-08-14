# u06_naming
handling canonical names for puzzles

This repository is created to process names of sudoku puzzles.

In any process working with plenty of sudokus, the same issue appears: Is this sudoku a morph of a previous one.
Cleaning the redundancy is important 

to save processing time 

to build the final result

to avoid redundancy, the classical way is to use a "canonical morph of the entry". For a solution grid, the most common canonical morph is the min lexical form of the solution grid.

For a puzzle, we use here the puzzle morphed to the canonical solution grid. As the solution grid can have auto morphs, an additional condition is to use the min lexical morph of the puzzle for these auto morphs of the solution grid.

Having the ED (essentially different) puzzle, next point is "how do you show it to have a short record.

We know that we have 5 472 730 538 ED solution grids. I posted a DLL giving the solution grid for a given rank in the min lexical catalog and reverse.

This is the start to define a short canonical name. Having the solution grid reduced to this number, the puzzle can be defined by the given cells, a 81 bits field. Next point is "how can we print this sequence of 81 binary values.

The hexadecimal print is well known to show a 4 bits sequence. The print uses 16 characters, 0-9 plus a-f or A-F. The print will be shorter if we use more printable characters.

If we stick to the 128 first ascii values, the maximum that we can do is to use 64 characters for a 6 bits sub field. This is what is done here with the following sequence 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}

These 64 printable characters are here in increasing ascii value, giving room to the use of many programs as the “sort” program.
Then, the 81 bits field can be printed as 14 characters 6x13= 78 bits, the last characters with 3 bits used.
This is the first possible name for a puzzle

10 characters for the rank of the solution grid 14 characters for the bit field, plus one separator to have the file easy to process.

This "relatively short" name has a big interest; the solution grid rank is explicitly given.

If a very short name is wanted, we can switch easily to the second name, a 19 bytes name. The solution grid rank uses 33 bits the bit field 81 bits This is a total of 114 bits = 19x6 These 114 bits are printed in the same way.
Note: such names in a file sorted are compressed very efficiently by the .zip process.

This small package do the main functions needed to use the naming process. The code is relatively short, but uses DLLs of very big size to do some key operation as

back force solver 

grid morphing to minlex 

virtual catalog of min lexical solution grids. ....

The corresponding DLLs must be in a directory used to load programs.

====================================================================

the package functions are using my "standard" frame and have usually one entry file, one output file and a possible redirection of "cout" where can be seen a short message if the process fails.

As this is mainly for steps in a wider process, the entry file must fit with the process called and the process stops in case of anomaly seen.

The command line will have some common parameters for all functions as in this line

sku06_names -c5  -v4-2 -i_tmissingsp_rat.txt    -o_tmo2   >_tmo2.txt  "skfred to p er r b"

sku06_names for theprogram sku06_names.exe

-cn where n is the function id 

-v4-p an optional parameter (default 0) needed in some functions

-ixxx where xxx is the file name here _tmissingsp_rat.txt 

-oyyyy thhe root file name for output; 

  mostoften the output will be yyy_file1.txt (only one file in output)

  but in one process, a second output file  yyy_file2.txt can be delivered

>zzz redirection for the "cout" file

==================================================================
function -c0

entry any valid puzzle 

The process is cancelled if a wrong puzzle is seen 

To limit the risk of an endless brute force, a minimum of 17 clues is required, but the caller should take care not to send puzzles with plenty of solutions.

standard output name  in fix format "rrrrrrrrrr bbbbbbbbbbbbbb" length 10+1+14=25

-v4-n is an output request for a second file

  puzzle in canonical morph;rank;bitfield

  =========================

  function -c1 
  
  This is a variant of the process C4 optimal when the entry file is sorted on the solution grid rank.
  Here the solution grid is searche only once per rank.
  As this is the longest part of the process, the runtime cal fall down sharply.

  ==============================================================
  function -c2

  convert rank + bit field to 19 bytes

  entry file must be 10+1+14  or rank;bitfield
  
  out file is name 19;entry 

  This process could have entries where rank;bitfile should be replaced but the use is not yet identified
  

  ==============================================================
  function -c3

  convert name  19 bytes to rank plus bit field

  as for the process 3, this is a "test code" with an entry file of names 19 and an output having the rank+bitf followed bay the entry.

the standard output is 10+1+14

the output is rank;bitf is -v4-n is in the command line

=================================================================

function -c4   

this process delivers the puzzle in min lex morphfor a given name 10+1+14 or rank;bitf.

Some runtime can be saved using the output puz;rank;name in the process -c1. In the vicinity search, names are used to clear redundancy, then new puzzles are rated. One problem is that SKFR loose the name linked to the puzzle, so another way (data base) must be used to match the records.

function -c4 is relatively expensive, but can go back from the name to the puzzle.

==================================================================

function -c5

this is a tailor made function to go from skfr output (assumed to be done using a canonical puzzle) to the record to save.
here a typical output of skfr and the expected output
.....67.....18.26.....27.412.4.......9...8...831.7......6.4287......1....4..6..12 ED=11.0/8.4/2.6
.....67.....18.26.....27.412.4.......9...8...831.7......6.4287......1....4..6..12;110;5411131853;W1Rmk0Yu2q34I6

110 is for 11.0

then comes the rank and the name.



  















