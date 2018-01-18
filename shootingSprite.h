#ifndef SHOOTINGSPRITE_H
#define SHOOTINGSPRITE_H
#include <string>
#include <iostream>
#include <list>
#include "sprite.h"
#include "bullet.h"
#include "subjectSprite.h"
#include "twoWaySprite.h"
//#include "clock.h"


class ShootingSprite : public /*Sprite*/ SubjectSprite {
public:
  ShootingSprite(const std::string& n);
  ShootingSprite(const ShootingSprite& s);

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  void shoot();
  void shootRight();

  unsigned int bulletCount() const {

      return bullets.size();
  }

  unsigned int freeCount() const 
  {
      return freeList.size();
  }

  std::list<Bullet> getBulletList() {return bullets;}

private:
  std::string bulletName;
  std::list<Bullet> bullets;
  std::list<Bullet> freeList;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastFrame;
  ShootingSprite& operator=(const ShootingSprite&);
  //Clock& clock;
};
#endif