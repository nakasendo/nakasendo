#!/usr/bin/python3
import subprocess
def main():
    r1 = subprocess.call("build/x64/release/test_AsymKey")
    r2 = subprocess.call("build/x64/release/test_BigNumbers")
    r3 = subprocess.call("build/x64/release/test_MessageHash")
    r4 = subprocess.call("build/x64/release/test_SymEncDec")
    exit(r1 + r2 + r3 + r4)

if __name__ == '__main__':
    main()
