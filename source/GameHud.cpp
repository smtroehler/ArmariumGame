#include "GameHud.h"
#include <iostream>
#include "PlayerObject.h"

using namespace std;

extern PlayerObject player;

GameHud::GameHud() {
   used[0] = false;
   used[1] = false;
   used[2] - false;
}

void GameHud::load(int w, int h) {
   HudShader.setShaderNames("Shaders/hud_vert.glsl", "Shaders/hud_frag.glsl");

   portrait.loadFile("Assets/square.obj");
   portrait.setWH(w,h);
   portrait.setTextureName("Assets/HUD/wizard_face_hud.jpg");
   portrait.setBlendName("Assets/HUD/wizard_face_hud_overla.jpg");
 
   healthBar.loadFile("Assets/square2.obj");
   healthBar.setWH(w,h);
   healthBar.setTextureName("Assets/HUD/health_bar.jpg");
   healthBar.setBlendName("Assets/HUD/health_bar_overlay.jpg");

   healthBarEndL.loadFile("Assets/square.obj");
   healthBarEndL.setWH(w,h);
   healthBarEndL.setTextureName("Assets/HUD/health_bar_end.jpg");
   healthBarEndL.setBlendName("Assets/HUD/health_bar_end_overlay.jpg");

   healthBarEndR.loadFile("Assets/square.obj");
   healthBarEndR.setWH(w,h);
   healthBarEndR.setTextureName("Assets/HUD/health_bar_end.jpg");
   healthBarEndR.setBlendName("Assets/HUD/health_bar_end_overlay.jpg");


   healthBarBG.loadFile("Assets/square.obj");
   healthBarBG.setWH(w,h);
   healthBarBG.setTextureName("Assets/HUD/health_bar_bg.jpg");
   healthBarBG.setBlendName("Assets/HUD/health_bar_bg_overlay.jpg");

   spell1.loadFile("Assets/square.obj");
   spell1.setWH(w,h);
   spell1.setTextureName("Assets/HUD/spell_icon.jpg");
   spell1.setBlendName("Assets/HUD/spell_icon_blend.jpg");

   spell2.loadFile("Assets/square.obj");
   spell2.setWH(w,h);
   spell2.setTextureName("Assets/HUD/spell_icon_1.jpg");
   spell2.setBlendName("Assets/HUD/spell_icon_blend.jpg");

   spell3.loadFile("Assets/square.obj");
   spell3.setWH(w,h);
   spell3.setTextureName("Assets/HUD/spell3.jpg");
   spell3.setBlendName("Assets/HUD/spell_icon_blend.jpg");


   spell1Used.loadFile("Assets/square.obj");
   spell1Used.setWH(w,h);
   spell1Used.setTextureName("Assets/HUD/spell_icon_used.jpg");
   spell1Used.setBlendName("Assets/HUD/spell_icon_blend.jpg");

   spell2Used.loadFile("Assets/square.obj");
   spell2Used.setWH(w,h);
   spell2Used.setTextureName("Assets/HUD/spell_icon_1_used.jpg");
   spell2Used.setBlendName("Assets/HUD/spell_icon_blend.jpg");

   spell3Used.loadFile("Assets/square.obj");
   spell3Used.setWH(w,h);
   spell3Used.setTextureName("Assets/HUD/spell3_used.jpg");
   spell3Used.setBlendName("Assets/HUD/spell_icon_blend.jpg");


   gameOver.loadFile("Assets/square.obj");
   gameOver.setWH(w,h);
   gameOver.setTextureName("Assets/HUD/game_over.jpg");
   gameOver.setBlendName("Assets/HUD/game_over_overlay.jpg");

   gameWon.loadFile("Assets/square.obj");
   gameWon.setWH(w, h);
   gameWon.setTextureName("Assets/HUD/game_won.jpg");
   gameWon.setBlendName("Assets/HUD/game_won_overlay.jpg");

   restart.loadFile("Assets/square.obj");
   restart.setWH(w,h);
   restart.setTextureName("Assets/HUD/restart.jpg");
   restart.setBlendName("Assets/HUD/restart_overlay.jpg");

   BookHealthBar.loadFile("Assets/square2.obj");
   BookHealthBar.setWH(w,h);
   BookHealthBar.setTextureName("Assets/HUD/health_bar.jpg");
   BookHealthBar.setBlendName("Assets/HUD/health_bar_overlay.jpg");

   BookHealthBarEndL.loadFile("Assets/square.obj");
   BookHealthBarEndL.setWH(w,h);
   BookHealthBarEndL.setTextureName("Assets/HUD/health_bar_end.jpg");
   BookHealthBarEndL.setBlendName("Assets/HUD/health_bar_end_overlay.jpg");

   BookHealthBarEndR.loadFile("Assets/square.obj");
   BookHealthBarEndR.setWH(w,h);
   BookHealthBarEndR.setTextureName("Assets/HUD/health_bar_end.jpg");
   BookHealthBarEndR.setBlendName("Assets/HUD/health_bar_end_overlay.jpg");
   
   BookHealthBarBG.loadFile("Assets/square.obj");
   BookHealthBarBG.setWH(w,h);
   BookHealthBarBG.setTextureName("Assets/HUD/health_bar_bg.jpg");
   BookHealthBarBG.setBlendName("Assets/HUD/health_bar_bg_overlay.jpg");

   bookPortrait.loadFile("Assets/square.obj");
   bookPortrait.setWH(w,h);
   bookPortrait.setTextureName("Assets/HUD/book_rocks.jpg");
   bookPortrait.setBlendName("Assets/HUD/book_rocks_overlay.jpg");

   width = w;
   height = h;

   oldHp = player.getLife();
   maxHp = oldHp = player.getLife();

   BookMaxHp = BookOldHp = player.getBookHealth();

}


void GameHud::setup() {

 

   portrait.init();   
   healthBar.init();
   healthBarEndL.init();
   healthBarEndR.init();
   healthBarBG.init();
   spell1.init();
   spell2.init();
   spell3.init();
   spell3Used.init();

   spell1Used.init();
   gameOver.init();
   gameWon.init();
   restart.init();
   spell2Used.init();

   BookHealthBarEndL.init();
   BookHealthBarEndR.init();
   BookHealthBarBG.init();
   BookHealthBar.init();
   bookPortrait.init();

 
   HudShader.init();
   HudShader.addUniform("P");
   HudShader.addUniform("MV");
   HudShader.addAttribute("vertPos");
   HudShader.addAttribute("vertTex");
   HudShader.addUniform("texture0");
   HudShader.addUniform("texture1");

 
   portrait.bindProg(&HudShader);
   healthBar.bindProg(&HudShader);
   healthBarEndL.bindProg(&HudShader);
   healthBarEndR.bindProg(&HudShader);
   healthBarBG.bindProg(&HudShader);
   spell1.bindProg(&HudShader);
   spell2.bindProg(&HudShader);
   spell3.bindProg(&HudShader);
   spell1Used.bindProg(&HudShader);
   spell2Used.bindProg(&HudShader);
   spell3Used.bindProg(&HudShader);
   gameOver.bindProg(&HudShader);
   gameWon.bindProg(&HudShader);
   restart.bindProg(&HudShader);
   BookHealthBar.bindProg(&HudShader);
   BookHealthBarBG.bindProg(&HudShader);
   BookHealthBarEndR.bindProg(&HudShader);
   BookHealthBarEndL.bindProg(&HudShader);
   bookPortrait.bindProg(&HudShader);

   portrait.pos = Eigen::Vector3f(100, 100, 1);
   portrait.scale = Eigen::Vector3f(250, 250, 1);
   portrait.rot(0) = M_PI /2;  //warning trunc from double to float
   
   hpBarPos = healthBar.pos = Eigen::Vector3f(105, 50, 3);
   healthBar.scale = Eigen::Vector3f(300, 800, 1);
   hpBarLen = 800 * .5;
   healthBar.rot(0) = M_PI /2;  //warning trunc from double to float
   healthBar.rescale();

   healthBarEndR.pos = Eigen::Vector3f(410, 50, 4);
   healthBarEndR.scale = Eigen::Vector3f(800, 800, 1);
   healthBarEndR.rot(0) = M_PI /2 ;

   healthBarEndL.pos = Eigen::Vector3f(100, 50, 4);
   healthBarEndL.scale = Eigen::Vector3f(800, 800, 1);
   healthBarEndL.rot(0) = M_PI /2 ;
   healthBarEndL.mirror();

   healthBarBG.pos = Eigen::Vector3f(250, 50, -2);
   healthBarBG.scale = Eigen::Vector3f(500, 800, 1);
   healthBarBG.rot(0) = M_PI /2;  //warning trunc from double to float

   float SpellPositioning = width/2 ;
   spell1.pos = Eigen::Vector3f(SpellPositioning - 120, 75, 1);
   spell1.scale = Eigen::Vector3f(100, 100, 1);
   spell1.rot(0) = M_PI /2;  //warning trunc from double to float

   spell1Used.pos = Eigen::Vector3f(SpellPositioning - 120, 75, 1);
   spell1Used.scale = Eigen::Vector3f(100, 100, 1);
   spell1Used.rot(0) = M_PI /2;  //warning trunc from double to float

   spell2.pos = Eigen::Vector3f(SpellPositioning + 0, 75, 1);
   spell2.scale = Eigen::Vector3f(100, 100, 1);
   spell2.rot(0) = M_PI /2;  //warning trunc from double to float

   spell3.pos = Eigen::Vector3f(SpellPositioning + 120, 75, 1);
   spell3.scale = Eigen::Vector3f(100, 100, 1);
   spell3.rot(0) = M_PI /2;  //warning trunc from double to float

   spell3Used.pos = Eigen::Vector3f(SpellPositioning + 120, 75, 1);
   spell3Used.scale = Eigen::Vector3f(100, 100, 1);
   spell3Used.rot(0) = M_PI /2;  //warning trunc from double to float
   
   spell2Used.pos = Eigen::Vector3f(SpellPositioning + 0, 75, 1);
   spell2Used.scale = Eigen::Vector3f(100, 100, 1);
   spell2Used.rot(0) = M_PI /2;  //warning trunc from double to float

   gameOver.pos = Eigen::Vector3f(width/2.0, height/2.0, 1);
   gameOver.scale = Eigen::Vector3f(600, 600, 1);
   gameOver.rot(0) = M_PI /2.0;

   gameWon.pos = Eigen::Vector3f(width / 2.0, height / 2.0, 1);
   gameWon.scale = Eigen::Vector3f(600, 600, 1);
   gameWon.rot(0) = M_PI / 2.0;

   restart.pos = Eigen::Vector3f(width/2.0, height/2.0 - 200, 1);
   restart.scale = Eigen::Vector3f(400, 400, 1);
   restart.rot(0) = M_PI /2.0;

   float ofst = width - 500;
   float ofsty = 50;
   BookHealthBar.pos = Eigen::Vector3f(ofst + 105, ofsty, 3);
   BookHealthBar.scale = Eigen::Vector3f(300, 800, 1);
   BookHealthBar.rot(0) = M_PI /2;  //warning trunc from double to float
   BookHealthBar.rescale();

   BookHealthBarEndR.pos = Eigen::Vector3f(ofst + 410, ofsty, 4);
   BookHealthBarEndR.scale = Eigen::Vector3f(800, 800, 1);
   BookHealthBarEndR.rot(0) = M_PI /2 ;

   BookHealthBarEndL.pos = Eigen::Vector3f(ofst + 100, ofsty, 4);
   BookHealthBarEndL.scale = Eigen::Vector3f(800, 800, 1);
   BookHealthBarEndL.rot(0) = M_PI /2 ;
   BookHealthBarEndL.mirror();

   BookHealthBarBG.pos = Eigen::Vector3f(ofst + 250, ofsty, -2);
   BookHealthBarBG.scale = Eigen::Vector3f(500, 800, 1);
   BookHealthBarBG.rot(0) = M_PI /2;  //warning trunc from double to float

   bookPortrait.pos = Eigen::Vector3f(ofst + 400, 100, 1);
   bookPortrait.scale = Eigen::Vector3f(220, 220, 1);
   bookPortrait.rot(0) = M_PI /2;  //warning trunc from double to float
}




void GameHud::draw(MatrixStack *P, MatrixStack *MV, PlayerObject *playerPtr, float time, GLFWwindow* window) {

   portrait.draw(P,MV);
   
   float life = playerPtr->getLife();
   float bookLife = playerPtr->getBookHealth();

   float bar_scale = 300 * life / maxHp;
   if(bar_scale != healthBar.scale(0) && life >= 0) {
      float old_scale = healthBar.scale(0);
      healthBar.scale(0) = bar_scale;
      float diff = old_scale - healthBar.scale(0);
      healthBarEndR.pos(0) -= diff;
      healthBar.rescale();
   }

   float bookBarScale = 300 * bookLife / BookMaxHp;
   if(bookBarScale != BookHealthBar.scale(0) && bookLife >= 0) {
      float old_scale = BookHealthBar.scale(0);
      BookHealthBar.scale(0) = bookBarScale;
      float diff = old_scale - BookHealthBar.scale(0);
      BookHealthBarEndR.pos(0) -= diff;
      BookHealthBar.rescale();
   }


   
   healthBarBG.draw(P,MV);
   if(life > 0) {
       healthBarEndL.draw(P,MV);
       healthBarEndR.draw(P,MV);
       healthBar.draw(P,MV);
   }

   bookPortrait.draw(P,MV);
   BookHealthBarBG.draw(P,MV);
   if(bookLife > 0) {
       BookHealthBarEndL.draw(P,MV);
       BookHealthBarEndR.draw(P,MV);
       BookHealthBar.draw(P,MV);
   }
   
   if (playerPtr->cooldowns[0] > 0.0f){
      spell1Used.draw(P, MV);
   }
   else {
   spell1.draw(P,MV);
   }

   if(playerPtr->cooldowns[1] > 0.0f)
   {
      spell2Used.draw(P,MV);
   }
   else {
   spell2.draw(P,MV);
   }
    
   if(playerPtr->cooldowns[2] > 0.0f)
   {
       spell3Used.draw(P,MV);
   }
   else {
       spell3.draw(P,MV);
   }

   if(bookLife <=0 || life <= 0) {
       gameOver.draw(P,MV);
       restart.draw(P,MV);
   }
   else if (time >= 320 || glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
       cout << time << endl;
       gameWon.draw(P, MV);
       restart.draw(P, MV);
   }

}