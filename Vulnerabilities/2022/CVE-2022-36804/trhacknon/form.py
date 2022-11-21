import sys

try:
    with open(sys.argv[1]) as f:
        text = f.readlines()

    for i in text:
        if not "total" in i or "ip" in i and not '*' in i:
            string = i.replace('\n','')
            for i in range(10):
                string = string.replace('  ',' ')
            string = string.replace(' ', ':')
            l = len(string)
            print(string[:l-1])
except:
    print("Usage : python3 form.py /path/to/file.txt")
    sys.exit(1)
