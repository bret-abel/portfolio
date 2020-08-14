import java.io.*;
import java.util.Scanner;

class Animalpedia {
	
	public static void main(String[] args) {
		String filename = "animals.tsv"; //name of the file to be used
		final int INIT_SIZE = 11; //default size of the HashTables
		Scanner fileScanner = null; //scanner to use for reading files
		File file = new File(filename); //file to be read
		String userInput; //String containing user input
		Scanner user = new Scanner(System.in); //Scanner to read user input
		HashTable<String, String> animalsByType = new HashTable<String, String>(INIT_SIZE); //HashTable holding animals and their types
		HashTable<String, String> animalsByFeet = new HashTable<String, String>(INIT_SIZE); //HashTable holding animals and their number of feet
		
		System.out.println("Welcome to Animalpedia!");
		
		//attempts to initialize scanner to read file
		try {
			fileScanner = new Scanner(file);
		} catch (FileNotFoundException e) {
			System.err.println("File Not Found");
			e.printStackTrace();
		}
		
		//fills two HashTables with values from file
		while (fileScanner.hasNextLine()) {
			String[] holder = fileScanner.nextLine().split("\t");
			animalsByType.put(holder[0], holder[1]);
			animalsByFeet.put(holder[0], holder[2]);
		}
		
		System.out.println("Loaded \"" + filename + "\"");
		
		//Initially queries user
		System.out.println("Please enter the name of an animal or hit enter to quit:");
		System.out.print(">");
		
		userInput = user.nextLine();
		
		//Prints result of query to user and re-queries until they quit
		while(!userInput.equals("")) {
			if (animalsByType.get(userInput) != null) {
				System.out.println("The " + userInput + " is a " + animalsByType.get(userInput) + " that has " + animalsByFeet.get(userInput) + " feet.");
			}
			else System.out.println("Sorry, I don't know what that animal is!");
			
			System.out.println("Please enter the name of an animal or hit enter to quit: ");
			System.out.print(">");
			userInput = user.nextLine();
			
		}
		System.out.println("Goodbye!");
		
	}

}
