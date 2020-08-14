package graph;

import util.PriorityQueue;
import util.Pair;
import java.util.Arrays;
import java.util.HashMap;

public class GraphAlgorithms {

    /**
     * Returns the shortest paths between vertices in a graph.
     * 
     * @param graph The graph to find paths for
     * @return 2D array holding shortest paths
     */
    public static int[][] floydWarshall(Graph<Integer> graph) {
        int V = graph.numVertices();
        int INF = 99999;
        // did this as opposed to Integer.MAX_VALUE to avoid
        // spilling over to 0 when comparing distance

        // make a 2D array to hold the distances and initialize to infinite
        int[][] dist = new int[V][V];
        for (int[] row : dist) {
            Arrays.fill(row, INF);
        }

        // Since we have no weights, reinitialize distances
        // using existence of edges in graph
        int src = 0;
        int dest = 0;
        try {
            for (Integer v : graph.getVertices()) {
                for (Integer u : graph.getVertices()) {
                    if (graph.edgeExists(v, u)) {
                        dist[src][dest] = 1;
                    }
                    dest++;
                }
                src++;
            }
        } catch (Exception e) {
            System.out.println("Invalid vertex encountered in Flloyd-Warshall.");
            return null;
        }

        // Generate shortest paths by comparing intermediate vertices
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                // optimization: ignore nodes that aren't connected by an edge
                if (dist[i][j] == INF)
                    continue;
                for (int k = 0; k < V; k++) {
                    if (dist[i][j] + dist[j][k] < dist[i][k]) {
                        dist[i][k] = dist[i][j] + dist[j][k];
                    }
                }
            }
        }
        return dist;
    }

    /**
     * Returns the shortest paths from source to other vertices in graph.
     * 
     * @param graph  graph to analyze
     * @param source the starting vertex of the path
     * @return mapping of a vertex to its predecessor on shortest path from source
     */
    public static HashMap<Integer, Integer> dijkstrasAlgorithm(Graph<Integer> graph, int source) {
        PriorityQueue Q = new PriorityQueue();
        HashMap<Integer, Integer> dist = new HashMap<>(); // stores best distance from s to another node
        HashMap<Integer, Integer> prev = new HashMap<>(); // stores prev node on shortest path
        // int[] dist = new int[graph.numVertices()];
        // int[] prev = new int[graph.numVertices()];

        // Set all dists to infinity, all prevs to null
        for (Integer v : graph.getVertices()) {
            dist.put(v, Integer.MAX_VALUE);
            prev.put(v, null); // -1 represents null (no path predecessor for vertex v yet)
        }

        dist.put(source, 0); // starting node

        for (Integer v : graph.getVertices()) {
            System.out.println("Adding vertex " + v + " with priority " + dist.get(v));
            Q.push(dist.get(v), v);
        }
        
        Pair u; // stores current vertex
        Integer uElement;

        while (!Q.isEmpty()) {
            u = Q.pop(); // shortest distance to u
            uElement = (Integer) u.element;

            try {
                for (Integer v : graph.getNeighbors(uElement)) {
                    Integer alt = dist.get(uElement) + 1; // Is this better than what we've found so far? (weight from u
                                                          // to v is 1)
                    if (alt < dist.get(v)) {
                        dist.put(v, alt);
                        prev.put(v, uElement);
                        Q.changePriority(alt, v);
                    }
                }
            } catch (Exception e) {
                System.out.println("Invalid vertex encountered in Dijkstra's.");
                return null;
            }
        }
        return prev;
    }

    public static void main(String[] args) {
        // test algs
        Graph<Integer> dtest = new Graph<Integer>();
        dtest.addVertex(5);
        dtest.addVertex(16);
        dtest.addVertex(12);
        dtest.addVertex(8);

        dtest.addEdge(5, 16);
        dtest.addEdge(16, 12);
        dtest.addEdge(12, 8);
        dtest.addEdge(5, 8);
        // dtest.addEdge(16,8);
        // dtest.addEdge(8,12);

        HashMap<Integer, Integer> prev = dijkstrasAlgorithm(dtest, 5);
        for (Integer v : prev.keySet()) {
            System.out.println("vertex: " + v + " previous: " + prev.get(v));
        }

        int[][] dists = floydWarshall(dtest);
        System.out.println(Arrays.toString(dists));

    }
}
