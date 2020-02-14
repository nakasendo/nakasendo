#!/usr/bin/python3
import subprocess
import shlex
def main():
    rawargs = 'git log --pretty=oneline --grep "SL-[0-9][0-9]"'
    args = shlex.split(rawargs)
    out = subprocess.check_output(args)
    notes = out.split(b"\n")
    with open("release-notes.txt", "w") as rn:
        for x in notes:
            print(x, file=rn)

if __name__ == '__main__':
    main()
