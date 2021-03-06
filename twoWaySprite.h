#ifndef TWOWAYSPRITE__H
#define TWOWAYSPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class TwoWaySprite : public Drawable {
public:
  TwoWaySprite(const std::string&);
  TwoWaySprite(const TwoWaySprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

  virtual void explode();
protected:
  std::vector<Image *> imagesRight;
  std::vector<Image *> imagesLeft;
  std::vector<Image *> imagesShootRight;
  std::vector<Image *> imagesShootLeft;
  std::vector<Image *> images;
  ExplodingSprite* explosion;
  
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  //bool restingContact;

  void advanceFrame(Uint32 ticks);
  TwoWaySprite& operator=(const TwoWaySprite&);
};
#endif
