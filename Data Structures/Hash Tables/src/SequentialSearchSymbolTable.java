import java.util.ArrayList;

/**
 * This class represents a Sequential-Search Symbol Table.
 * @author Bret Abel
 * @version 4/25/2017
 *
 * @param <K> Key connected to the Value
 * @param <V> Value connected to the Key
 */
class SequentialSearchSymbolTable<K,V> {
	
	private ArrayList<K> keys;
	private ArrayList<V> values;
	private int size;
	
	/**
	 * Constructor for a SequentialSearchSymbolTable object.
	 */
	public SequentialSearchSymbolTable() {
		keys = new ArrayList<K>();
		values = new ArrayList<V>();
		size = 0;
	}
	
	/**
	 * Clears all key-value pairs from the SSST.
	 */
	public void clear() {
		keys.clear();
		values.clear();
		size = 0;
	}
	
	/**
	 * This method removes a key-value pair from the SSST.
	 * @param key The key to be deleted.
	 * @return True if successful, false if the key wasn't present in the SSST.
	 */
	public boolean delete(K key) {
		if (!keys.contains(key)) return false;
		
		int i = keys.indexOf(key);
		values.remove(i);
		keys.remove(i);
		size--;
		return true;	
	}
	
	/**
	 * Returns the value associated with a given key.
	 * @param key The key to be entered.
	 * @return The value associated with the key.
	 */
	public V get(K key) {
		if (!keys.contains(key)) return null;
		
		int i = keys.indexOf(key);
		return values.get(i);
	}
	
	/**
	 * Returns the size of the SSST.
	 * @return the size of the SSST.
	 */
	public int getSize() {
		return size;
	}
	
	/**
	 * Adds a key-value pair to the SSST or replaces the value with a new one if the key
	 * is already present in the SSST.
	 * @param key The key to be added.
	 * @param value The value to be added.
	 */
	public void put(K key, V value) {
		if (!keys.contains(key)) {
			keys.add(key);
			values.add(value);
			size++;
		} else {
			int i = keys.indexOf(key);
			values.set(i, value);
		}
	}
	
	/**
	 * Returns a string representation of the SSST.
	 * @return A String representing the SSST.
	 */
	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		for(int i = 0; i < size; i++) {
			builder.append(keys.get(i));
			builder.append(": ");
			builder.append(values.get(i));
			builder.append("\n");
		}
		return builder.toString();
	}
	
	/////main method///////////////////////////////////////////////////////////
	public static void main(String[] args) {
		System.out.println("Testing SSST...");
		SequentialSearchSymbolTable<String, String> ssst = new SequentialSearchSymbolTable<String, String>();
		
		System.out.println("Adding key 'cow' with value 'moo' ...");
		String cow = "cow";
		String moo = "moo";
		ssst.put(cow, moo);
		System.out.println("Adding key 'pig' with value 'oink' ...");
		String pig = "pig";
		String oink = "oink";
		ssst.put(pig, oink);
		
		System.out.println("Testing key: 'cow'");
		System.out.println("Returned: " + ssst.get(cow));
		System.out.println("Size: " + ssst.getSize());
		System.out.println();
		System.out.println("Deleting 'cow'");
		ssst.delete(cow);
		System.out.println("Size: " + ssst.getSize());
		
		System.out.println("Clearing SSST...");
		ssst.clear();
		System.out.println("Size: " + ssst.getSize());
	}
}
