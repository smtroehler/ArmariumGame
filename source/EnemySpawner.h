#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN

#include "Enemy.h"
#include "../dependencies/Eigen/Dense"
#include "../dependencies/Eigen/Geometry"
#include "../dependencies/Eigen/LU"
#include <vector>

//Demines n enemies with n starting positions
typedef struct Wave{
    float time;
    std::vector<Enemy *> enemies;
} Wave;


class EnemySpawner
{
public:
    EnemySpawner();
    void addShape(ShapeObj *e);
    void populate(GameObject *playerObject, SceneObject *sceneObject);
    void update(float timestep, vector<GameObject *> *mainList);
    void destroy();
private:
    float totalTime;
    std::vector<Wave> waves;
    std::vector<ShapeObj*> beastiary; //List of enemy templates.  It's a cool name, I swear
};
