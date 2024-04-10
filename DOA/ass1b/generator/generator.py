import random
import subprocess
from math import floor

import networkx as nx
import matplotlib.pyplot as plt
import numpy as np


def custom_floor(num: float):
    delta = 0.00001
    if abs(round(num) - num) < delta:
        return int(round(num))
    return int(floor(num))


class TestCaseGenerator:

    def __init__(self):
        self.iterations = 10000

    def run_and_compare(self, program, inp, exp):
        output = subprocess.check_output([f'./{program}'], input=inp.encode()).decode()
        return output.strip() == exp.strip(), output

    def benchmark_A(self):
        print('running A')
        decay = 0.002
        upper = 2000
        program = 'parta'
        ns = np.arange(2, upper)
        weights = np.exp(-decay * ns) / np.sum(np.exp(-decay * ns))

        for i in range(self.iterations):
            n = np.random.choice(ns, p=weights)
            print(f'iter = {i:6d}, n = {n:6d}', end='\r')

            graph = self.generate_graph_A(n)

            res = np.random.choice(np.arange(n), 2, replace=False)
            start, end = int(res[0]), int(res[1])

            # solve
            inp = self.create_input_A(graph, start, end)
            expected = self.create_output(self.solve_A(graph, start, end))

            mat, got = self.run_and_compare(program, inp, expected)
            if not mat:
                print(f'mismatch at ./{program} where n = {n}')
                print(f'got {got.strip()}, expected {expected.strip()}')
                print(f"start = {start}, end = {end}")
                self.write_error(inp, got, expected)
                self.write_graph(graph)

                exit(0)

        print('\nall good!')

    def benchmark_B(self):
        print('running B')

        decay = 0.002
        upper = 2000
        program = 'partb'
        ns = np.arange(2, upper)
        weights = np.exp(-decay * ns) / np.sum(np.exp(-decay * ns))

        for i in range(self.iterations):
            n = np.random.choice(ns, p=weights)
            print(f'iter = {i:6d}, n = {n:6d}', end='\r')

            graph = self.generate_graph_B(n)

            res = np.random.choice(np.arange(n), 2, replace=False)
            start, end = int(res[0]), int(res[1])

            # solve
            inp = self.create_input_B(graph, start, end)
            expected = self.create_output(self.solve_A(graph, start, end))

            mat, got = self.run_and_compare(program, inp, expected)
            if not mat:
                print(f'mismatch at ./{program} where n = {n}')
                print(f'got {got.strip()}, expected {expected.strip()}')
                print(f"start = {start}, end = {end}")
                self.write_error(inp, got, expected)

                self.write_graph(graph)

                exit(0)

        print('\nall good!')

    def benchmark_C(self):
        print('running C')

        decay = 0.002
        upper = 2000
        program = 'partc'
        ns = np.arange(2, upper)
        weights = np.exp(-decay * ns) / np.sum(np.exp(-decay * ns))

        for i in range(self.iterations):
            n = np.random.choice(ns, p=weights)
            print(f'iter = {i:6d}, n = {n:6d}', end='\r')

            graph = self.generate_graph_B(n)

            res = np.random.choice(np.arange(n), 1, replace=False)
            start = int(res[0])

            # solve
            inp = self.create_input_C(graph, start)
            expected = self.create_output(self.solve_C(graph, start))

            mat, got = self.run_and_compare(program, inp, expected)
            if not mat:
                print(f'mismatch at ./{program} where n = {n}')
                print(f'got {got.strip()}, expected {expected.strip()}')
                print(f"start = {start}")
                self.write_error(inp, got, expected)

                self.write_graph(graph)

                exit(0)

        print('\nall good!')

    def benchmark_D(self):
        print('running D')

        decay = 0.02
        upper = 200
        program = 'partd'
        ns = np.arange(2, upper)
        weights = np.exp(-decay * ns) / np.sum(np.exp(-decay * ns))
        skipped = 0

        for i in range(self.iterations):
            n = np.random.choice(ns, p=weights)
            print(f'iter = {i:6d}, n = {n:6d}, skipped = {skipped:6d}', end='\r')

            graph = self.generate_graph_D(n)

            res = np.random.choice(np.arange(n), 2, replace=False)
            start, end = int(res[0]), int(res[1])

            inp = self.create_input_B(graph, start, end)

            # solve
            new_graph = graph.copy()
            for a, b in new_graph.edges:
                new_graph[a][b]['weight'] = np.log(1 + new_graph[a][b]['weight'] / 100)
            path_weight = custom_floor(np.exp(self.solve_A(new_graph, start, end)) * 100) - 100
            if path_weight > 2**31-1:
                # skip overflows
                skipped += 1
                continue

            expected = self.create_output(path_weight)

            mat, got = self.run_and_compare(program, inp, expected)
            if not mat:
                print(f'mismatch at ./{program} where n = {n}')
                print(f'got {got.strip()}, expected {expected.strip()}')
                print(f"start = {start}, end = {end}")
                self.write_error(inp, got, expected)

                self.write_graph(graph)

                exit(0)

        print('\nall good!')

    def solve_A(self, graph, start, end):
        path = nx.dijkstra_path(graph, start, end)
        return nx.path_weight(graph, path, 'weight')

    def solve_C(self, graph, start):
        T = nx.minimum_spanning_tree(graph)
        return int(T.size(weight='weight'))

    def generate_graph_A(self, vertices: int):
        graph = nx.connected_watts_strogatz_graph(vertices, min(vertices, 3), 0.6)
        for start, end in graph.edges:
            graph[start][end]['weight'] = 1

        return graph

    def generate_graph_B(self, vertices: int):
        graph = nx.connected_watts_strogatz_graph(vertices, min(vertices, 3), 0.6)
        for start, end in graph.edges:
            graph[start][end]['weight'] = random.randint(1, 100)

        return graph

    def generate_graph_D(self, vertices: int):
        graph = nx.connected_watts_strogatz_graph(vertices, min(vertices, 3), 0.6)
        for start, end in graph.edges:
            graph[start][end]['weight'] = random.randint(1, 200)

        return graph

    def create_input_A(self, graph: nx.Graph, start: int, end: int):
        text = []
        text.append(str(len(graph.nodes)))
        text.append(str(len(graph.edges)))
        text.append(str(start))
        text.append(str(end))

        for edge in graph.edges:
            text.append(f"{edge[0]} {edge[1]}")

        return '\n'.join(text)

    def create_input_B(self, graph: nx.Graph, start: int, end: int):
        text = []
        text.append(str(len(graph.nodes)))
        text.append(str(len(graph.edges)))
        text.append(str(start))
        text.append(str(end))

        for edge in graph.edges:
            text.append(f"{edge[0]} {edge[1]} {graph[edge[0]][edge[1]]['weight']}")

        return '\n'.join(text)

    def create_input_C(self, graph: nx.Graph, start: int):
        text = []
        text.append(str(len(graph.nodes)))
        text.append(str(len(graph.edges)))
        text.append(str(start))

        for edge in graph.edges:
            text.append(f"{edge[0]} {edge[1]} {graph[edge[0]][edge[1]]['weight']}")

        return '\n'.join(text)

    def create_output(self, result: int):
        return str(result) + '\n'

    def write_error(self, inp, got, expected):
        with open('input.txt', 'w') as f:
            f.write(inp)

        with open('got.txt', 'w') as f:
            f.write(got)

        with open('expected.txt', 'w') as f:
            f.write(expected)

    def write_graph(self, graph):
        plt.figure()
        pos = nx.spring_layout(graph)
        nx.draw_networkx(graph, pos, with_labels=True)
        labels = nx.get_edge_attributes(graph, 'weight')
        nx.draw_networkx_edge_labels(graph, pos, edge_labels=labels)
        plt.show()


gen = TestCaseGenerator()
gen.benchmark_A()
gen.benchmark_B()
gen.benchmark_C()
gen.benchmark_D()
