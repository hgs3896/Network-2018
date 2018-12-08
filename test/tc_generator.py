import random as r
import os, sys
import subprocess as sp
import time

TESTSIZE = 100000
NUM_CASES = 10

def makeTestCase(sz):
    arr = []
    for _ in range(sz):
        arr.append(str(r.randrange(1, 10000)))
    return '\n'.join(arr)

def testProgram():
    serverpath = "./server/server"
    clientpath = "./client/client"
    server = sp.Popen(serverpath)
   
    path1 = "./test/testcases/"
    path2 = "./"
    name  = "testfile%d.txt"

    for _ in range(NUM_CASES):
        print("TESTCASE #%d"%(_ + 1))
        filepath = (path1+name)%(_)
        fd = os.open(filepath, os.O_RDWR | os.O_CREAT )
        os.write(fd, makeTestCase(TESTSIZE).encode('utf-8'))
        os.close(fd)

        client = sp.Popen([clientpath, filepath])
        client.wait()

    time.sleep(5)
    server.terminate()
    
    for _ in range(NUM_CASES):
        filename = name%(_)
        p = sp.Popen(["diff", path1+filename, path2+filename])
        p.wait()

testProgram()
