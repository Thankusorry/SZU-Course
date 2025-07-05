import random
from random import randint

def genTree(point_num: int, weighted = False, weight_min = 1, weight_max = 10**5, one_indexed = False):
    res = []
    for i in range(1, point_num):
        u = randint(0, i - 1)
        if weighted:
            res.append([u, i, random_randint(weight_min, weight_max)])
        else:
            res.append([u, i])
    
    random.shuffle(res)
    if one_indexed:
        for edges in res:
            edges[0] += 1
            edges[1] += 1
    return res

def genGraph(point_num: int, edge_num: int, weighted = False, weight_min = 1, weight_max = 10**5, connected = False, one_indexed = False):
    res = genTree(point_num, weighted, weight_min, weight_max, False) if connected else []
    used_edges = set(tuple(edge) for edge in res)
    while len(used_edges) < edge_num:
        u = randint(0, point_num - 1)
        v = randint(0, point_num - 1)
        if u == v or (u, v) in used_edges: continue

        if weighted:
            res.append([u, v, randint(weight_min, weight_max)])
        else:
            res.append([u, v])
            used_edges.add((u, v))
    
    random.shuffle(res)
    if one_indexed:
        for edges in res:
            edges[0] += 1
            edges[1] += 1
    return res

def solve():
    n, m = map(int, input().split(' '))

    res = genGraph(n, m)
    print(n, m)
    for edges in res:
        print(edges[0], edges[1])

if __name__ == "__main__":
    solve()