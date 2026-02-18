from collections import defaultdict, deque
import random
import matplotlib.pyplot as plt

import math
import numpy as np
import argparse




#generates an adjacency list to get where each of the processors has k additional connections to randomly selected processors

#same code as given in the assignment handout

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


#generates a 2d mesh and then later on adds the extra k random edges
def generate_2d_mesh(p, k):
    
    proc = int(round(math.sqrt(p)))



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

#generates an id to a processor in the 3d mesh
def generate_3d_id(x, y, z, width):
    return x*width*width + y*width + z

#generates a 3d mesh and then later on adds the extra k random edges
def generate_3d_mesh(p, k):
    proc = int(round(p ** (1/3)))

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


#generates a hypercube and then later on adds the extra k random edges
def generate_hypercube(p, k):
    
    sixth_root = int(round(p ** (1/6)))

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






#generates the bfs along with the traversal order and parents array (helpful for congestion)
def bfs_distances(adj, src, n=None):
  
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


#estimates the diameter by sampling many source and destination pairs and then taking the max of those BFS distances
def estimate_diameter(adj, num_pairs=10000, seed=0):

    random.seed(seed)
    n = len(adj)
    best = 0

    # Choose about sqrt(num_pairs) sources, and for each, sample multiple targets
    num_sources = max(1, int(num_pairs**0.5))
    targets_per_source = max(1, num_pairs // num_sources)
  
    
    for _ in range(num_sources):
        s = random.randrange(n)
        #calculate BFS distances once per every source node(more efficient that way)
        dist, _ , _= bfs_distances(adj, s, n)

        for _ in range(targets_per_source):
            t = random.randrange(n)
            d = dist[t]
            if d > best:
                best = d
    
    return best

# estimate the bisection width by continuously sampling and partitioning into two different sets and then taking the min the edges crossing between the two partitions
def estimate_bisection_width(adj, num_trials=10000, seed=0):
 
    random.seed(seed)
    n = len(adj.keys())
    half = n // 2
    np.random.seed(seed)


    V = []
    U = []

    for k, v in adj.items():
        for vertex in v:
            if k < vertex:
                U.append(k)
                V.append(vertex)
    U = np.array(U,dtype=np.int32) # convert to nump arrays
    V = np.array(V,dtype=np.int32) # convert to numpy arrays

    width = float("inf")
    for i in range(num_trials):
        side = np.zeros(n)
        A = np.random.choice(n, size=half, replace=False) # partition it into a subset
        side[A] = 1
        
        num_crossings = int(np.count_nonzero(side[U] != side[V])) # find the number of crossing and then take a min

        width = min(width, num_crossings)
    return width if width < float("inf") else 0
    





# estimates dilation and congestion
def estimate_dilation_congestion(adj_a, adj_b,):

    

    dilation = 0

    load = defaultdict(int) # the dictionary to calculate the load on the edges
    n = len(adj_a)

    for s in range(len(adj_a)):
        distance, parents, order = bfs_distances(adj_b, s, len(adj_a)) #do a BFS once per every source (much more efficient)
        demand = [0] * n  # the number of times a route passes through a vertex
        for t in adj_a[s]:
            if s < t:
                d = distance[t]
                if d == -1:
                    continue
                dilation = max(dilation, d) # takes the max distance from the source to all neighbors 
                demand[t] += 1
        #Iterates up the traversal order to see which edge got most called and store those values. the max of them will be returned
        for u in reversed(order):
            p = parents[u]
            if p != -1:
                if demand[u]:
                    edges = (p, u) if p  < u else (u, p)
                    load[edges] += demand[u]
                    demand[p] += demand[u]
    return dilation, max(load.values()) if load else 0



if __name__ == "__main__":


    args_parser = argparse.ArgumentParser(description="simulation generator")

    args_parser.add_argument("-m", "--mesh", type=str,help="Choose between 2D, 3D, Hypercube", default="2D", required=False)
    args_parser.add_argument("-p", "--processors", type=int, help="Choose the p value", default=0, required=False)
    args_parser.add_argument("-k", "--connections", type=int,dest="k", help="Choose the number of connetions for each processor", default=4, required=False)
    args_parser.add_argument("-n", "--network_mapping",  help="network_mapping",action="store_true",  required=False)
    args_parser.add_argument("-all", "--all_meshes", help="Run for all meshes for diameter and bisection width",action="store_true",required=False)
    args_parser.add_argument("-p4", "-part_4", help="part_4 code",action="store_true",required=False)

    processors = [2**6, 3**6, 4**6, 5**6, 6**6]
   
    args = args_parser.parse_args()

    graph = None
    diameter_val = defaultdict(list)
    bisection_val = defaultdict(list)

    dilation_vals = []
    congestion_vals = []

    if args.p4:
        graph_2d = generate_2d_mesh(args.processors, args.k)
        graph_3d = generate_3d_mesh(args.processors, args.k)
        print(f"mesh 2d has Bisection: {estimate_bisection_width(graph_2d)}\n")
        print(f"Mesh 3d has Bisection:{estimate_bisection_width(graph_3d)}\n")

    else:


        if not args.all_meshes:
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




    

        


    
