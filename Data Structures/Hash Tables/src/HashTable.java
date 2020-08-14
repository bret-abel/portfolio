import java.util.ArrayList;

class HashTable<K, V> {
	private ArrayList<SequentialSearchSymbolTable> mainTable;
	
	/**
	 * Constructor for HashTable objects; it initializes the HashTable to have a given size.
	 * @param tableSize The size for the hash-table.
	 */
	public HashTable(int tableSize) {
		mainTable = new ArrayList<SequentialSearchSymbolTable>(tableSize);
		//this.tableSize = tableSize;
		//size = 0;
		
		for (int i = 0; i < tableSize; i++) {
			mainTable.add(new SequentialSearchSymbolTable());
		}
	}
	
	/**
	 * Clears the HashTable of all key/value pairs.
	 */
	public void clear() {
		for (int i = 0; i < mainTable.size(); i++) {
			mainTable.get(i).SSSTclear();
		}
	}
	
	/**
	 * Deletes a specific key/value pair.
	 * @param key The key to be deleted.
	 * @return True if succeeded, false if the key didn't exist to begin with.
	 */
	public boolean delete(K key) {
		SequentialSearchSymbolTable table = mainTable.get(convertToIndex(key, mainTable.size()));
		if (table.SSSTget(key) == null) return false;
		table.SSSTdelete(key);
		return true;
	}
	
	/**
	 * Returns the value attached to a given key.
	 * @param key The key to search for.
	 * @return The value attached to the key.
	 */
	public V get(K key) {
		//finds the right SSST by converting the key to index
		//then searches that SSST for the key
		return mainTable.get(convertToIndex(key, mainTable.size())).SSSTget(key);
	}
	
	/**
	 * Returns the number of values in the HashTable.
	 * @return The size of the HashTable.
	 */
	public int getSize() {
		int total = 0;
		for (int i = 0; i < mainTable.size(); i++) {
			total += mainTable.get(i).SSSTgetSize();
		}
		return total;
	}
	
	/**
	 * Adds a key/value pair to the HashTable.
	 * @param key The key to be added.
	 * @param value The value to be added with the key.
	 */
	public void put(K key, V value) {
		mainTable.get(convertToIndex(key, mainTable.size())).SSSTput(key, value);
		if (getAverageSSSTSize() > 2.0) rehash();
	}
	
	//converts a value's hashcode to a usable index.
	private int convertToIndex(K key, int size) {
		int hashCode = key.hashCode();
		hashCode = (Math.abs(hashCode)) % size;
		return hashCode;
	}
	
	/**
	 * A method to rehash the HashTable when it gets too big.
	 */
	private void rehash() {
		//The table should always have a prime number size
		int newTableSize = getNextPrime(mainTable.size());
		
		//makes the new table and fills it with SSSTs
		ArrayList<SequentialSearchSymbolTable> newMainTable = new ArrayList<SequentialSearchSymbolTable>(newTableSize);
		for (int i = 0; i < newTableSize; i++) {
			newMainTable.add(new SequentialSearchSymbolTable());
		}
		
		//Transfers values from old table to new
		for (int i = 0; i < mainTable.size(); i++) {
			SequentialSearchSymbolTable current = mainTable.get(i);
			for (int j = 0; j < current.SSSTgetSize(); j++) {
				K key = current.keys.get(j);
				V val = current.SSSTget(key);
				//puts the key and value from an SSST in the old table into a new SSST in the new table
				newMainTable.get(convertToIndex(key, newMainTable.size())).SSSTput(key, val);
			}
		}
		mainTable = newMainTable;
	}
	
	//Calculates the average number of key/value pairs per SSST
	private double getAverageSSSTSize() {
		return (double) ((double)getSize() / (double)mainTable.size());
	}
	
	//Finds the next prime number after a given prime number
	private int getNextPrime(int oldPrime) {
		int newPrime = oldPrime;
		
		do {
			newPrime += 2;
		}
		while (!isPrime(newPrime));
		return newPrime;
	}
	
	//Tests whether or not a number is prime
	private boolean isPrime(int n) {
		//check if multiple of 2
		if (n % 2 == 0) return false;
		//if not, then check odd numbers (no even numbers besides 2 are prime)
		//Use i*i because if (i^2 > n), n cannot be divisible by something larger than i
		//without also being divisible by something smaller than i, which is already
		//checked
		for (int i = 3; i*i <= n; i += 2) if (n % i == 0) return false;
		return true;
	}
	
	public static void main(String[] args) {
		HashTable<Integer, Integer> hashTable = new HashTable<Integer, Integer>(3);
		
		System.out.println("Putting things in hash table...");
		for (int i = 0; i < 20; i++) {
			hashTable.put(i, i);
		}
		System.out.println("Done");
		System.out.println("HashTable size (number of values): " + hashTable.getSize());
		System.out.println("Table size: " + hashTable.mainTable.size());
		
		System.out.println("Adding more stuff...");
		for (int i = 21; i < 40; i++) {
			hashTable.put(i, i);
		}
		System.out.println("Done");
		System.out.println("HashTable size (number of values): " + hashTable.getSize());
		System.out.println("Table size: " + hashTable.mainTable.size());
		
		System.out.println("Getting key '4'...");
		System.out.println("Value returned: " + hashTable.get(4));
		
		System.out.println("Clearing Table...");
		hashTable.clear();
		System.out.println("HashTable size (number of values): " + hashTable.getSize());
		System.out.println("Table size: " + hashTable.mainTable.size());
				
	}
	
	/////////////////////////////////////////////////////////////////
	//Private SSST class that serves the HashTable class
	/////////////////////////////////////////////////////////////////
	private class SequentialSearchSymbolTable {
		
		public ArrayList<K> keys;
		public ArrayList<V> values;
		public int size;
		
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
		public void SSSTclear() {
			keys.clear();
			values.clear();
			size = 0;
		}
		
		/**
		 * This method removes a key-value pair from the SSST.
		 * @param key The key to be deleted.
		 * @return True if successful, false if the key wasn't present in the SSST.
		 */
		public boolean SSSTdelete(K key) {
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
		public V SSSTget(K key) {
			if (!keys.contains(key)) return null;
			
			int i = keys.indexOf(key);
			return values.get(i);
		}
		
		/**
		 * Returns the size of the SSST.
		 * @return the size of the SSST.
		 */
		public int SSSTgetSize() {
			return size;
		}
		
		/**
		 * Adds a key-value pair to the SSST or replaces the value with a new one if the key
		 * is already present in the SSST.
		 * @param key The key to be added.
		 * @param value The value to be added.
		 */
		public void SSSTput(K key, V value) {
			if (!keys.contains(key)) {
				keys.add(key);
				values.add(value);
				size++;
			} else {
				int i = keys.indexOf(key);
				values.set(i, value);
			}
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////


}
