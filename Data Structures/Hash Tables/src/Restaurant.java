import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.Scanner;

/**
 * This class represents a restaurant. The user can order from a menu and
 * the program tracks the subtotal of their meal using a SSST.
 * @author Bret Abel
 * @version 4/25/2017
 *
 */
class Restaurant {
	
	private int subtotal;
	private int total;
	private static final double TAX = 0.1;
	private static final double TIP = 0.15;
	
	/**
	 * Calculates total value of the meal
	 * @return
	 */
	public int calculateTotal() {
		total = subtotal + (int)(subtotal * TAX) + (int)(subtotal * TIP);
		return total;
	}
	
	/**
	 * Calculates the tax value
	 * @return the tax
	 */
	public int getTax() {
		return (int) (subtotal * TAX);
	}
	
	/**
	 * Calculates the tip value
	 * @return the tip 
	 */
	public int getTip() {
		return (int) (subtotal * TIP);
	}
	
	/**
	 * formats an amount of money in cents in dollars
	 * @param cents An amount of money in cents
	 * @return The same amount of money in dollars as a String
	 */
	public String formatAsDollars(int cents) {
		int dollars = cents / 100;
		cents = cents - (dollars * 100);
		String payment = String.format("$%d.%02d", dollars, cents);
		return payment;
	}
	
	/**
	 * Adds a cost to the subtotal
	 * @param price the price of the item to add
	 */
	public void add(int price) {
		subtotal += price;
	}
	
	/**
	 * returns subtotal of the order
	 * @return the subtotal
	 */
	public int getSubtotal() {
		return subtotal;
	}
	
	////main method///////////////////////////////////////////////////////////
	
	public static void main(String[] args) {
		Restaurant bigBoy = new Restaurant();
		Scanner fileScanner = null;
		Scanner user = new Scanner(System.in);
		File menuFile = new File("menu.tsv");
		SequentialSearchSymbolTable<String, Integer> menu = new SequentialSearchSymbolTable<String, Integer>();
		
		try {
			fileScanner = new Scanner(menuFile);
		} catch (FileNotFoundException e) {
			System.err.println("File not fond.");
			e.printStackTrace();
		}
		
		while (fileScanner.hasNextLine()) {
			String[] holder = fileScanner.nextLine().split("\t");
			Integer temp = Integer.parseInt(holder[1]);
			menu.put(holder[0], temp);
		}
		
		System.out.println("Welcome to Big Boy! Here is our menu:");
		System.out.println(menu.toString());
		System.out.print("Enter a food item or hit enter to total up your order: ");
		
		String userInput = user.nextLine();
		while (!userInput.equals("")) {
			bigBoy.add(menu.get(userInput));
			System.out.println("Your subtotal is " + bigBoy.formatAsDollars(bigBoy.getSubtotal()));
			System.out.print("Enter a food item or hit enter to total up your order: ");
			userInput = user.nextLine();
		}
		
		System.out.println("Thanks for coming to Big Boy!");
		System.out.println("Subtotal: " + bigBoy.formatAsDollars(bigBoy.getSubtotal()));
		System.out.println("Tax: " + bigBoy.formatAsDollars(bigBoy.getTax()));
		System.out.println("Tip: " + bigBoy.formatAsDollars(bigBoy.getTip()));
		System.out.println("Total: " + bigBoy.formatAsDollars(bigBoy.calculateTotal()));
		System.out.println("Thank you! Goodbye!");
		
		
	}

}
