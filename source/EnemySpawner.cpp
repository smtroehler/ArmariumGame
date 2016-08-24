#include "EnemySpawner.h"
#include "Bookmonster.h"
#include "Program.h"
EnemySpawner::EnemySpawner(){
    totalTime = 0;
}


//Must be called before populate!
void EnemySpawner::addShape(ShapeObj *e){
    beastiary.push_back(e);
    // printf("size: %d\n", (int)beastiary.size());
}
extern Program defferedShader;
void EnemySpawner::populate(GameObject *playerObject, SceneObject *sceneObject){
    waves.clear();
    
    //Make some waves!  Need unique enemy objects in the list.
    ShapeObj *monster;
    monster = new ShapeObj();
    monster->loadSmd("Assets/smd_models/bookmonster/bookmonster.smd");
    monster->loadSmdAnim("Assets/smd_models/bookmonster/animations/bookmonster_running.smd");
    monster->loadSmdAnim("Assets/smd_models/bookmonster/animations/dance.smd");
    monster->loadSmdAnim("Assets/smd_models/bookmonster/animations/DEATH.smd");
    monster->setTexture("Assets/Textures/bookmonster_diffuse.jpg");
  

    monster->init();

    int waveTime = 10;
    //Generate waves.  One every wavetime seconds for 5 minutes.
    for (int i = 0; i <= 300; i += waveTime){
        if (i > 240) { waveTime = 6; }
        else if (i > 180) { waveTime = 7; }
        else if (i > 120) { waveTime = 8; }
        else if (i > 60) { waveTime = 9; }
        

        Wave tempWave;
        tempWave.time = i;
        tempWave.enemies.push_back(new Bookmonster());
        Bookmonster *e = dynamic_cast<Bookmonster *>(tempWave.enemies[tempWave.enemies.size() - 1]);
        e->init(monster, Eigen::Vector3f(20, 2, -14), 0, 0, 0, 0.08, true);
        e->ghettoCalcPath();
        e->setReferences(playerObject, sceneObject);
        e->bindProg(&defferedShader);
        e->setLife(1.0);

        tempWave.enemies.push_back(new Bookmonster());
        e = dynamic_cast<Bookmonster *>(tempWave.enemies[tempWave.enemies.size() - 1]);
        e->init(monster, Eigen::Vector3f(-20, 2, -14), 0, 0, 0, 0.08, true);
        e->ghettoCalcPath();
        e->setReferences(playerObject, sceneObject);
        e->bindProg(&defferedShader);
        e->setLife(1.0);

        tempWave.enemies.push_back(new Bookmonster());
        e = dynamic_cast<Bookmonster *>(tempWave.enemies[tempWave.enemies.size() - 1]);
        e->init(monster, Eigen::Vector3f(25, 2, 4), 0, 0, 0, 0.08, true);
        e->ghettoCalcPath();
        e->setReferences(playerObject, sceneObject);
        e->bindProg(&defferedShader);
        e->setLife(1.0);

        tempWave.enemies.push_back(new Bookmonster());
        e = dynamic_cast<Bookmonster *>(tempWave.enemies[tempWave.enemies.size() - 1]);
        e->init(monster, Eigen::Vector3f(-25, 2, 0), 0, 0, 0, 0.08, true);
        e->ghettoCalcPath();
        e->setReferences(playerObject, sceneObject);
        e->bindProg(&defferedShader);
        e->setLife(1.0);


        waves.push_back(tempWave);
    }
    //printf("enemies: %d\n", tempEnemies.size());
}


void EnemySpawner::update(float time, vector<GameObject *> *mainList){
    //check to see if time corresponds to next in list
    //printf("waves remaining: %d\n", waves.size());
    if (waves.size() != 0 && waves.front().time < time){
        //if it is, point enemies to mainList
        for (int i = 0; i < waves.front().enemies.size(); i++){
            mainList->push_back(waves.front().enemies[i]);
            //Initialize pathing here.
        }
        //pop
        waves.erase(waves.begin());
    }

}

void EnemySpawner::destroy() {
    while (waves.size() != 0) {
        while (waves[0].enemies.size() != 0) {
            waves[0].enemies.erase(waves[0].enemies.begin());

        }
        waves.erase(waves.begin());
    }
}