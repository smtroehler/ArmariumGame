#pragma once
#ifndef _GAMEHUD_H_
#define _GAMEHUD_H_

#include "HudObject.h"
#include "PlayerObject.h"

class GameHud {
   public:
      GameHud();
      void setup();

      void draw(MatrixStack *P, MatrixStack *MV, PlayerObject *playerPtr, float time, GLFWwindow* window);

      void load(int w, int h);
      bool used[3];
   private:
      Program HudShader;
      HudObject portrait;
      HudObject healthBar;
      HudObject healthBarEndL;
      HudObject healthBarEndR;
      HudObject healthBarBG;
      HudObject spell1;
      HudObject spell2;
      HudObject spell3;
      HudObject spell1Used;
      HudObject spell2Used;
      HudObject spell3Used;
      HudObject gameOver;
      HudObject gameWon;
      HudObject restart;

      HudObject BookHealthBar;
      HudObject BookHealthBarEndL;
      HudObject BookHealthBarEndR;
      HudObject BookHealthBarBG;
      HudObject bookPortrait;

      Eigen::Vector3f hpBarPos;
      float hpBarLen;
      float oldHp;
	  float maxHp;

      float BookOldHp;
      float BookMaxHp;

      int  width, height;
};

#endif