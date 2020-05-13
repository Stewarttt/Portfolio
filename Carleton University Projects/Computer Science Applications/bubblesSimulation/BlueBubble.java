 import java.util.Random;
public class BlueBubble extends Bubble{
  
  public BlueBubble(double x, double y, double speedX, double speed, double radius, int health) {
    super(x, y, radius); //calls parant constructor
    super.setHealth(health); //sets health
    super.setSpeedX(speedX); //sets speed horizontal
    super.setSpeedY(speed); //sets speed vertical
    super.setColour(Bubble.COLOURS[1]); //from inside the global colour list, set the colour using the getter
  }
  
  public void update(Bubble[] bubbles, int x1, int y1, int x2, int y2) {

    if ("Blue". equals(getColour())) { //makes sure balls are blues, so other types dont get dobule negated
      
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
      
      if (x-radius <= (x1+x2)/2 && x > (x1+x2)/2) { //left side
        x=(x1+x2)/2 + radius;
        setSpeedX(-(getSpeedX()));
      }
      else if (x+radius >= (x1+x2)/2 && x < (x1+x2)/2) { //right side
        x=(x1+x2)/2 - radius;
        setSpeedX(-(getSpeedX()));
      }
      
      
    }

    
    
    
    for (Bubble b : bubbles) {
      if (collidesWith(b) && this != b) {
        if ("Red".equals(b.getColour())) {
          setSpeedX(0);
          setSpeedY(0);
        }
        else {
          setSpeedX(-getSpeedX());
          setSpeedY(-getSpeedY());
        }
      }
      // checks to see if blues can transfer health
      if (b.getHealth() - getHealth() >= 2 && collidesWith(b) && "Blue".equals(b.getColour()) && this != b) {
        b.setHealth( b.getHealth() - 1); //it is not possible for this to run twice, as the conditions can never be met within the same tick
        if (getHealth() > 0) {
          setHealth(getHealth() + 1 );
        }
        else {
          setHealth(getHealth() + 1);
          if (collidesWith(b)) {
            Random r = new Random();
            setSpeedX(1 + (Bubble.MAX_SPEED + 1) * r.nextDouble());
            setSpeedY(1 + (Bubble.MAX_SPEED + 1) * r.nextDouble());
          }
        }
      }
      
        
    }
    for (Bubble b : bubbles) {
      if (collidesWith(b) && this != b) {
        if ("Red".equals(b.getColour())) {
          setSpeedX(0);
          setSpeedY(0);
        }
      }
    }
    
    if (getHealth() == 0) {
      setSpeedX(0);
      setSpeedY(0);
    }
    
    this.capSpeed(MAX_SPEED * 1.5); //blues can move 1.5x faster then others
    this.x =  (this.x + this.speedX);
    this.y =  (this.y + this.speedY);
  
  }
}