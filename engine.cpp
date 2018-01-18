#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include <thread>
#include <SDL_ttf.h>
#include <SDL.h>
#include "sprite.h"
#include "player.h"
#include "multisprite.h"
#include "twoWaySprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "hud.h"

Engine::~Engine() { 



  for ( SmartSprite* sprite : spriteVec ) {
    delete sprite;
  }

  for ( Drawable* simplesprite : sprites ) {
    delete simplesprite;
  }


  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  
  for ( Drawable* strategy : asteroid ) {
    delete strategy;
  }
   
    
  delete enemy;
  
  delete player;
  std::cout << "Terminating program" << std::endl;

}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  trees("trees", Gamedata::getInstance().getXmlInt("trees/factor") ),
  snowhill("snowhill", Gamedata::getInstance().getXmlInt("snowhill/factor") ),
  third("third", Gamedata::getInstance().getXmlInt("third/factor") ),
  viewport( Viewport::getInstance() ),
  player(new ShootingSprite("Fighter")),
  enemy(),
  spriteVec(),
  sprites(),
  asteroid(),
  strategies(),
  currentStrategy(0),
  hu(Hud::getInstance()),
  collision(false),
  displayHud(false),
  shootFlag(true),
  makeVideo( true ),
  healthCount(100),
  enemyHealthCount(100),
  sound(),
  godFlag(false)
{
  spriteVec.reserve(4);
  sprites.reserve(1);

  unsigned int j = 10; 
  for ( unsigned int i = 0; i < j; ++i ) {
    Drawable* s = new Sprite("Asteroid");
    float scale = Gamedata::getInstance().getRandFloat(0.5,1.5);
    s->setScale(scale);
    asteroid.push_back(s);
  }
  std::vector<Drawable*>::iterator ptr = asteroid.begin();
  ++ptr;
  sort(ptr, asteroid.end(), SpriteLess());

  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();


  int n = Gamedata::getInstance().getXmlInt("Alien/enemies");


 
  enemy = new EnemyShootSprite("Goku",pos,w,h); 

  sprites.push_back( new Sprite("Earth"));

  for ( int i=0 ; i < n ; i++)
  {
    spriteVec.push_back( new SmartSprite("Alien", pos, w, h) );
    player->attach( spriteVec[i] );
  }
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::printScales() const {
  for (Drawable* s : sprites) {
    std::cout << s->getScale() << std::endl;
  }
}

void Engine::draw() const {
  std::stringstream fpstr;



 std::stringstream bulletlist;
 bulletlist << "Bullet Count: " << player->bulletCount();
 std::stringstream freelist;
 freelist << "Free Count: " << player->freeCount();

 std::string bl=bulletlist.str();
 std::string fl=freelist.str();

 
  snowhill.draw();

  for (unsigned int i=0 ; i<asteroid.size()/3; i++)
  {
    asteroid[i]->draw();
  }


  trees.draw();

  for (unsigned int i=(asteroid.size()/3) + 1 ; i<2*asteroid.size()/3; i++)
  {
   asteroid[i]->draw();
  }


  third.draw();

  for (unsigned int i=2 *(asteroid.size()/3)+1 ; i<asteroid.size(); i++)
  {
    asteroid[i]->draw();
  }
 

 SDL_Rect healthBar = {50,50,healthCount,20};
 SDL_SetRenderDrawColor(renderer,255,153,51,255);
 SDL_RenderFillRect(renderer,&healthBar);


    for(auto spritePtr : sprites)
        {
          spritePtr->draw();
        }

    
    /*if (enemy != NULL)
    {

      if (enemy->getExplosionStatus())
      {
        IOmod::getInstance().writeText("The God Beerus Destroyed Goku and Now he is about to conquer Earth", 420, 130);
        clock.pause();
      }      

    }*/
  
  if (displayHud || clock.getSeconds() < 3)
  {
    hu.drawHud(renderer);
    hu.displayBulletHud(renderer,fl,bl);  
  }


  

  if(enemy != NULL )
  {
    if(spriteVec.size() == 0)
    {
      if(!enemy->getExplosionStatus())
      {
        
        
        IOmod::getInstance().writeText("Enemy Health", 670, 20);
        enemy->draw();
        SDL_Rect enemyHealthBar = {680,50,enemyHealthCount,20};
        SDL_SetRenderDrawColor(renderer,255,153,51,255);
        SDL_RenderFillRect(renderer,&enemyHealthBar);

      }
    } 
  }
   

  if(godFlag)
  {
    string godMessage = "GOD MODE ON";
    SDL_Color godMessageColor = {255,255,0,0};
    io.writeText(godMessage,620,50,godMessageColor);
  }

  IOmod::getInstance().writeText("Player Health", 25, 20);
  for ( const Drawable* sprite : spriteVec ) {
    sprite->draw();
  }
  std::stringstream strm;
  strm << spriteVec.size() << " Smart Sprites Remaining";
  IOmod::getInstance().writeText(strm.str(), 30, 125);

  strategies[currentStrategy]->draw();
  if ( collision ) {
    IOmod::getInstance().writeText("Oops: Collision", 30, 50);
  }

  player->draw();
  float fps = clock.getFps();

  fpstr << "FrameRate: " << fps;

  io.writeText(fpstr.str(),30,100);

  std::string uname = "Ashwin Kumar Vajantri";

  SDL_Color nameColor = {255,255,0,0};

  io.writeText(uname,30,450,nameColor);

  if(healthCount <= 0)
 {
    player->explode();
    clock.pause();
    string restartMessage = "Press R to Restart";
    io.writeText(restartMessage,420,130,nameColor);
 }  




 if (enemyHealthCount <= 0)
 { 
    if(enemy != NULL)
    {
      enemy->explode();
      IOmod::getInstance().writeText("The God Beerus Destroyed Goku and Now he is about to conquer Earth", 50, 160);
      //clock.pause();
    }
 } 
  
  SDL_RenderPresent(renderer);
}



void Engine::enemyBulletCollisionCheck()
{
   if(enemy != NULL)
   {
      std::list<Bullet> enemyBulletList = enemy->getBulletList();
      for(const auto d : enemyBulletList)
      {
        if(strategies[currentStrategy]->execute(*player,d))
        {

          if(!godFlag)
          {
             healthCount = healthCount - 2;
          }
        }
      }
    }
}

void Engine::playerEnemyCollisionCheck()
{
  std::list<Bullet> playerBulletList = player->getBulletList();
  for(const auto d : playerBulletList)
  {
    if (enemy != NULL)
    {
        if(strategies[currentStrategy]->execute(*enemy,d))
        {
          collision = true;
          if(enemyHealthCount>0)
             enemyHealthCount = enemyHealthCount - 2;
        }
    }
  }
}





void Engine::checkForCollisions() {

if(spriteVec.size() == 0)
{
  enemyBulletCollisionCheck();
  playerEnemyCollisionCheck();
}
  
std::list<Bullet> bulletlist = player->getBulletList();
  for (const auto d : bulletlist)
  { 
    std::vector<SmartSprite*>::iterator it1 =  spriteVec.begin();
    while ( it1 != spriteVec.end() ) 
    { 
      if ( strategies[currentStrategy]->execute(**it1, d) )
      {
        
        collision = true;
        (*it1)->explode();
        ++it1;

      }
      else
      {
        ++it1;
      }
    }
    it1 =  spriteVec.begin();
    while ( it1 != spriteVec.end() ) 
    { 
      if ((*it1)->getExplosionStatus())
      {
        player->detach(*it1);
        delete *it1;
        it1 = spriteVec.erase(it1);
      }
      else
      {
        ++it1;
      }
    }
  }


  collision = false;
  for ( const SmartSprite* d : spriteVec ) {
    if ( strategies[currentStrategy]->execute(*player, *d) ) {
      collision = true;
      if(!godFlag)
      {
         healthCount = healthCount - 10;
      }
    }
  }
  for ( const Drawable* d : sprites ) {
    if ( strategies[currentStrategy]->execute(*player, *d) ) {
      collision = true;
    }
  }

  if ( collision ) {
    player->collided();
  }
  else {
    player->missed();
    collision = false;
  }
  auto it = spriteVec.begin();
  while ( it != spriteVec.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = spriteVec.erase(it);
    }
    else ++it;
  }


}

void Engine::update(Uint32 ticks) {

  checkForCollisions();
  for ( SmartSprite* sprite : spriteVec ) {
    sprite->update( ticks );
  }
  player->update(ticks);

  if(enemy != NULL)
  {
    
    enemy->update(ticks);
  }

  if(ticks % 10 == 0)
  {
    if(enemy != NULL)
    {
      enemy->shoot();
    }
  }
  trees.update();

  for (unsigned int i=0 ; i<asteroid.size()/3; i++)
  {
   asteroid[i]->update(ticks);
  }
  snowhill.update(); 

  for (unsigned int i=(asteroid.size()/3)+1 ; i<2*asteroid.size()/3; i++)
  {
    asteroid[i]->update(ticks);
  }
  third.update();
  for (unsigned int i=2 *(asteroid.size()/3)+1 ; i<asteroid.size(); i++)
  {
    asteroid[i]->update(ticks);
  }


  if(enemy != NULL)
  {
     if (enemy->getExplosionStatus() && enemyHealthCount <= 0)
      {
        delete enemy;
        enemy = NULL;
        clock.pause();
      }
  }

  viewport.update(); // always update viewport last
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }

     
        if( keystate[SDL_SCANCODE_R] ){
          clock.unpause();
          return true;
        }

        if ( keystate[SDL_SCANCODE_SPACE] ) {
          if (shootFlag)
          {
            sound[0];
            static_cast<ShootingSprite*>(player)->shootRight();
          }
          else
          {
            sound[0];
            static_cast<ShootingSprite*>(player)->shoot();
          }
        }

        if (keystate[SDL_SCANCODE_G] && !godFlag)
        {

          godFlag = true;
        }
        else if(keystate[SDL_SCANCODE_G] && godFlag) {
          godFlag = false;
        }



        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if (keystate[SDL_SCANCODE_F1] && !displayHud)
        {
          displayHud = true;
        }
        else if(keystate[SDL_SCANCODE_F1] && displayHud) {
          displayHud = false;
        }

        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      
      if (keystate[SDL_SCANCODE_A]) {
        shootFlag = false;
        player->left();
      }

      if (keystate[SDL_SCANCODE_D]) {
        shootFlag = true;
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
