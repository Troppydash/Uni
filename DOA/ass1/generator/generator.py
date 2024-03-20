from scipy.spatial import ConvexHull
import numpy as np


def convex_hull(points):
    # convex hull, first node at the bottom
    hull = ConvexHull(points)
    hull_points = points[hull.vertices, :]

    # points are counterclockwise

    # find left most element
    left_most = None
    left_index = -1
    for i in range(hull_points.shape[0]):
        p = hull_points[i, :]
        if left_most is None or (
            p[0] < left_most[0] or p[0] == left_most[0] and p[1] < left_most[1]
        ):
            left_most = tuple(p)
            left_index = i

    bottom_most = None
    bottom_index = -1
    for i in range(hull_points.shape[0]):
        p = hull_points[i, :]
        if bottom_most is None or (
            p[1] < bottom_most[1] or p[1] == bottom_most[1] and p[0] < bottom_most[0]
        ):
            bottom_most = tuple(p)
            bottom_index = i

    # anchor based on left_index and bottom_index

    # uses the left_index
    jarvis_march = np.vstack([hull_points[left_index:], hull_points[:left_index]])
    graham_scan = np.vstack([hull_points[bottom_index:], hull_points[:bottom_index]])

    return jarvis_march, graham_scan

# returns a n-sample of 2d points
def sample(n: int):
    return np.random.randn(n, 2) * 100

def create_input(points):
    text = []

    n = points.shape[0]
    text.append(str(n))

    for row in range(n):
        text.append(f"{points[row][0]} {points[row][1]}")

    return '\n'.join(text)


def create_output(points):
    n = points.shape[0]

    # first in clockwise
    clockwise = ""
    for i in [0] + list(range(n-1, 0, -1)):
        clockwise += f"({points[i][0]:.6f}, {points[i][1]:.6f}) "

    # second in counterclockwise
    cclockwise = ""
    for i in range(0, n):
        cclockwise += f"({points[i][0]:.6f}, {points[i][1]:.6f}) "
    
    return clockwise + '\n' + cclockwise


def create_pair(name: str, n: int):
    points = sample(n)
    jarvis, graham = convex_hull(points)

    # write input
    input_name = f"{name}-input.txt"
    output_name_jarvis = f"{name}-jarvis.txt" 
    output_name_graham = f"{name}-graham.txt"

    with open(input_name, 'w') as f:
        f.write(create_input(points))
    
    with open(output_name_jarvis, 'w') as f:
        f.write(create_output(jarvis))

    with open(output_name_graham, 'w') as f:
        f.write(create_output(graham))

create_pair('test1', 100)
