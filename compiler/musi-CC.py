"""
Compile a sheet music PDF into machine interpretable pitch and duration data

Usage:  python musi-CC.py in-path out-path
"""


from hashlib    import sha256
from os         import mkdir
from sys        import argv
from subprocess import run


# Cliargs
CORRECT_ARGV    = 3
NO_PROGNAME     = 1

# Error Messages
CANNOT_CREATE   = "Cannot create temporary directory."
USAGE           = "Usage: python musi-CC.py in-path out-path"

# Exit Codes
GOOD            = 0
BAD_ARGV        = 1
BAD_TEMPDIR     = 2

# Pathing
HIDDEN          = "."
STR_ENCODING    = "utf-8"


"""
Create a temporary hidden directory for storing intermediary files.

base    - the base name of the PDF originally read in

returns - the name of the temporary directory
"""
def gen_temp_dir(base):
    dirname = HIDDEN + sha256(base.encode(STR_ENCODING)).hexdigest()
    try:
        mkdir(dirname)
    except FileExistsError:
        print(CANNOT_CREATE)
        exit(BAD_TEMPDIR)

    return dirname


def main():
    if len(argv) != CORRECT_ARGV:
        print(USAGE)
        exit(BAD_ARGV)

    path_in, path_out = tuple(argv[NO_PROGNAME:])
    dirname = gen_temp_dir(path_in)


    return GOOD


if __name__ == "__main__":
    main()
