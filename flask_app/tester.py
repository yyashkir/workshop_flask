#something to test running of the process, instead should invoke whatever C++ code you want
import time, sys
import argparse

parser = argparse.ArgumentParser(description='test stuff')

parser.add_argument('--x1', type=float, required=True)
parser.add_argument('--x2', type=float, required=True)
args = parser.parse_args()

print(f'X1^2+X2={args.x1**2+args.x2}')

    
