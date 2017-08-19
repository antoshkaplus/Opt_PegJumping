#!python

import os
import sys
from subprocess import Popen, PIPE

errwrite = lambda x: sys.stderr.write(x + "\n")

temp_dir = "./../temp/"
executable = "./../bin/solver"

if not os.path.isdir(temp_dir):
    os.mkdir(temp_dir)

# where problem goes
input_name = temp_dir + "input.txt"
# where output of the problem goes
output_name = temp_dir + "output.txt"
# different stuff goes
log_name = temp_dir + "log.txt"

input = open(input_name, "w+")

errwrite("reading input")

n = int(raw_input())
input.write(str(n) + "\n");
for i in range(n):
    input.write(raw_input() + "\n")
n = int(raw_input())
input.write(str(n) + "\n");
for i in range(n):
    input.write(raw_input() + "\n")


errwrite("solving problem")

output = open(output_name, "w+")
input.seek(0)
args = {"stdout" : output, "stderr" : sys.stderr, "stdin" : input}
Popen([executable], 
      **args).wait()

input.close()

output.seek(0)
s = output.read()
print s
sys.stderr.write(s)
#sys.stdout.flush()
output.close()