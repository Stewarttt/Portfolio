import java.lang.Math;
public class RedBubble extends Bubble {

  /* constructor */
  public RedBubble(double x, double y, double speedX, double speedY, double radius, int health) {
    super(x, y, radius);
    this.speedX = speedX;
    this.speedY = speedY;
    this.health = health;
    this.colour = Bubble.COLOURS[0];
  }


  /** updates current bubble object */
  public void update(Bubble[] bubbles, int x1, int y1, int x2, int y2) {
    // if bubbles hits border of world then bounce off in a random direction
    if ( x-radius<x1) {
      x=x1+radius; 
      setSpeedX(-getSpeedX());
    } else if ( x+radius>x2) {
      x = x2-radius;
      setSpeedX(-getSpeedX());
    }
    if ( y+radius>y2) {
      y=y2-radius;
      setSpeedY(-getSpeedY());
    } else if ( y-radius<y1) {
      y=y1+radius;
      setSpeedY(-getSpeedY());
    }

    // steal health from any bubble it overlaps wth
    for (Bubble b : bubbles) {
      if ( this != b && b.getHealth() > 0 && collidesWith(b) && "Green".equals(b.getColour())) {
        health +=1 ;
        b.setHealth( b.getHealth() - 1);
      } else if ( this != b && collidesWith(b) && "Blue".equals(b.getColour())) {
        health += b.getHealth() ;
        b.setHealth(0);
      }
    }

    // Variables 
    double hypotenuse;
    double shortestHhypotenuse = Math.sqrt(Math.pow((x2-x1), 2) + Math.pow((y2-y1), 2)); // longest possible distance, the diagonal value of the enter board is set, therefore, the maximum possible distance isnt greater then the distance of the board
    double slopeX ;
    double slopeY ;
    double sSquare;

    for (Bubble b : bubbles) {

      slopeX = b.getX() - getX() ;
      slopeY = b.getY() - getY() ;
      
      if (x > b.getX()) { //ball is left of me
        slopeX += b.getRadius();
      }
      else {
        slopeX -= b.getRadius();
      }
      
      if (y < b.getY()) { //ball is above me
        slopeY += getRadius();
      }
      else {
        slopeY -= getRadius();
      }
      
      hypotenuse = Math.sqrt((slopeX * slopeX) + (slopeY * slopeY)); //two negitives always make a positive
      
      if (Bubble.COLOURS[1].equals(b.getColour())) {
        if (hypotenuse < shortestHhypotenuse) {
          
          
          
          
          
          shortestHhypotenuse = hypotenuse;
          speedX = slopeX;
          speedY = slopeY;
          sSquare = speedX * speedX + speedY * speedY;
          
          while (sSquare > MAX_SPEED*MAX_SPEED) { //verifies speed dosnt cross threashold, well still preserving the ratio of X and Y speed
            if (speedX < 0) {
              speedX = -Math.sqrt(-speedX); //negates speedX to repapply negation after square root
            }
            else {
              speedX = Math.sqrt(speedX);
            }
            if (speedY < 0) {
              speedY = -Math.sqrt(-speedY); // same as above
            }
            else {
              speedY = Math.sqrt(speedY);
            }
            sSquare = speedX * speedX + speedY * speedY;
          }
        }
        if (Bubble.COLOURS[1].equals(b.getColour()) && collidesWith(b)) { //attacks bubble then stops
          setSpeedX(0);
          setSpeedY(0);
        }
      }
    }
    
    this.capSpeed(MAX_SPEED);
    this.x =  (this.x + this.speedX);
    this.y =  (this.y + this.speedY);
    
  }
}