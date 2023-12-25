"""
Compile a sheet music PDF into machine interpretable pitch and duration data

Usage:  python musi-CC.py in-path out-path
"""


from hashlib    import sha256
from os         import mkdir
from shutil     import rmtree
from subprocess import DEVNULL, run
from sys        import argv


# Cliargs
CORRECT_ARGV    = 3
NO_PROGNAME     = 1

# Error Messages
CANNOT_CONVERT  = "Cannot convert the specified PDF"
CANNOT_CREATE   = "Cannot create temporary directory."
USAGE           = "Usage: python musi-CC.py in-path out-path"

# Exit Codes
GOOD            = 0
BAD_ARGV        = 1
BAD_TEMPDIR     = 2
BAD_MUSIC       = 3

# File Conversions
PDF2PNG         = lambda p, d : ["pdftoppm", "-gray", "-png", p, f"{d}/-"]

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
    if run(PDF2PNG(path_in, dirname), stdout = DEVNULL, stderr = DEVNULL).returncode:
        rmtree(dirname)
        print(CANNOT_CONVERT)
        exit(BAD_MUSIC)

    #rmtree(dirname)

    return GOOD


if __name__ == "__main__":
    main()
