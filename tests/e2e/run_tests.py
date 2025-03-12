import os
import subprocess
import glob

CURRENT_PATH = os.path.abspath(os.path.dirname(__file__))
PATH_TO_EXECUTABLE = CURRENT_PATH + '/../../build/driver/triangles'
PATH_TO_INPUT = [CURRENT_PATH + '/func/input/', CURRENT_PATH + '/bench/input']

def ansFilePath(input_path):
  return os.path.join((os.path.dirname(input_path)), os.path.pardir) + "/ans/" + (os.path.basename(input_path).replace('test_', 'ans_').replace('.in', '.out'))

def getAns(ans_file):
  with open(ans_file, 'r') as af:
    return af.read()

def test():
  fail = False

  for i in range(len(PATH_TO_INPUT)):
    for input_path in glob.glob(os.path.join(PATH_TO_INPUT[i], '*')):
      ans_path = ansFilePath(input_path)
      with open(input_path, 'r') as input_file:
        process = subprocess.run(
          [PATH_TO_EXECUTABLE],
          stdin=input_file, text=True, capture_output=True
        )
        if process.returncode != 0:
          raise RuntimeError(f'Driver failed on test {input_path}: {process.stderr}')
        output = process.stdout
        ref_output = getAns(ans_path)
        if (output != ref_output):
          print(f"Test {input_path} failed\n"
                f"Expected: {ref_output}\n"
                f"Actual:   {output}\n")
          fail = True
        else:
          print(f"Test {ans_path} passed")

  if fail:
    raise RuntimeError("End-to-end test failed\n")

test()
