import sys

def encrypt(string):
    i=0
    out=""
    for c in string:
        power=pow(2,i)
        if power == 256:
            i=0
            power=pow(2,i)
        i+=1
        val=ord(c)
        k=val // power
        if k % 2 == 0:
            val+=power
        else:
            val-=power
        out+=str(hex(val)).replace('0x','')
        #print "Power:"+str(power)+" - Before:"+str(hex(ord(c)))+" - After:"+str(hex(val))
    return out
    
def decrypt(string):
    i=0
    k=0
    array=[]    
    l=len(string)-1
    while k < l:
        array.append(int(string[k:k+2],16))    
        k+=2
    out=""
    for c in array:
        power=pow(2,i)
        if power == 256:
            i=0
            power=pow(2,i)
        i+=1
        val=c
        k=val // power
        if k % 2 == 0:
            val+=power
        else:
            val-=power
        out+=chr(val)
        #print "Power:"+str(power)+" - Before:"+str(hex(c))+" - After:"+str(hex(val))+ " - Char:"+chr(val)
    return out

def main(args):
    if len(args)<3:
        print "Usage: "+args[0]+ " <e><d> " +" string"
        return
    if args[1] == 'd':
        o=decrypt(args[2])
    elif args[1] == 'e':
        o=encrypt(args[2])
    print o

if __name__=='__main__':
    main(sys.argv)
