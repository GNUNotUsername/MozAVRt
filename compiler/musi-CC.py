"""
Compile a sheet music PNG into machine interpretable pitch and duration data

Usage:  python musi-CC.py in-path out-path
"""


from sys import argv


# Cliargs
CORRECT_ARGV    = 3
NO_PROGNAME     = 1

# Exit Codes
GOOD            = 0
BAD_ARGV        = 1


def main():
    if len(argv) != CORRECT_ARGV:
        exit(BAD_ARGV)

    path_in, path_out = tuple(argv[NO_PROGNAME:])

    return GOOD


if __name__ == "__main__":
    main()
