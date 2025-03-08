import os
import subprocess

CURRENT_PATH = os.path.abspath(os.path.dirname(__file__))
PATH_TO_EXECUTABLE = CURRENT_PATH + '/../../build/src/triangles'
PATH_TO_INPUT = CURRENT_PATH + '/input/'
PATH_TO_ANSWERS = CURRENT_PATH + '/ans/'

def inputFilePath(test_num):
  return PATH_TO_INPUT + f'test_{test_num}.in'

def ansFilePath(test_num):
  return PATH_TO_ANSWERS + f'ans_{test_num}.out'

def getAns(ans_file):
  with open(ans_file, 'r') as af:
    return af.read()

def test(test_num):
  fail = False

  for test_num in range(10):
    input_path = inputFilePath(test_num)
    ans_path = ansFilePath(test_num)
    with open(input_path, 'r') as input_file:
      process = subprocess.run(
        [PATH_TO_EXECUTABLE],
        stdin=input_file, text=True, capture_output=True
      )
      if process.returncode != 0:
        raise RuntimeError(f'Driver failed: {process.stderr}')
      output = process.stdout
      ref_output = getAns(ans_path)
      if (output != ref_output):
        print(f"Test {test_num} failed\n"
              f"Expected: {ref_output}\n"
              f"Actual:   {output}\n")
        fail = True
      else:
        print(f"Test {test_num} passed")

  if fail:
    raise RuntimeError("End-to-end test failed\n")

for i in range(10):
  test(i)
