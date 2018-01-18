#ifndef ENEMYSHOOTSPRITE_H
#define ENEMYSHOOTSPRITE_H
#include <string>
#include <iostream>
#include <list>
#include "sprite.h"
#include "bullet.h"
#include "smartSprite.h"
#include "twoWaySprite.h"
//#include "clock.h"


class EnemyShootSprite : public SmartSprite {
public:
  EnemyShootSprite(const std::string& n, const Vector2f& pos, int w, int h);
  EnemyShootSprite(const EnemyShootSprite& s);

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
  EnemyShootSprite& operator=(const EnemyShootSprite&);
  //Clock& clock;
};
#endif