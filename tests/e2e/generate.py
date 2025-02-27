import numpy as np
import trimesh

def generateRandomTriangle():
  vertices = np.random.uniform(size=(3, 3))
  return trimesh.Trimesh(vertices=vertices)

def printAnswer(file_path, triangles):
  output = set()
  for i, tri1 in enumerate(triangles):
    for j, tri2 in enumerate(triangles):
      if i != j and trimesh.intersections.mesh_intersection(tri1, tri2):
        output.add(i)
        output.add(j)

  ans_str = ' '.join([i for i in output])
  with open(file_path, 'w') as of:
    of.write(ans_str)
