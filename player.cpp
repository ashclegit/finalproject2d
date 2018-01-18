#include "player.h"

//constexpr float fJumpImpulse = -200.0f;
//constexpr float fGravity = 100.0f;


Player::Player( const std::string& name) :
  TwoWaySprite(name),
  collision(false),
  initialVelocity(getVelocity())
  //jumpFlag(false)
  //restingContact(true)
{ }

Player::Player(const Player& s) :
  TwoWaySprite(s), 
  collision(s.collision),
  initialVelocity(s.getVelocity())
  //jumpFlag(s.jumpFlag)
  //restingContact(s.restingContact)
  { }

Player& Player::operator=(const Player& s) {
  TwoWaySprite::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  //jumpFlag = s.jumpFlag;
  //restingContact = s.restingContact;
  return *this;
}

void Player::stop() { 
  
  setVelocity(Vector2f(0,0));

  //TwoWaySprite::images = TwoWaySprite::imagesRight;

}

/*void Player::jump() { 
  // if(restingContact)
  // {
  //   setVelocityY(fJumpImpulse);
  //   std::cout << "the jump impulse: " << std::endl;
  //   restingContact = false;
  // }

}*/

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth() ) {
    setVelocityX(initialVelocity[0]);
    TwoWaySprite::images = TwoWaySprite::imagesRight;
  }
} 
void Player::left()  { 
  if ( getX() > 0 ) {
    setVelocityX(-initialVelocity[0]);
    TwoWaySprite::images = TwoWaySprite::imagesLeft;
  } 
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  //TwoWaySprite::images = TwoWaySprite::imagesShootRight;
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) {
  TwoWaySprite::update(ticks);
  if (getVelocityX() == 0 && getVelocityY() == 0 )
  {
    stop();
  }
  else
  {
    //TwoWaySprite::update(ticks);
    /*if(restingContact)
      {
        if (getVelocity() != Vector2f(0, 0) ) advanceFrame(ticks);
      }
    else
      {
        setVelocityY(getVelocityY() + static_cast<float>(ticks) * fGravity * 0.001f);
      }*/ 

    /*if(restingContact)
      {
        if (getVelocity() != Vector2f(0, 0) ) advanceFrame(ticks);
        std::cout << " in the resting contact true" << std::endl;
      }
    else
      {
        setVelocityY(getVelocityY() + static_cast<float>(ticks) * fGravity * 0.001f);
        std::cout << " in the resting contact false" << std::endl;
      }*/

    if ( !collision ) 
    {
      advanceFrame(ticks);
    }

    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);

  /*if ( getY() > worldHeight - getScaledHeight() && getVelocityY() > 0.0f) {
    restingContact = true;
    setY(worldHeight - getScaledHeight());
  }*/
    stop();

  }
  /*if(restingContact)
  {
    setVelocityY(std::max(getVelocityY(), 0.0f));
    stop();
  }*/




}

