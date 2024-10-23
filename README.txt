To run the program, go into /bin/<system> and drag out some executables.

If you want to compile the binaries yourself, go to /src.


TMsim+.c ---| Compile |--> output+.exe: The main Turing Machine simulator.

Use with 1, 2 or 3 arguments.

./output+.exe <Number of iterations before timeout, %llu> <Maximum number of states allowed, %i>
Default maximum runtime is 4294967296 iterations, and state maximum is 1024 states.

Dependency: Reads the machine from Machine.txt.
Dependency: Reads from the input tape Intape.txt.

Outputs the result to Outtape.txt.

The machine's speed cannot be configured. It will always run at full speed.
2 billion iterations takes approximately 10 ~ 20 seconds.



Intape.txt: The input tape for the Turing Machine.
The input alphabet should be {'0', '1', '2'}.
The tape alphabet is {'0', '1', '2', '\0'}. with '\0' denoting the blank.



Outtape.txt: The output tape for the Turing Machine.
The output should contain only {'0', '1', '2'}. '\0' will cut off the output.




When writing your Turing Machine...

Use the symbol '3' to indicate a 'blank' symbol. This is because a 'NULL' character is hard to type.
'L' and 'R' will be used to move the head.

The syntax for each line is:

<State to go when read 0>,<Char to write when read 0>,<State to go when read 1>,<Char to write when read 1>...
The CSV must be exactly 8 columns. A comment can be written afterwards (It'll be ignored)

The program assumes the state number is the linenumber. So state '0' will be present on the first line.

If you don't want the transition, write '-1' for the destination state number.
An example of a state:

43,1,43,2,43,3,44,0 # This is state 43. It will go to state 44 and write a 0 of it reads a blank.
-1,3,-1,3,-1,3,-1,3 # This is state 44 and it looks like the halt state.



Stateshifter.c ---| Compile |--> Stateshifter.exe: Shifts every state number in the Machine file.

Use with 2 arguments.

./Stateshifter.exe <Number to shift by>

Dependency: Reads the machine from Machine.txt

Writes a modified copy of the machine to Modify.txt. Useful if you want to add some states because line 0 is assumed to be the start.



IntapeWriter.c ---| Compile |--> IntapeWriter.exe: Writes some pattern to the input tape.

Change the code yourself. If you're tired of typing into the file, use this program to write it for you
Make sure to compile it every time!




Code first constructed by 354ika (Hong Fulin).

The author(s) are not liable for any kind of physical (burning computer) or emotional damage (as a result of frustration) caused to
anyone who chooses to use this program or derivative works of this program.
