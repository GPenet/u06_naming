# u06_naming
handling canonical names for puzzles
This repository is created to process names of sudoku puzzles.
In any process working with plenty of sudokus, the same issue appears: Is this sudoku a morph of a previous one.
Cleaning the redundancy is important to save processing time to build the final result
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











