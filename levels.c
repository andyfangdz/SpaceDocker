#include "images.h"
#include "myLib.h"

GameLevel getLevel(int level) {
  Vector2f satellitePos = {0}, stationPos = {0}, startVelocity = {0};
  int maxFuel;
  GameLevel ret;
  switch (level) {
    case 1:
      satellitePos.x = intToFixed(80);
      satellitePos.y = intToFixed(120);
      stationPos.x = intToFixed(80);
      stationPos.y = intToFixed(240 - STATION_HEIGHT / 2);
      maxFuel = 100;
      break;
    case 2:
      satellitePos.x = intToFixed(50);
      satellitePos.y = intToFixed(120);
      stationPos.x = intToFixed(80);
      stationPos.y = intToFixed(240 - STATION_HEIGHT / 2);
      maxFuel = 100;
      break;
    case 3:
      satellitePos.x = intToFixed(50);
      satellitePos.y = intToFixed(120);
      stationPos.x = intToFixed(100);
      stationPos.y = intToFixed(120 + STATION_HEIGHT / 2);
      maxFuel = 100;
      break;
    case 4:
      satellitePos.x = intToFixed(100);
      satellitePos.y = intToFixed(180);
      stationPos.x = intToFixed(100);
      stationPos.y = intToFixed(100);
      maxFuel = 100;
      break;
    default:
      break;
  }
  ret.satellitePos = satellitePos;
  ret.stationPos = stationPos;
  ret.startVelocity = startVelocity;
  ret.maxFuel = maxFuel;
  ret.levelId = level;
  return ret;
}