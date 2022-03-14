def dijkstra(graph, src, dst, distances, parent, visited):
    visited[src] = True

    if src == dst:
        print 'done'
    else:
        neighbors = graph[src]
        for neighbor, distance in neighbors.iteritems():
            if visited[neighbor] == False:
                route = distances[src] + distance
                if route < distances[neighbor]:
                    distances[neighbor] = route
                    parent[neighbor] = src

        least = float('inf')
        for x in distances:
            if visited[x] != True and distances[x] < least:
                least = distances[x]
                src = x

        dijkstra(graph, src, dst, distances, parent, visited)

def start(graph, src, dst):
    distances = {}
    parent = {}
    visited = {}

    for v in graph:
        distances[v] = float('inf')
        parent[v] = None
        visited[v] = False

    distances[src] = 0
    dijkstra(graph, src, dst, distances, parent, visited)
    return [parent, distances]