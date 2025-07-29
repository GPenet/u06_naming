# u06_naming
handling canonical names for puzzles

This repository is created to process names of sudoku puzzles.

In any process working with plenty of sudokus, the same issue appears :
Is this sudoku a morph of a preious one. 

Cleaning the redundancy is important
to save processing time
to build the final result 

to avoid redundancy, the classical way is to use a "canonical  morph of the entry". 
For a solution grid, the most common canonical morph is the min lexical form of the solution grid.

For a puzzle, we use here the puzzle morphed to the canonical solution grid.
As the solution grid can have auto morphs, an additionnal condition is to use the min lexical morph of the puzzle for these auto morphs of the solution grid.

Having the ED (essentially different) puzzle, next point is "how do you show it to have a short record.

We know that we have 5 472 730 538 ED solution grids. I posted a DLL giving the solution grid for a given rank in the min lexical catalog.

This is the start to define a short canonical name.
Having the solution grid reduced to this number, the puzzle can be definet by the given cells, a 81 bits field.




