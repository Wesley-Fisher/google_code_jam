#!/usr/bin/python3

import os
import math
import random
import numpy as np


NQ = 10000
NS = 100

# Look at different stats for bad, med, and good students
ch = [0, 49, 99]

print()

with open(os.path.dirname(__file__) + '/../tests/gen_test.txt', 'w') as output:
    output.write('1\n')
    output.write('0\n')

    for i in range(0, NS):
        Si = (i / float(NS)) * 6 - 3.0
        key = ""

        for j in range(0, NQ):
            Qj = (j / float(NQ)) * 6 - 3.0

            ans = ""

            if i % 10 == 0 and random.choice([1,2]) == 1:
                ans = "1"
            else:
                p = 1.0 / (1.0 + math.exp(-(Si-Qj)))
                x = random.uniform(0.0, 1.0)

                if x < p:
                    ans = "1"
                else:
                    ans = "0"
            key = key + ans
        
        key = key + "\n"
        output.writelines(key)
