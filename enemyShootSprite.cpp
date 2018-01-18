#include "enemyShootSprite.h"
#include "gamedata.h"

EnemyShootSprite::EnemyShootSprite(const std::string& name, const Vector2f& pos, int w, int h) :
  SmartSprite(name,pos,w,h),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(),
  freeList(),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0)
  //clock( Clock::getInstance() )

{ }

EnemyShootSprite::EnemyShootSprite(const EnemyShootSprite& s) :
  SmartSprite(s),
  bulletName(s.bulletName),
  bullets(s.bullets),
  freeList(s.freeList),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastFrame(s.timeSinceLastFrame)
 // clock( Clock::getInstance() )
{ }


void EnemyShootSprite::shoot() { 
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()/2;
  //std::cout << "TICKS : "<< clock.getTicks() << std::endl;
  //TwoWaySprite::images = TwoWaySprite::imagesShootLeft;
 // SmartSprite::images = SubjectSprite::imagesShootLeft;
  // I need to add some minSpeeld to velocity:
  if (freeList.empty())
  {
    Bullet bullet(bulletName);
    bullet.setPosition( getPosition() + Vector2f(deltaX-175 , deltaY) );
    bullet.setVelocity( getVelocity() - Vector2f(minSpeed, 0) );
    bullets.push_back( bullet );
    timeSinceLastFrame = 0;
  }

  else
  {
    Bullet bul = freeList.front();
    freeList.pop_front();
    bul.reset();
    bul.setPosition( getPosition() + Vector2f(deltaX-175 , deltaY) );
    bul.setVelocity( getVelocity() - Vector2f(minSpeed, 0) );
    bullets.push_back(bul);
  }

}


void EnemyShootSprite::shootRight() { 
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()/2;
  
  //SSprite::images = SubjectSprite::imagesShootRight;
  // I need to add some minSpeed to velocity:
    //  TwoWaySprite::images = TwoWaySprite::imagesShootRight;

  if (freeList.empty())
  {
    Bullet bullet(bulletName);
    bullet.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
    bullet.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
    bullets.push_back( bullet );
    timeSinceLastFrame = 0;
  }

  else
  {
    Bullet bul = freeList.front();
    freeList.pop_front();
    bul.reset();
    bul.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
    bul.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
    bullets.push_back(bul);
  }
    //  TwoWaySprite::images = TwoWaySprite::imagesRight;


}

void EnemyShootSprite::draw() const { 
  SmartSprite::draw();
  for ( const Bullet& bullet : bullets ) {
    bullet.draw();
  }
}

void EnemyShootSprite::update(Uint32 ticks) { 
	timeSinceLastFrame += ticks;
  SmartSprite::update(ticks);
  std::list<Bullet>::iterator ptr = bullets.begin();
  
  while(ptr != bullets.end())
  {
     ptr->update(ticks);
    // std::cout << "-----------------" << std::endl;
    // std::cout << "GET X VALUE IS : " << ptr->getX()<<std::endl;
    // std::cout << "-----------------" << std::endl;
     if(ptr->goneTooFar())
     {
      freeList.push_back(*ptr);
      ptr=bullets.erase(ptr);
     }
     else ++ptr;
  }

}

