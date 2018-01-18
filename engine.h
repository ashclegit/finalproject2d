#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "smartSprite.h"
#include "subjectSprite.h"
#include "collisionStrategy.h"
#include "shootingSprite.h"
#include "enemyShootSprite.h"
#include "sound.h"

class CollisionStrategy;
class Bullet;


class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;


  SDL_Renderer * const renderer;
  World trees;
  World snowhill;
  World third;
  Viewport& viewport;
  
  ShootingSprite* player;
  EnemyShootSprite* enemy;
  std::vector<SmartSprite*> spriteVec;
  std::vector<Drawable*> sprites;
  std::vector<Drawable*> asteroid;
  std::vector<CollisionStrategy*> strategies;
  //sd::vector<>


  int currentStrategy;

  Hud& hu;
  bool collision;
  bool displayHud;
  bool shootFlag;
  bool makeVideo;
  int healthCount;
  int enemyHealthCount;
  SDLSound sound;
  bool godFlag;

  void draw() const;
  void update(Uint32);
  void printScales();
  
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
  void enemyBulletCollisionCheck();
  void playerEnemyCollisionCheck();
  //void playerEarthCollision();
  //bool enemySummonCheck();
};
