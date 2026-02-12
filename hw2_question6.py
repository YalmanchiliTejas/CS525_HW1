from collections import defautldict
import random
import matplotlib

import math

import argparse


'''

generates an adjacency list to get where each of the processors has k connections to randomly selected processors
'''
def generate_random_edges(p, k):

    visited = set()
    adj_list = defaultdict(list)  
    random.seed(0)
    edge_count = defaultdict(int)
    for v in p:
        edge_count[v] = 0

    for vertex in p:
        while edge_count[vertex] < k:

            need = k - edge_count[u]

            available = [p1 for p1 in p if p1 != vertex and edge_count[p1] < k ]

            if not available:
                break
            sampled = random.sample(available, min(need, len(available)))
            for v in sampled:
                if edge_count[u] < k and edge_count[v] < k and (vertex, v) not in visited:
                    adj[vertex].append(v)
                    adj[v].append(vertex)
                    edge_count[vertex] += 1
                    edge_count[v] += 1
                    visited.add((vertex, v))
                    visited.add((v, vertex))
    return adj_list



def generate_2d_mesh(p, k):
    total_p = math.sqrt(p) * math.sqrt(p)

    vertices = [i for i in range(total_p)]
    adj_list = generate_random_edges(total_p, k)
    return adj_list

def generate_3d_mesh(p, k)
    total_p =  int((p**(1/3)) * (p**(1/3)) * (p**(1/3)))

    vertices = [i for i in range(total_p)]
    adj_list = generate_random_edges(total_p, k)
    return adj_list

def generate_hypercube(p, k):

    exponent = int(2 * (p**(1/6)))

    total_p =  2**(exponent)

    vertices = [i for i in range(total_p)]
    adj_list = generate_random_edges(total_p, k)
    return adj_list







ef bfs_distances(adj, src, n=None):
    """Standard BFS shortest-path distances from src."""
    if n is None:
        n = len(adj)
    dist = [-1] * n
    dist[src] = 0
    q = deque([src])
    while q:
        u = q.popleft()
        for v in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                q.append(v)
    return dist


def estimate_diameter(adj, num_pairs=2000, seed=0):
    """
    Simulation-based diameter estimate:
    sample many random (s,t), compute shortest-path distance via BFS, take max.
    More efficient variant: BFS once per sampled source and query multiple targets.
    """
    random.seed(seed)
    n = len(adj)
    best = 0

    # Choose about sqrt(num_pairs) sources, and for each, sample multiple targets
    num_sources = max(1, int(num_pairs**0.5))
    targets_per_source = max(1, num_pairs // num_sources)

    for _ in range(num_sources):
        s = random.randrange(n)
        dist = bfs_distances(adj, s, n)

        for _ in range(targets_per_source):
            t = random.randrange(n)
            d = dist[t]
            if d > best:
                best = d

    return best


def estimate_bisection_width(adj, num_trials=200, seed=0):
    """
    Simulation-based bisection width estimate:
    sample many random equipartitions (A, V\A) with |A|=n/2,
    compute edge crossings, take min.
    """
    random.seed(seed)
    n = len(adj)
    half = n // 2
    best = None

    for _ in range(num_trials):
        A = set(random.sample(range(n), half))
        inA = [False] * n
        for u in A:
            inA[u] = True

        crossings = 0
        # Count each undirected edge once by counting edges from A to not-A
        for u in A:
            for v in adj[u]:
                if not inA[v]:
                    crossings += 1

        if best is None or crossings < best:
            best = crossings

    return 0 if best is None else best


if __name__ == "__main__":


    args_parser = argparse.ArgumentParser(description="simulation generator")

    args_parser.add_argument("-m", "--mesh", type=string,help="Choose between 2D, 3D, Hypercube", default="2D", required=True)
    args_parser.add_argument("-p", "--processors", type=int, help="Choose the p value", default=0, required=True)
    args_parser.add_argument("-k", "--connections", type=int, help="Choose the number of connetions for each processor", default=4, required=True)

    args = args_parser.parse_args()

    mesh = generate_mesh(args)
    graph = None
    if mesh == "2D":
        graph = generate_2d_mesh(args.p, args.k)
    elif mesh == "3D":
        graph = generate_3d_mesh(args.p, args.k)
    else:
        graph = generate_hypercube(args.p, args.k)
