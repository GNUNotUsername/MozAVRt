"""
Compile a sheet music PDF into machine interpretable pitch and duration data

Usage:  python musi-CC.py in-path out-path
"""


from hashlib    import sha256
from os         import listdir, mkdir, rename
from shutil     import rmtree
from subprocess import DEVNULL, run
from sys        import argv


# Cliargs
CORRECT_ARGV    = 3
NO_PROGNAME     = 1

# Error Messages
CANNOT_CONCAT   = "Cannot concatenate PDF pages."
CANNOT_CONVERT  = "Cannot convert the specified PDF."
CANNOT_CREATE   = "Cannot create temporary directory."
USAGE           = "Usage: python musi-CC.py in-path out-path"

# Exit Codes
GOOD            = 0
BAD_ARGV        = 1
BAD_TEMPDIR     = 2
BAD_MUSIC       = 3
MAGICK_FAIL     = 4

# File Conversions
CONCAT_IMGS     = lambda d      : ["magick", f"{d}/*", "-append", f"{d}/working.png"]
PDF2PNG         = lambda p, d   : ["pdftoppm", "-gray", "-png", p, f"{d}/-"]

# Pathing
HIDDEN          = "."
STR_ENCODING    = "utf-8"


"""
Convert a PDF into a greyscale png within the a temporary directory

path_in - path of PDF to convert
dirname - path of temporary directory
"""
def convert_pdf(path_in, dirname):
    if run(PDF2PNG(path_in, dirname), stdout = DEVNULL, stderr = DEVNULL).returncode:
        rmtree(dirname)
        print(CANNOT_CONVERT)
        exit(BAD_MUSIC)

    if len(listdir(dirname)) != 1:
        if run(CONCAT_IMGS(dirname)).returncode:
            rmtree(dirname)
            print(CANNOT_CONCAT)
            exit(MAGICK_FAIL)
    else:
        rename(f"{dirname}/--1.png", WORKING)



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


"""
Send all pixels in the working PNG to either 0x000000 or 0xFFFFFF

tempdir - temporary directory holding working PNGs
"""
def heighten_contrast(tempdir):
    pass


def main():
    if len(argv) != CORRECT_ARGV:
        print(USAGE)
        exit(BAD_ARGV)

    path_in, path_out = tuple(argv[NO_PROGNAME:])
    dirname = gen_temp_dir(path_in)
    convert_pdf(path_in, dirname)
    heighten_contrast(dirname)

    #rmtree(dirname)

    return GOOD


if __name__ == "__main__":
    main()
