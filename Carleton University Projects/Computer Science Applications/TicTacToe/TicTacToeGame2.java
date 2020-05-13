/** COMP 1006/1406
  * Summer 2017
  * 
  * Assignment 1
  */

import java.util.Scanner;
import java.io.FileReader;
import java.io.BufferedReader;

public class TicTacToeGame{
  /** symbol for X */
  public static final Character ex = 'X';
  
  /** symbol for O */
  public static final Character oh = 'O';

  /** symbol for empty grid element */
  public static final Character empty = ' ';
  
  /** board is the grid that the game is played on; 
    * each element must be one of ex, oh or empty*/
  public static Character[][] board;
  
  
  /** rows is the number of rows (N) in the game board */
  public static int rows;
    
  /** columns is the number of columns (M) in the game board */
  public static int columns;
  
  /** win_condition is the value of K, the winning condition of the game */
  public static int win_condition; 
  
  /** specifies if the game is 1p (human-human) or 2p (human-computer) */
  public static boolean human_human_game;
  
  
  /** Checks for a win based on the last symbol played in the game
   * 
   * It is assumed that the position specified by the last_row_played
   * and last_column_played is valid and that the symbol in the board
   * at that position is not empty. (It must be <em>ex</em> or <em>oh</em>)
   * 
   * @param last_row_played is the row of the last symbol played
   * @param last_column_played is the column of the last symbol played
   * @return the length of the winning row/column/diagonal of symbols 
   * if the last move is a winning move, or -1 if the last move is not 
   * a winning move.
   */
  public static int win(int last_row_played, int last_column_played){
    
    // variables
    int total = 0;
    Character peace = board[last_row_played][last_column_played];
    int highest = 0;
    
    //checks to see if a victory condition is met upon a horizontal plane
    for(int cols=0; cols<columns; cols+=1){
      if (board[last_row_played][cols] != peace) {
        total = 0;
      }
      if (board[last_row_played][cols] == peace) {
        total += 1; 
      }
      if (total >= win_condition) {
        highest = total;
      }
    }
    total = 0;
    if (highest != 0) {
      return highest;
    }
    //checks to see if a victory condition is met upon a vertical plane
    for(int row=0; row<rows; row+=1){
      if (board[row][last_column_played] != peace) {
        total = 0;
      }
      if (board[row][last_column_played] == peace) {
        total += 1; 
      }
      if (total >= win_condition) {
        highest = total;
      }
    }
    if (highest != 0) {
      return highest;
    }
    
    
    // check to see if a victory condition is met upon a positive-diagonal plane
    total = 0; 
    int colRight = columns - (last_column_played+1);
    int colLeft = last_column_played;
    int rowDown = rows - (last_row_played+1);
    int rowUp = last_row_played;
    int possibleQuadOne = 0;
    int possibleQuadThree = 0;
    int possibleQuadTwo = 0;
    int possibleQuadFour = 0;
    
    
    if (rowUp > colRight) {
      possibleQuadOne = colRight;
    }
    else {
      possibleQuadOne = rowUp;
    }
    
    if (rowDown > colLeft) {
      possibleQuadThree = colLeft;
    }
    else {
      possibleQuadThree = rowDown;
    }

    for(int in = 1; in <= possibleQuadOne; ++in) {
      if (board[last_row_played-in][last_column_played+in] == peace) {
        total += 1;
      }
      else {
        break; 
      }
    }

    for(int in = 0; in <= possibleQuadThree; ++in) {
      if (board[last_row_played+in][last_column_played-in] == peace) {
        total += 1;
      }
      else {
        break;
      }
    }

    if (total >= win_condition) {
        return total;
    }
    
    
    
    total = 0;
    
    
    
    if (rowUp > colLeft) {
      possibleQuadTwo = colLeft;
    }
    else {
      possibleQuadTwo = rowUp;
    }
    
    if (rowDown > colRight) {
      possibleQuadFour = colRight;
    }
    else {
      possibleQuadFour = rowDown;
    }
    
    for(int in = 1; in <= possibleQuadTwo; ++in) {
      if (board[last_row_played-in][last_column_played-in] == peace) {
        total += 1;
      }
      else {
        break; 
      }
    }

    for(int in = 0; in <= possibleQuadFour; ++in) {
      if (board[last_row_played+in][last_column_played+in] == peace) {
        total += 1;
      }
      else {
        break;
      }
    }

    if (total >= win_condition) {
        return total;
    }
    
    
    /*
    // logic block sorts the positive diagonal plane into a horizontal list
    for(int row=0; row<rows*2; row+=1){
      for(int cols=0; cols<columns; cols+=1){
        
        int positive = row - cols;
        if(positive < rows && cols < rows && positive > -1) {
          if (board[positive][cols] != peace) {
            total = 0;
          }
          if (board[positive][cols] == peace) {
            total += 1; 
          }
          if (total >= win_condition) {
            highest = total;
          }
        }
        else {
          total = 0;
        }
      }
    }
    total = 0;
    if (highest != 0) {
      return highest;
    }
    // logic block sorts the negitive diagonal plane into a horizontal list
    
    //creates a rotated version of the board to be analized by the diagonal algorithum
    Character[][] dup;
    
    int ColumnCount = board.length;
    int RowCount = board[0].length;
    
    dup = new Character[RowCount][ColumnCount];

    
    // duplicates the contents of the array, and rotates it positive 90 degrees to be run into the next scanner
    for (int row = 0; row < ColumnCount; row++) {
        for (int col = 0; col < RowCount; col++) {
            dup[col][ColumnCount-1-row] = board[row][col];
        }
    }
    
    
    total =0;
    // checks for a negitive diagonal on the flipped board
    
    for(int row=0; row<rows*2; row+=1){
      for(int cols=0; cols<columns; cols+=1){
        int positive = row - cols;
        if(positive < rows && cols < rows && positive > -1 && positive < columns-1) {
          if (dup[positive][cols] != peace) {
            total = 0;
          }
          if (dup[positive][cols] == peace) {
            total += 1; 
          }
          if (total >= win_condition) {
            highest = total;
          }
        }
        else {
          total = 0;
        }
      }
    }
    if (highest != 0) {
      return highest;
    }

    total = 0; //resets the total variable for next check
    */
    
    
    return -1;
  }
  
  public static void initBoard(){
    /* nested for loop, goes through each row procedurally, then selects each column within that row, intilizing it to a empty space */
    for(int currentRow=0; currentRow<rows; currentRow+=1) {
      for(int currentColumn=0; currentColumn<columns; currentColumn+=1) {
        board[currentRow][currentColumn] = empty;
      }
    }
  } //end initBoard
  
  /* checks if the game is a draw */
  public static int draw() {
    for(int currentRow=0; currentRow<rows; currentRow+=1) {
      for(int currentColumn=0; currentColumn<columns; currentColumn+=1) {
        if (board[currentRow][currentColumn] == empty) { //checks if there is still a space in the board
          return 0;
        } //end if
      } //end for
    } //end for
    return 1;
  } // end draw
  
  /* ai move maker */
  public static int ai(boolean peaceIsEx) {
    Character peace = 'Q';
    if (peaceIsEx == true) {
      peace = oh;
    }
    else if (peaceIsEx == false) {
      peace = ex;
    } //end if
    int winNumber = 0;
    for(int currentRow=0; currentRow<rows; currentRow+=1) {
      for(int currentColumn=0; currentColumn<columns; currentColumn+=1) {
        if (board[currentRow][currentColumn] == empty) { //checks if there is still a space in the board
          board[currentRow][currentColumn] = peace;
          System.out.print("computer plays r " + currentRow + " c " + currentColumn + "\n");
          winNumber = win(currentRow, currentColumn);
          if (winNumber > -1) {
            return winNumber;
          } 
          else {
            return -1;
          } // end if 
        } //end if
      } //end for
    } //end for
    return -1;
  } //end ai
  
  /* gets a simple string from user */
  public static String input() {
    Scanner input = new Scanner(System.in);
    String userInput = input.nextLine(); //scans for the next input token as a String
    
    return userInput;
    
  }
  
  
  /* accsess function, prints the board to the user */
  public static void printBoard() {
    for(int currentRow=0; currentRow<rows; currentRow+=1) {
      System.out.print("|");
      for(int currentColumn=0; currentColumn<columns; currentColumn+=1) {
        System.out.print(" " + board[currentRow][currentColumn] + " |");
      }
      System.out.print("\n+");
      for (int col=0; col<columns; col += 1){
        System.out.print("---+");
      }
      System.out.print("\n");
    }
  }
  
  public static int[] parser(String input) {
    int[] array = new int[3];
    input = input.toLowerCase();  //bring the whole string to lower case letters
    String[] splittedInput = input.split("\\s+"); //splits string allong the whitespace regular expression
    array[2] = 0;
    if ("r".equals(splittedInput[0]) || "row".equals(splittedInput[0])) {
      array[0] = Integer.parseInt(splittedInput[1]);
      if ("c".equals(splittedInput[2]) || "col".equals(splittedInput[2]) || "column".equals(splittedInput[2])) {
        array[1] = Integer.parseInt(splittedInput[3]);
      }
    }
    else if ("c".equals(splittedInput[0]) || "col".equals(splittedInput[0]) || "column".equals(splittedInput[0])) {
      array[1] = Integer.parseInt(splittedInput[1]);
      if ("r".equals(splittedInput[2]) || "row".equals(splittedInput[2])) {
        array[0] = Integer.parseInt(splittedInput[3]);
      }
    }
    return array;
  }
  
  
  /* mutator function, changes a simple value within the array for the board */
  public static void placeMove(int row, int col, boolean player) {
    if (player == true) {
      board[row][col] = ex;
    }
    else if (player == false) {
      board[row][col] = oh;
    }
  }
  
  /** main method to run the game 
    * 
    * @param args optional command line arguments to 
    * specify if the game is 1p (human-computer) or
    * 2p (human-human). Expect the string "2p" if any
    * command line argument is given.
    */
  public static void main(String[] args){
    
    /*------------------------------------------
     * handle command line arguments if any     
     * to determine if the game is human-human  
     * or human-computer                        
     *------------------------------------------*/
    if( args.length > 0){
      /* there are commend line arguments present */
      
      if (args[0].equals("2p")){
        human_human_game = true;
      }
      else if (args[0].equals("1p")){
        human_human_game = false;
      }
      else {
        human_human_game = false; 
      }
        
      
    }else{
      /* there are no command line argument present */
      
      human_human_game = false;
      
    }
    
    /*------------------------------------
     * read N-M-K data from init file   
     * N = rows                         
     * M = columns                      
     * K = win_condition                
     *------------------------------------*/
    
    /*-------------------------------------
     *-------------------------------------
     * BEGIN : Do NOT change the code here
     *-------------------------------------*/
    BufferedReader file_input;
    FileReader     file;
    String         file_name = "init"; 
    String         line;
    
    try{
      file = new FileReader(file_name);
      file_input = new BufferedReader(file);
      
      line = file_input.readLine();
      rows = Integer.parseInt(line);
      
      line = file_input.readLine();
      columns = Integer.parseInt(line);
      
      line = file_input.readLine();
      win_condition = Integer.parseInt(line);
      
      /* always close your files you are done with them! */
      file_input.close();
      
    }catch(Exception e){
      /* somethine went wrong! */
      System.err.println("Failure to read data from init file properly");
      System.err.println(e);
      System.err.println("Program ending");
      return;
    }

    
    /*-------------------------------------
     * END : Do NOT change the code here
     *------------------------------------- 
     *-------------------------------------*/

    
    /* create and initialize the game board */
    
    /* allocate memory for the board array */
    board = new Character[rows][columns];
    
    // add code to initialize all elements to empty
    initBoard();
    
    /* nested for loop, goes through each row procedurally, then selects each column within that row, intilizing it to a empty space */
    /*for(int currentRow=0; currentRow<rows; currentRow+=1) {
      for(int currentColumn=0; currentColumn<columns; currentColumn+=1) {
        board[currentRow][currentColumn] = empty;
      }
    }*/
    
    
    
    
    
     
    
    /* code to drive the game */
    
    // welcome message
    
    
    System.out.print("welcome to Connor's tic-tac-toe++ game!\n");
    
    // ---Variables--- 
    
    // intergers
    int winNumber = 0; //number of peaces crossed that the winner used to win the game
    int playerOneWins = 0; //total number of wins for this player
    int playerTwoWins = 0; //total number of wins for this player
    int games = 0; //total number of games played
    int draws = 0; //number of draws within play session
    int playerRow = 0; //row that the player selects via. input
    int sym = -2; //simulation of victory conditions, runs check to see if anyone won
    int playerCol = 0; //column that the player selects via. input
    int bestWin = 0; //largest amount of peaces to be crossed within a single play session
    int playerNum = -3334; //the interger value of the current player
    int type = 0; //the type of peace to be displayed on the board
    
    //interger array
    int[] result; //stores the result of the parsed input within this interger array
    
    // Boolean operators
    boolean gameActive = true; //controlls loop for a single game
    boolean invalidInput = true; //contolls loop for user input, regarding peaces moving
    boolean replayLogic = true; //controlls the main loop, is turned false upon user input
    boolean correctInput = false; //controlls loop for user input, reguarding the replay discition
    boolean peaceIsEx = true; //denotes the first peace to move upon the first turn, if true, the first player (who is always x) goes first. Alternates each bew gane.
    boolean peaceInverted = true; //the inverted value of peaceIsEx
    boolean runNum = true; //run number, used to controll what player goes first, as the players swap each game
    
    //strings
    String name = "default"; //String value of current user playering. IE player 1, player 2, or computer
    String replay = "yes"; //stores the value for if the user wishes to replay the game
    String extraSpaces = ""; //used for text-formating for the score board. extra spaces for the difference in charicters betweeen computer and player 2
    
    
    
    while (replayLogic == true) {
      invalidInput = true;
      if (human_human_game == true) {
        printBoard();
        games += 1;
        type = 0;
        while (gameActive == true) {
          invalidInput = true;
          while (invalidInput == true) {
            if (peaceIsEx == true) {
              playerNum = 1;
            }
            else {
              playerNum = 2;
            }
            System.out.print("player "+playerNum+" input : ");
            
            result = parser(input());
            playerRow = result[0];
            playerCol = result[1];
            
            if ((playerRow < 0 || playerRow >= rows) || (playerCol < 0 || playerCol >= columns)) {
              System.out.print("you cannot move there\n");
            }
            else {
              if (board[playerRow][playerCol] == empty) {
                placeMove(playerRow, playerCol, peaceIsEx);
                invalidInput = false;
              }
              else {
                System.out.print ("you cannot move over another peace\n");
              }
            }
          }
          invalidInput = true;
          sym = win(playerRow, playerCol);
          if (sym != -1) {
            System.out.print("player "+playerNum+" wins with " + sym + " symbols!\n");
            if (peaceIsEx == true) {
              playerOneWins += 1;
            }
            else {
              playerTwoWins += 1;
            }
            printBoard();
            break;
          }
          if (draw() == 1) {
            System.out.print("There are no spaces left, the game is a draw!\n");
            draws += 1;
            printBoard();
            break;
          }
          printBoard();
          while (invalidInput == true) {
            if (peaceIsEx == true) {
              playerNum = 2;
            }
            else {
              playerNum = 1;
            }
            System.out.print("player "+playerNum+" input : ");
            result = parser(input());
            playerRow = result[0];
            playerCol = result[1];
            if ((playerRow < 0 || playerRow >= rows) || (playerCol < 0 || playerCol >= columns)) {
              System.out.print("you cannot move there\n");
            }
            else {
              if (board[playerRow][playerCol] == empty) {
                peaceInverted = peaceIsEx;
                peaceInverted ^= true;
                placeMove(playerRow, playerCol, peaceInverted);
                invalidInput = false;
              }
              else {
                System.out.print ("you cannot move over another peace\n");
              }
            }
          }
          
          sym = win(playerRow, playerCol);
          if (sym != -1) {
            System.out.print("player "+playerNum+" wins with " + sym + " symbols!\n");
            if (peaceIsEx == true) {
              playerTwoWins += 1;
            }
            else {
              playerOneWins += 1;
            }
            printBoard();
            break;
          }
          if (draw() == 1) {
            System.out.print("There are no spaces left, the game is a draw!\n");
            draws += 1;
            printBoard();
            break;
          }
          printBoard();
        }
        
        
        
        
      }
      else if (human_human_game == false) {
        games += 1;
        type = 1;
        while (gameActive == true) {
          //ai move 2
          
          if (runNum == false){
            winNumber = ai(true);
            if (winNumber != -1) {
              System.out.print("The ai wins with " + winNumber + " symbols!\n");
              playerTwoWins += 1; //computer is player 2
              printBoard();
              gameActive = false;
              break;
            }
          }
          
          printBoard();
          
          if (draw() == 1) {
            System.out.print("There are no spaces left, the game is a draw!\n");
            draws += 1;
            break;
          }
          
          invalidInput = true;
          while (invalidInput == true) {
            System.out.print("player 1 input : ");
            result = parser(input());
            playerRow = result[0];
            playerCol = result[1];
            
            if ((playerRow < 0 || playerRow >= rows) || (playerCol < 0 || playerCol >= columns)) {
              System.out.print("you cannot move there\n");
            }
            else {
              if (board[playerRow][playerCol] == empty) {
                placeMove(playerRow, playerCol, true);
                invalidInput = false;
              }
              else {
                System.out.print ("you cannot move over another peace\n");
              }
            }
          }
          invalidInput = true;
          sym = win(playerRow, playerCol);
          if (sym != -1) {
            System.out.print("player 1 wins with " + sym + " symbols!\n");
            playerOneWins += 1;
            printBoard();
            break;
          }
          if (draw() == 1) {
            System.out.print("There are no spaces left, the game is a draw!\n");
            draws += 1;
            printBoard();
            break;
          }
          
          //ai move 1
          
          if (runNum == true) {
            winNumber = ai(true);
            if (winNumber != -1) {
              System.out.print("The ai wins with " + winNumber + " symbols!\n");
              playerTwoWins += 1; //computer is player 2
              printBoard();
              break;
            }
          }
        }
      }
      
      correctInput = false;
      if (sym > bestWin) {
        bestWin = sym;
      }
      while (correctInput == false) {
        System.out.print("Would you like to play again? ");
        replay = input();
        if ("yes".equals(replay)) {
          correctInput = true;
          peaceIsEx ^= true;
          runNum ^= true;
          gameActive = true;
          initBoard();
        }
        else if ("no".equals(replay)) {
          replayLogic = false;
          correctInput = true;
          if (type == 0) {
            name = "2";
            extraSpaces = "       ";
          }
          else if (type == 1) {
            name = "computer";
          }
          System.out.print("Thanks for playing Connor's tic-tac-toe++!\n");
          System.out.print("--------SCORE--------\n");
          System.out.print("games played          : " + games + "\n");
          System.out.print("player 1 wins         : " + playerOneWins + "\n");
          System.out.print("player "+ name +" wins  "+extraSpaces+": " + playerTwoWins + "\n");
          System.out.print("draws                 : " + draws + "\n");
          System.out.print("best win              : " + bestWin + " in a row\n");  
        }
        else {
          System.out.print("error, invalid entry, please enter a 'yes' or a 'no' in lowercase letters to continue\n");
        }
      }
    }
  }
} 