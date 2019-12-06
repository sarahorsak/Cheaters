Partners: Maddi Sikorski and Sarah Orsak
In this program, the main goal is to learn how to use hash tables
To begin, you must unzip the files. Inside this zipped folder, you will find a
-main.cpp
-readme.txt
-makefile

Compile the file to ensure they are all linked.
g++ -o plagiarismCatcher main.cpp -std=c++0x

Compiling can also be done by typing in the command line after being in the program directory: make

Then, run the code. In order to run the code, you must type in the command line: ./plagiarismCatcher path/to/files 6 200

The 6 resembles the number of words per chunk. There would be 6 words per chunk in this example
The 200 resembles the cut-off of what to print onto the screen of plagiarism amount, based off of the chunks. There must
be at least 200 counts of plagiarism for this example.

The code works by reads in the arguments passed in. It will place the number of chunks as well as the number of similar
chunks into an int variable.

We set the hash table size to a random number. We also create a 2D array in order to place the numbers of similarities
between each set of 2 files.

We push the chunks to a queue in order to get their hash key and place them in a hash table. The hash table is a seperate
chaining table. To keep the same queue, we pop the first word then push the next word of the file onto the queue.

Once all of the files are hashed to the table, then we count the number of similarities of each chunk and place the
number of similarites into the 2D array. We delete the nodes as the chunks are counted.

After, we sort the collisions in a list (which only has the ones larger than the beginning number of similarities cut
off), and then print out the files that are most plagiarized.