# FIT 2100
Operating Systems Assignment A

This is a File Utility program written entirely in C,
designed to open a given file and output at least the first n words as provided,
and append to a given file, or display to the user in the terminal if not provided

As such, the user may compile the C source file by opening the command line,
navigating to the current user directory, and running:
    $ gcc 31865224_fileutil.c -o fileutil
which will create a program named "fileutil" in the current directory.
We can now run this by running the following in the command line:
    $ ./fileutil
This will output any user data to the command line, and write to any files as
required. Now, we can specify the absolute path to the source file to read from
as well, otherwise it will just use the sample file in the current directory:
    $ ./fileutil /home/student/dir1/a.txt
The first other command line argument we can specify is the destination file to
append words to, and if not provided, fileutil will simply display words to the
user:
    $ ./fileutil -a /home/student/dir1/a.txt
The second other command line argument is the number of words to read in from
the source file. The program will read in at least this number of words, or all
if there are less
    $ ./fileutil -n 45
Multiple command line arguments may be used together, however, the source file
must always be the first if used. The others may be used in any order. All the
arguments could even be used together, like:
    $ ./fileutil /home/student/dir1/a.txt -a /home/student/dir2/b.txt -n 15
    $ ./fileutil /home/student/dir1/a.txt -n 15 -a /home/student/dir2/b.txt
