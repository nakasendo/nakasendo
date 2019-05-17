
#!/usr/bin/python3
import subprocess
import os
def main():
    args = ['python3', './test_messageHashAPI_test.py']
    r1 = subprocess.call(args)
    args = ["python3","./test_bigNumberAPI_Test.py"]
    r2 = subprocess.call(args)
    exit(r1 + r2)

if __name__ == '__main__':
    main()
