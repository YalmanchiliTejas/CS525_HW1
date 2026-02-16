from collections import defaultdict, deque
import random
import matplotlib.pyplot as plt

import math
import numpy as np
import argparse


'''

generates an adjacency list to get where each of the processors has k additional connections to randomly selected processors
'''


def generate_random_edges(adj, k):

    V = list(adj.keys())

    E = set()

    random.seed(0)

    for vertex in V:

        for w in adj[vertex]:

            edge = (vertex, w) if vertex < w else (w, vertex)

            E.add(edge)
    
    E_prime = set()
    V_prime = []
    temp_k = 0

    for vertex in V:
        temp_k = k
        neighbors = set(adj[vertex])
        while temp_k != 0:
            
            remaining = [x for x in V if x != vertex and x not in neighbors]

            sample_size = min(len(remaining), temp_k)

            V_prime = random.sample(remaining, sample_size)

            for w in V_prime:

                edge = (vertex,w) if vertex < w else (w, vertex)

                if edge not in E:
                    E_prime.add(edge)
                
            for (v, w) in E_prime:
                E.add((v, w))
                adj[v].append(w)
                adj[w].append(v)
            
            temp_k -= len(E_prime)
            E_prime.clear()
            V_prime = []
    return adj


#generate a number to a processor
def generate_2d_id(x, y, width):

    return  x*width+ y
def generate_2d_mesh(p, k):
    
    proc = int(math.sqrt(p))

    adj = defaultdict(list)

    
    for p1 in range(proc):
        for p2 in range(proc):
            v1 = generate_2d_id(p1, p2, proc)

            #check if there can be any down neighbors
            if p1 + 1 < proc:
                v2 = generate_2d_id(p1 + 1, p2, proc)
                adj[v1].append(v2)
                adj[v2].append(v1)
            #check if there can be any neighbor to right
            if p2 + 1 < proc:
                v2 = generate_2d_id(p1, p2 + 1, proc)
                adj[v1].append(v2)
                adj[v2].append(v1)
    return generate_random_edges(adj, k)

def generate_3d_id(x, y, z, width):
    return x*width*width + y*width + z

def generate_3d_mesh(p, k):
    proc = int(p ** (1/3))
    adj = defaultdict(list)

    for p1 in range(proc):
        for p2 in range(proc):
            for p3 in range(proc):
                v1 = generate_3d_id(p1, p2, p3, proc)

                #check if there can be any right neighbors
                if p1 + 1 < proc:
                    v2 = generate_3d_id(p1 + 1, p2, p3, proc)
                    adj[v1].append(v2)
                    adj[v2].append(v1)
                #check if there can be any neighbor below
                if p2 + 1 < proc:
                    v2 = generate_3d_id(p1, p2 + 1, p3, proc)
                    adj[v1].append(v2)
                    adj[v2].append(v1)
                #check if there can be any neighbor in the z direction
                if p3 + 1 < proc:
                    v2 = generate_3d_id(p1, p2, p3 + 1, proc)
                    adj[v1].append(v2)
                    adj[v2].append(v1)
    return generate_random_edges(adj, k)


def generate_hypercube(p, k):

    sixth_root = int(p ** (1/6))
    dimension = 2 * sixth_root
    total_nodes = 2**(dimension)

    adj = defaultdict(list)

    for i in range(total_nodes):
        for d in range(dimension):
            neighbor = i ^ (1 << d)
            
            if i < neighbor:
                adj[i].append(neighbor)
                adj[neighbor].append(i)
    return generate_random_edges(adj, k)







def bfs_distances(adj, src, n=None):
    """Standard BFS shortest-path distances from src."""
    if n is None:
        n = len(adj)
    dist = [-1] * n
    dist[src] = 0
    parent = [-1] * n
    q = deque([src])
    order = []
    while q:
        u = q.popleft()
        order.append(u)
        for v in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                parent[v] = u
                q.append(v)
    return dist, parent, order


def estimate_diameter(adj, num_pairs=10000, seed=0):
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
        dist, _ , _= bfs_distances(adj, s, n)

        for _ in range(targets_per_source):
            t = random.randrange(n)
            d = dist[t]
            if d > best:
                best = d
    
    return best


def estimate_bisection_width(adj, num_trials=10000, seed=0):
    """
    using native python
    """
    random.seed(seed)
    n = len(adj.keys())
    half = n // 2
    np.random.seed(seed)
    # vertices = list(adj.keys())
    # best = None

    # for _ in range(num_trials):
    #     A = set(random.sample(vertices, half))
    #     inA = [False] * n
    #     for u in A:
    #         inA[u] = True

    #     crossings = 0
    #     # Count each undirected edge once by counting edges from A to not-A
    #     for u in A:
    #         for v in adj[u]:
    #             if not inA[v]:
    #                 crossings += 1

    #     if best is None or crossings < best:
    #         best = crossings

    # return 0 if best is None else best

    V = []
    U = []

    for k, v in adj.items():
        for vertex in v:
            if k < vertex:
                U.append(k)
                V.append(vertex)
    U = np.array(U,dtype=np.int32)
    V = np.array(V,dtype=np.int32)

    width = float("inf")
    for i in range(num_trials):
        side = np.zeros(n)
        A = np.random.choice(n, size=half, replace=False)
        side[A] = 1
        
        num_crossings = int(np.count_nonzero(side[U] != side[V]))

        width = min(width, num_crossings)
    return width if width < float("inf") else 0
    






def estimate_dilation_congestion(adj_a, adj_b,):

    

    dilation = 0

    load = defaultdict(int)
    n = len(adj_a)

    for s in range(len(adj_a)):
        distance, parents, order = bfs_distances(adj_b, s, len(adj_a))
        demand = [0] * n
        for t in adj_a[s]:
            if s < t:
                d = distance[t]
                if d == -1:
                    continue
                dilation = max(dilation, d)
                demand[t] += 1
                # current = t
                # while current != s:
                #     parent = parents[current]
                #     if parent == -1:
                #         break
                #     edges = (parent, current) if parent < current else (current, parent)
                #     load[edges] += 1
                #     current = parent
        for u in reversed(order):
            p = parents[u]
            if p != -1:
                if demand[u]:
                    edges = (p, u) if p  < u else (u, p)
                    load[edges] += demand[u]
                    demand[p] += demand[u]
    return dilation, max(load.values()) if load else 0


def plot_and_save_multiline(x,val_dict, x_label, y_label, title, filename):

    plt.figure()
    for key , value in val_dict.items():
        plt.plot(x, value, marker="o", label=key)
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.title(title)
    plt.grid(True)
    plt.legend()
    plt.savefig(filename)
    plt.close()

def plot_and_save_single(x, y, x_label, y_label, title, filename):

    plt.figure()
    plt.plot(x, y, marker="o")
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.title(title)
    plt.grid(True)
    plt.savefig(filename)
    plt.close()


def plot_diameter_bisection(diameter_dict, bisection_dict, processors):
    
    plot_and_save_multiline(processors, diameter_dict, "Number of Processors", "Diameter", "Diameter vs Number of Processors", "diameter.png")
    plot_and_save_multiline(processors, bisection_dict, "Number of Processors", "Bisection Width", "Bisection Width vs Number of Processors", "bisection.png")

def plot_dilation_congestion(dilation_vals, congestion_vals, processors):
    plot_and_save_single(processors, dilation_vals, "Number of Processors", "Dilation", "Dilation vs Number of Processors", "dilation.png")
    plot_and_save_single(processors, congestion_vals, "Number of Processors", "Congestion", "Congestion vs Number of Processors", "congestion.png")



if __name__ == "__main__":


    args_parser = argparse.ArgumentParser(description="simulation generator")

    args_parser.add_argument("-m", "--mesh", type=str,help="Choose between 2D, 3D, Hypercube", default="2D", required=False)
    # args_parser.add_argument("-p", "--processors", type=int, help="Choose the p value", default=0, required=True)
    args_parser.add_argument("--k", "--connections", type=int,dest="k", help="Choose the number of connetions for each processor", default=4, required=False)
    args_parser.add_argument("-n", "--network_mapping",  help="network_mapping",action="store_true",  required=False)
    args_parser.add_argument("-all", "-all_meshes", help="Run for all meshes for diameter and bisection width",action="store_true",required=False)

    processors = [2**6, 3**6, 4**6, 5**6, 6**6]
   
    args = args_parser.parse_args()

    graph = None
    diameter_val = defaultdict(list)
    bisection_val = defaultdict(list)

    dilation_vals = []
    congestion_vals = []
    if not args.all:
        for i, p in enumerate(processors):
            if  not args.network_mapping:
                if args.mesh == "2D":
                    graph = generate_2d_mesh(p, args.k)
                elif args.mesh == "3D":
                    graph = generate_3d_mesh(p, args.k)
                else:
                    graph = generate_hypercube(p, args.k)
                
                diameter_val[args.mesh].append(estimate_diameter(graph))
                bisection_val[args.mesh].append(estimate_bisection_width(graph))
            else:
                map_a = generate_2d_mesh(p, args.k)
                map_b = generate_3d_mesh(p, args.k)
                dilation, congestion = estimate_dilation_congestion(map_a, map_b)
                dilation_vals.append(dilation)
                congestion_vals.append(congestion)
                print(f"The dilation: {dilation_vals[-1]} and congestion_vals: {congestion_vals[-1]} for this processor index:{i}")
        
    else:
        meshes = ["2D", "3D", "Hypercube"]
        for mesh in meshes:
            for i,  p in enumerate(processors):
                if mesh == "2D":
                    graph = generate_2d_mesh(p, args.k)
                elif mesh == "3D":
                    graph = generate_3d_mesh(p, args.k)
                else:
                    graph = generate_hypercube(p, args.k)
                
                diameter_val[mesh].append(estimate_diameter(graph))
                print(f"mesh {mesh} has diameter: {diameter_val[mesh][-1]} for this processor index: {i}")
                bisection_val[mesh].append(estimate_bisection_width(graph))
                print(f"mesh{mesh} has bisection: {bisection_val[mesh][-1]} for this processor index: {i}")
    
    if args.network_mapping:
        plot_dilation_congestion(dilation_vals, congestion_vals, processors)
    else:
        plot_diameter_bisection(diameter_val, bisection_val, processors)
        print("dimatert and bisection plotting done")


    

        


    
