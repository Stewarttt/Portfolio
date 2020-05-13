//imported packages
import java.util.Scanner; //needed to get user input
import java.util.List; //used for making a string list
import java.util.ArrayList; //used for making a string list
/*
Name: comp3007_w19_101041125_a1_1
Input:
	user input of a word in snake case
Output:
	pig Latin representation of user snake case input
Description:
	A six step program that gets user input, checks it, and converts it into pig Latin
	user input is in snake case, in all lower case letters
	invalid input will end the program
	this class is intended to solve the problem outlined in assignment 1, question 1 or COMP 3007 W18
*/
public class comp3007_w19_101041125_a1_1 {	
	/*
	Name: getUserInput
	Input: 
		None
	Output:
		String - a string representation of the users input\
		PROMPT - instructions for input format
	Description:
		A simple function that prompts the user for input, then gets it from the system
	*/
	public String getUserInput() { //pre A
		Scanner input = new Scanner(System.in); //Define the system input
		System.out.println("This program converts lower case input in snake case into pig Latin\n"
		+"	Snake case is lower case input with '_' instead of spaces, an example would be: connor_made_this\n"
		+"Please enter you input in snake_case: "); //PROMPT the user
		String userInput = input.nextLine(); //get the user input		
		return userInput; //return the user input
	} //END getUserInput

	/*
	Name: isLowerCaseLetter
	Input: 
		String userInput - the user input to be checked
	Output:
		boolean - a true or false value representing if the string is all lower case letters
	Description:
		Recursively checks if a string is in all lower case letters
	*/
	public boolean isLowerCaseLetter(String userInput) { //Part A
		char currentChar = userInput.charAt(0); //gets the first char within the string
		//char upperCaseChar = (char)(currentChar & 0x5f); //conversion using boolean operators				
		int asciiValue = currentChar; //this converts the char to the integer representation of its corresponding ascii value
		
		if (currentChar == '_') { //skips over underscores
			if (userInput.substring(0,1) == userInput) { //if this is the last element
				return false; //not a valid position for a space in snake case
			} //END IF
			return true & isLowerCaseLetter(userInput.substring(1)); //find next character
		}
		else if (!(currentChar >= 97 && currentChar <= 122)) { //if this character is in its upper case form
			return false; //false, exits recursive cascade
		}
		else if (userInput.substring(0,1) == userInput) { //if the string is only 1 character long
			return true; //returns true, if the character was false the above else if would have been triggered
		}
		else { //return true for lower case, and checks the next letter
			return true & isLowerCaseLetter(userInput.substring(1)); 
		} //END IF			
	} //END isLowerCaseLetter	
	
	/*
	Name: leftmostUnderscore
	Input:
		userInput - the user input to be checked
	Output:
		int - the position of the leftmost underscore in the users input
	Description:
		this function takes user input and finds the leftmost underscore within the 
		imputed string. This is written under the convention that the first element in
		a string has an index value of zero.
		if the string has no underscore, the function simply returns the length of the list.
	*/
	public int leftmostUnderscore(String userInput) { //Part B
		char currentChar = userInput.charAt(0); //gets the first char within the string
		if (currentChar == '_') { //if there is an underscore
			return 0; //this is the current element, add nothing
		}
		else if (userInput.substring(0,1) == userInput) { //if the string is only 1 character long
			return 1; //this is the end of the list, add one
		}
		else { //currentChar != '_' and the string is greater then 1 character long
			return 1 + leftmostUnderscore(userInput.substring(1)); //return underscore position
		} //END IF
	} //END leftmostUnderscore
	
	/*
	Name: splitUnderscore
	Input: 
		String userInput - a string representation of the users input
	Output: 
		List<String> - a list representation of the users input
	Description:
		recursively splits an input string along its underscores to produce an array representation of the same list
			makes use of ArrayLists get, which is O(1)
	*/
	public List<String> splitUnderscore(String userInput) { //Part C
		int leftMostUnderscore; //the index of the leftmost underscore in the string
		String newSubString; //the sub-string made after splicing the string along the underscore
		
		//initialize a string list for the return
		List<String> stringList = new ArrayList<String>();
	
		//instantiate a copy of the class for use
		comp3007_w19_101041125_a1_1 assignmentOneObject = new comp3007_w19_101041125_a1_1();
		
		//find the leftmost underscore in the string
		leftMostUnderscore = assignmentOneObject.leftmostUnderscore(userInput);		
		
		//add to the string list the contents of the userInput up to the leftmost underscore
		stringList.add(userInput.substring(0, leftMostUnderscore));
		
		if (userInput == stringList.get(0)) { //if this is the last element in the input string
			return stringList; //the userInput is now empty, thus there's nothing left to split
		}
		else { //if this is not the last element in the input string
			//split the string up to the leftmost underscore
			newSubString = userInput.substring(leftMostUnderscore+1);
			//return the concatenated array list
			return concatArrayList(stringList, splitUnderscore(newSubString));
		} //END IF
	} //END splitUnderscore
	
	/*
	Name: concatArrayList
	Input: 
		List<String> addTo - the list that will be added to
		List<String> addFrom - the list that will be added from
	Output: 
		List<String> - the concatenated list
	Description:
		recursively concatenates two lists into one
		makes use of arrayLists size/get/subList/add, which all run in O(1) time
			size is stored as a variable in the array list object
	*/
	public List<String> concatArrayList(List<String> addTo, List<String> addFrom) {	//Part C & F Helper Function
		if (addFrom.size() == 1) { //size is O(1), if the list is at its last element, add it
			addTo.add(addFrom.get(0)); //get/add are O(1)
			return addTo;
		}
		else if (addFrom.size() == 0) { //if the list is empty, simply return the starting addTo list
			return addTo;
		}
		else { //add the elements from the addFrom list to the addTo list recursively
			addTo.add(addFrom.get(0)); //both add and get are O(1)
			addFrom = addFrom.subList(1, addFrom.size()); //subList is O(1)
			addTo = concatArrayList(addTo, addFrom); //recursive step
			return addTo; //return the concatenated list
		} //END IF		
	} //END concatArrayList
	
	/*
	Name: leftmostVowel
	input:
		String userInput - string representation of a word
	Output:
		int - position of the leftmost vowel in userInput
	Description:
		Simply finds the position of the leftmost vowel in the given string input
		This is done via recursive calls
	*/
	public int leftmostVowel(String userInput) { //Part D
		char currentChar = userInput.charAt(0); //gets the first char within the string
		if (currentChar == 'a' || currentChar == 'e' 
		|| currentChar == 'i' || currentChar == 'o'
		|| currentChar == 'u') { //if there is a vowel
			return 0; //this is the current element, add nothing
		}
		else if (userInput.substring(0,1) == userInput) { //if the string is only 1 character long
			return 1; //this is the end of the list, add one
		}
		else { //currentChar != 'vowel' and the string is greater then 1 character long
			return 1 + leftmostVowel(userInput.substring(1)); //return underscore position
		} //END IF
	} //END leftmostVowel
		
	/*
	Name: wordToPigLatin
	Input;
		String userInput - string representation of a single word
	Output:
		String - the pig Latin representation of userInput
	Description:
		Converts the userInput string into a pig Latin representation of the same word
		This is done through non recursive means
	*/
	public String wordToPigLatin(String userInput) { //Part E
		//instantiate a copy of the class for use
		comp3007_w19_101041125_a1_1 assignmentOneObject = new comp3007_w19_101041125_a1_1();
		//find the position of the leftmost vowel in the word
		int leftmostVowelPosition = assignmentOneObject.leftmostVowel(userInput);
		//find the prefixing letters to the leftmost vowel
		String prefixLetters = userInput.substring(0, leftmostVowelPosition);
		//find the sufficing letters from the leftmost vowel, including the leftmost vowel itself
		String suffixLetters = userInput.substring(leftmostVowelPosition);
		//re-concatenate the prefixing and the suffixing word in reverse order, and add 'ay' at the end to produce the pig Latin equivalent
		String reconcatenatedWordPigLatin = suffixLetters + prefixLetters + "ay";		
		//return the pig Latin word
		return reconcatenatedWordPigLatin; 
	} //END wordToPigLatin
	
	/*
	Name: recursiveConvertionToPigLatin
	Input:
		List<String> userInput - contains a list of words to be converted to pig Latin
	Output:
		List<String> - list of words containing the pig Latin representation of the input
	Description:
		Simply takes the input list of words and recursively converts it to pig latin
	*/
	public List<String> recursiveConvertionToPigLatin (List<String> userInput) { //Part F
		//holds the new pig Latin word;
		String pigLatinWord;
		
		//instantiate a copy of the class for use
		comp3007_w19_101041125_a1_1 assignmentOneObject = new comp3007_w19_101041125_a1_1();
		//initialize a string list for the return
		List<String> stringList = new ArrayList<String>();
		//convert the word to pig Latin
		pigLatinWord = wordToPigLatin(userInput.get(0));
		//add this new word to the string list for return
		stringList.add(pigLatinWord);
		
		if (userInput.size() == 1) { //if this is the last element in the input list
			return stringList; //the userInput is now empty, thus there's nothing left to split
		}
		else { //if this is not the last element in the input list		
			List<String> newSublist = userInput.subList(1, userInput.size()); //contains the list minus its first element
			return concatArrayList(stringList, recursiveConvertionToPigLatin(newSublist)); //join the current list with all its successive elements
		} //END IF 
	} //END reconcatenatedWordPigLatin
	
	/*
	Name: main
	Input:
		None
	Output:
		None
	Description:
		This is the entry point for the application
	*/
	public static void main(String[] args) {	
		//instantiate a copy of the class for use
		comp3007_w19_101041125_a1_1 assignmentOneObject = new comp3007_w19_101041125_a1_1();
	
		//get user input
		String userInput = assignmentOneObject.getUserInput();
		
		//check if the user input is all lower case
		boolean allLowerCase = assignmentOneObject.isLowerCaseLetter(userInput);
		if (!allLowerCase || userInput.charAt(0) == '_') { //end app if capitalized input is entered
			System.out.println("the characters entered in '"
			+ userInput + "' were not all lower case letters, or it began/ended with a _.\n"
			+ "The application will now exit\n"); //PROMPT user
			return; //end application by returning void
		} //END IF
		
		System.out.println("the following is the snake case input in pig Latin representation: "
		+ assignmentOneObject.recursiveConvertionToPigLatin(assignmentOneObject.splitUnderscore(userInput)) + "\n");
		
	} //END main
} //END comp3007_w19_101041125_a1_1