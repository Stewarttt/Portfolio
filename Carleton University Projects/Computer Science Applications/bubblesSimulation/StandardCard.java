public class StandardCard extends Card {
  
  public StandardCard(String suit, String rank){
    super(suit, rank); //does nothing
    if (SUITS[0].equals(suit)) {
      super.suit = suit;
    }
    else if (SUITS[1].equals(suit)) {
      super.suit = suit;
    }
    else if (SUITS[2].equals(suit)) {
      super.suit = suit;
    }
    else if (SUITS[3].equals(suit)) {
      super.suit = suit;
    }
    
    if (RANKS[2].equals(rank)) {
      super.rank = rank;
    }
    else if (RANKS[3].equals(rank)) {
      super.rank = rank;
    }
    else if (RANKS[4].equals(rank)) {
      super.rank = rank;
    }
    else if (RANKS[5].equals(rank)) {
      super.rank = rank;
    }
    else if (RANKS[6].equals(rank)) {
      super.rank = rank;
    }
    else if (RANKS[7].equals(rank)) {
      super.rank = rank;
    }
    else if (RANKS[8].equals(rank)) {
      super.rank = rank;
    }
    else if (RANKS[9].equals(rank)) {
      super.rank = rank;
    }
    else if (RANKS[10].equals(rank)) {
      super.rank = rank;
    }
    else if (RANKS[11].equals(rank)) {
      super.rank = "11";
    }
    else if (RANKS[12].equals(rank)) {
      super.rank = "12";
    }
    else if (RANKS[13].equals(rank)) {
      super.rank = "13";
    }
    else if (RANKS[14].equals(rank)) { //ace
      super.rank = "14";
    }
  }
  
  
 

  public String getSuit(){
    return super.suit;
  }
  
  
  
  public String getRankString(){
    return super.rank;
  }
  
  
  public int getRank(){
    return (Integer.parseInt(super.rank));
  }

  
  @Override
  public int compareTo(Card other){
    int otherRank = other.getRank();
    int myRank = this.getRank();

    myRank = cardRank(this.getSuit(), this.getRank());
    otherRank = cardRank(other.getSuit(), other.getRank());
    
    
    if (otherRank == myRank) {
      return this.getSuit().compareTo(other.getSuit());

    }
    
    else {
      return myRank - otherRank;
    }
  }



  
  /*simple healper method for organization
  takes the suit and rank and orders it numerically to be sorted*/
  public int cardRank(String suit, int rank) {
    int current = 0;
    if ("Diamonds".equals(suit)) {
      current = 13 * 0;
    }
    else if ("Clubs".equals(suit)) {
      current = 13 * 1;
    }
    else if ("Hearts".equals(suit)) {
      current = 13 * 2;
    }
    else if ("Spades".equals(suit)) {
      current = 13 * 3;
    }
    else {
      System.out.println("OMG"+ suit);
    }
    
    current = current + rank;
    
    return current;
  }
    

  
  
 /* public void main(String[] args) {
    System.out.println(myRank());
  }*/

}