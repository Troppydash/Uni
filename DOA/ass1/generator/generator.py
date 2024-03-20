from pathlib import Path

from scipy.spatial import ConvexHull
import numpy as np
import subprocess
import matplotlib.pyplot as plt


def convex_hull(points):
    # convex hull, first node at the bottom
    hull = ConvexHull(points, qhull_options='Fc')  # for including collinear points
    hull_points = points[hull.vertices, :]

    # points are counterclockwise

    # find left most element
    left_index = np.argmin(hull_points, axis=0)[0]

    # find bottom most element
    bottom_index = np.argmin(np.flip(hull_points, axis=1), axis=0)[0]

    # anchor based on left_index and bottom_index
    jarvis_march = np.vstack([hull_points[left_index:], hull_points[:left_index]])
    graham_scan = np.vstack([hull_points[bottom_index:], hull_points[:bottom_index]])

    return jarvis_march, graham_scan


def sample(n: int):
    # returns a n-sample of 2d points in a circle
    return np.random.uniform(-1000, 1000, (n, 2))


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
    for i in [0] + list(range(n - 1, 0, -1)):
        clockwise += f"({points[i][0]:.6f}, {points[i][1]:.6f}) "

    # second in counterclockwise
    cclockwise = ""
    for i in range(0, n):
        cclockwise += f"({points[i][0]:.6f}, {points[i][1]:.6f}) "

    return clockwise + '\n' + cclockwise + '\n'


def create_pair(name: str, n: int, write: bool = False):
    points = sample(n)
    jarvis, graham = convex_hull(points)

    # write input
    input_name = f"output/{name}-input.txt"
    output_name_jarvis = f"output/{name}-jarvis.txt"
    output_name_graham = f"output/{name}-graham.txt"

    inp = create_input(points)
    jar = create_output(jarvis)
    gra = create_output(graham)

    if write:
        # create output dir if not exist
        Path('output').mkdir(exist_ok=True)

        with open(input_name, 'w') as f:
            f.write(inp)

        with open(output_name_jarvis, 'w') as f:
            f.write(jar)

        with open(output_name_graham, 'w') as f:
            f.write(gra)

    return inp, jar, gra


def write_error(n, inp, expected, got):
    with open('expected.txt', 'w') as f:
        f.write(expected)
    with open('input.txt', 'w') as f:
        f.write(inp)
    with open('got.txt', 'w') as f:
        f.write(got)

    inputs = np.transpose(parse_input(inp))
    plt.scatter(inputs[0], inputs[1], label='raw')

    expected = np.transpose(parse_output(expected))
    plt.plot(expected[0], expected[1], label='expected', c='red')

    got = np.transpose(parse_output(got))
    plt.plot(got[0], got[1], label='got', c='green')
    plt.legend()

    plt.title(f'n = {n}')
    plt.xlabel('x axis')
    plt.ylabel('y axis')

    plt.show()


def parse_input(inp: str):
    lines = inp.split('\n')
    number = int(lines[0])
    points = []
    for i in range(1, number + 1):
        x, y = lines[i].split()
        points.append([float(x), float(y)])

    return np.array(points)


def parse_output(out: str):
    line = out.split('\n')[0]
    coords = line.split(') ')
    points = []
    for coord in coords[:-1]:
        if coord[-2:] == ') ':
            coord = coord[:-2]

        x, y = coord[1:].split(', ')
        points.append([float(x), float(y)])

    return np.array(points)


def benchmark():
    jarvis_program = 'program'
    graham_program = 'graham'
    iterations = 10
    each_iter = 5000
    decay = 0.002
    upper = 20000

    for i in range(iterations):
        print(f'iteration {i}', flush=True)

        ns = np.arange(3, upper)
        weights = np.exp(-decay * ns) / np.sum(np.exp(-decay * ns))

        for it in range(each_iter):
            n = np.random.choice(ns, p=weights)
            print(f"n is {n:6}, remain {each_iter - it - 1:6}", end='\r', flush=True)

            name = f"iter-{i}-size-{n}"
            inp, jarvis, graham = create_pair(name, n)

            # run jarvis program
            output = subprocess.check_output([f'./{jarvis_program}'], input=inp.encode()).decode()
            if output != jarvis:
                print(f'mismatch at {name}, on jarvis')
                write_error(n, inp, jarvis, output)

            output = subprocess.check_output([f'./{graham_program}'], input=inp.encode()).decode()
            if output != graham:
                print(f'mismatch at {name}, on graham')
                write_error(n, inp, graham, output)

    print('\nall good!')


benchmark()
