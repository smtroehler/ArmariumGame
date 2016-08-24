#include "SceneObject.h"
#include <iostream>
#include <fstream>
#include <string>
SceneObject::SceneObject()
{

}

void SceneObject::init() {
    for (int i = 0; i < shapes.size(); i++) {
        shapes[i]->init();
        shapes[i]->setMat(MAT_BLUE);
    }
}

void SceneObject::bindProg(Program *prog) {
    for (int i = 0; i < shapes.size(); i++) {
        shapes[i]->bindProg(prog);
    }
}

void SceneObject::shadowDraw(MatrixStack *mv) {
    for (int i = 0; i < props.size() - 1; i++) { //added -1 because a gaint table was displaying in windows
        props[i]->shadowDraw(mv);
    }
}

void SceneObject::draw(MatrixStack *mv) {
    for (int i = 0; i < props.size() - 1; i++) { //added -1 because a gaint table was displaying in windows
        props[i]->draw(mv);
    }
}

//Add corresponding GameObject to tree
void SceneObject::populateTree(QuadTree *tree){
    //int j = 0;
    for (int i = 0; i < props.size(); i++){
        if (props[i]->getCollidable() == true)
            tree->insert(props[i]);
    }
}

void SceneObject::drawBounding(MatrixStack *mv, ShapeObj *sphere) {
    for (int i = 0; i < props.size() - 1; i++) { //added -1 because a gaint table was displaying in windows
        if (props[i]->getShape() != shapes[6] && props[i]->getShape() != shapes[2]){
            props[i]->draw(mv);
            props[i]->drawBounding(mv, sphere);
        }
    }
}

void SceneObject::loadFile(const std::string &fileName) {
    std::ifstream readFrom;
    std::string line;
    readFrom.open(fileName);


    if (readFrom.is_open()) {
        char buffer[100];
        readFrom >> buffer;

        //make sure the first few lines aren't comments
        while (!readFrom.eof() && (buffer[0] == '/' || buffer[0] == '#')) {
            getline(readFrom, line);
            readFrom >> buffer;
        }

        if (strcmp(buffer, "shape_names") == 0) {
            int cur = 0;

            // where the models are loaded in
            for (readFrom >> buffer; buffer && strcmp(buffer, "prop_data"); readFrom >> buffer) {
                if (buffer[0] != '/' && buffer[0] != '#') {
                    // where it creates new shape and adds to shape list
                    ShapeObj *s = new ShapeObj();
                    shapes.push_back(s);
                    shapes[cur]->load(buffer);

                    // read in and check texture filename
                    readFrom >> buffer;
                    if (buffer[0] != '0')
                        shapes[cur]->setTexture(buffer);

                    // read in and check lightmap filename
                    readFrom >> buffer;
                    if (buffer[0] != '0')
                        shapes[cur]->setLightmap(buffer);

                    // read in and check bumpmap filename
                    readFrom >> buffer;
                    if (buffer[0] != '0')
                        shapes[cur]->setBumpmap(buffer);

                    cur++;
                }
                else {
                    getline(readFrom, line);
                }
            }

            cur = 0;

            // Where the actual props are read in
            // the props just reference the shapes so it doesn't have to copy it for each one
            while (!readFrom.eof()) {
                readFrom >> buffer;
                if (buffer[0] != '/' && buffer[0] != '#') {
                    GameObject *obj = new GameObject();
                    props.push_back(obj);

                    int shapeNum;
                    float x, y, z, rotx, roty, rotz, scale;

                    shapeNum = atoi(buffer);
                    if (shapeNum >= shapes.size()) {
                        std::cerr << "That is an invalid shape [first number]" << std::endl;
                    }

                    //don't collide with walls and floor
                    bool collide = true;
                    if (shapeNum == 6 || shapeNum == 0 || shapeNum == 2 || shapeNum == 13 || shapeNum == 7){
                        //printf("shapeNum is %d\n", shapeNum);
                        collide = false;
                    }


                    readFrom >> x;
                    readFrom >> y;
                    readFrom >> z;
                    readFrom >> rotx;
                    readFrom >> roty;
                    readFrom >> rotz;
                    readFrom >> scale;

                    props[cur]->init(shapes[shapeNum], Eigen::Vector3f(x, y, z), rotx*M_PI / 180.0, roty*M_PI / 180.0, rotz*M_PI / 180.0, scale, collide);
                    // Pylon
                    if(shapeNum == 4){
                        props[cur]->setUseRadius(true);
                        props[cur]->setRadScaler(0.6);
                    }
                    // Wooden pillar
                    else if(shapeNum == 5){
                        props[cur]->setUseRadius(true);
                        props[cur]->setRadScaler(0.9);
                    }
                    // Bookstand
                    else if(shapeNum == 8){
                        props[cur]->setUseRadius(true);
                        props[cur]->setRadScaler(0.7);
                    }

                    cur++;
                }
                else {
                    getline(readFrom, line);
                }
            }

        }
    }
    else {
        std::cout << "File not Found" << std::endl;
    }

    readFrom.close();
}
