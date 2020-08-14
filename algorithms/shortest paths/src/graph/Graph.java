package graph;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;

/**
 * @author Bret Abel
 * @author Harper Shapiro
 * @param <V>
 */
public class Graph<V> implements GraphIfc<V> {

	private HashMap<V, ArrayList<V>> adjList;
	private int size;

	public Graph(){
	    adjList = new HashMap<V,ArrayList<V>>();
	    size=0;
	}

    /**
	 * Returns the number of vertices in the graph
	 * @return The number of vertices in the graph
	 */
	public int numVertices(){
		return adjList.size();
	}
		
	/**
	 * Returns the number of edges in the graph
	 * @return The number of edges in the graph
	 */
	public int numEdges(){

		return size;
	}
	
	/**
	 * Removes all vertices from the graph
	 */
	public void clear(){
		adjList.clear();
		size=0;
	}
		
	/** 
	 * Adds a vertex to the graph. This method has no effect if the vertex already exists in the graph. 
	 * @param v The vertex to be added
	 */
	public void addVertex(V v){
		adjList.put(v,new ArrayList<V>());
	}
	
	/**
	 * Adds an edge between vertices u and v in the graph. 
	 * @param u A vertex in the graph
	 * @param v A vertex in the graph
	 * @throws IllegalArgumentException if either vertex does not occur in the graph.
	 */
	public void addEdge(V u, V v){
		adjList.get(u).add(v);//add v to u's adjacency list
		size++;
	}

	/**
	 * Returns the set of all vertices in the graph.
	 * @return A set containing all vertices in the graph
	 */
	public Set<V> getVertices(){
		return adjList.keySet();
	}
	
	/**
	 * Returns the neighbors of v in the graph. A neighbor is a vertex that is connected to
	 * v by an edge. If the graph is directed, this returns the vertices u for which an 
	 * edge (v, u) exists.
	 *  
	 * @param v An existing node in the graph
	 * @return All neighbors of v in the graph.
	 * @throws IllegalArgumentException if the vertex does not occur in the graph
	 */
	public List<V> getNeighbors(V v) throws Exception{
		if(!containsVertex(v)){
			throw new IllegalArgumentException("Vertex does not exist in graph");
		}
		return adjList.get(v);
	}

	/**
	 * Determines whether the given vertex is already contained in the graph. The comparison
	 * is based on the <code>equals()</code> method in the class V. 
	 * 
	 * @param v The vertex to be tested.
	 * @return True if v exists in the graph, false otherwise.
	 */
	public boolean containsVertex(V v){
		return adjList.keySet().contains(v);
	}
	
	/**
	 * Determines whether an edge exists between two vertices. In a directed graph,
	 * this returns true only if the edge starts at v and ends at u. 
	 * @param v A node in the graph
	 * @param u A node in the graph
	 * @return True if an edge exists between the two vertices
	 * @throws IllegalArgumentException if either vertex does not occur in the graph
	 */
	public boolean edgeExists(V v, V u) throws Exception{
		if(!containsVertex(v) || !containsVertex(u)){
			throw new IllegalArgumentException("Vertex does not exist in graph");
		}
		return adjList.get(v).contains(u);
	}

	/**
	 * Returns the degree of the vertex. In a directed graph, this returns the outdegree of the
	 * vertex. 
	 * @param v A vertex in the graph
	 * @return The degree of the vertex
	 * @throws IllegalArgumentException if the vertex does not occur in the graph
	 */
	public int degree(V v) throws Exception{
		return getNeighbors(v).size();
	}
	
	/**
	 * Returns a string representation of the graph. The string representation shows all
	 * vertices and edges in the graph. 
	 * @return A string representation of the graph
	 */
	public String toString(){
		StringBuilder builder = new StringBuilder();
		for(V k : adjList.keySet()){
			builder.append(k+": [");
			ArrayList<V> temp = adjList.get(k);
			for(int i=0;i<temp.size();i++){
				builder.append(temp.get(i));
				if(i<temp.size()-1) builder.append(", ");
			}
			builder.append("]\n");
		}
		return builder.toString();
	}

	public static void main(String[] args) throws Exception{
		Graph<Integer> graph = new Graph<Integer>();
		graph.addVertex(3);
		graph.addVertex(10);
		graph.addVertex(2);
		graph.addVertex(12);
		graph.addEdge(3,10);
		graph.addEdge(3,2);
		graph.addEdge(3,12);
		graph.addEdge(12,10);
		graph.addEdge(12,2);

		System.out.println(graph);
		System.out.println("#Vertices: " + graph.numVertices());
		System.out.println("#Edges " + graph.numEdges());

		//vertices
		System.out.print(" Vertices {");
		for(Integer v : graph.getVertices()){
			System.out.print(v+ " ");
		}
		System.out.println("}");

		//neighbors
		System.out.print("Neighbors of 3: ");
		for(Integer v: graph.getNeighbors(3)){
			System.out.print(v+" ");
		}

		System.out.println("Contains vertex 2? " + graph.containsVertex(2));
		System.out.println("Contains vertex 100? " + graph.containsVertex(100));

		System.out.println("Edge 3 to 12? " + graph.edgeExists(3,12));
		System.out.println("Edge 2 to 3? " +graph.edgeExists(2,3));

		System.out.println("Degree of 12: " + graph.degree(12));


	}
}