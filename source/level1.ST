// for the shapes, it goes name of obj file, the texture file, then the lightmap file
// the textures are optional
shape_names

//0
Assets/Desk.obj 0 0 0 
//1
Assets/Bookshelf1.obj 0 0 0
//2
Assets/square.obj Assets/Textures/stonewall1.png Assets/Textures/stonewall1_lightmap.png  0 //Assets/Textures/stonewall1_normals.png
//3
Assets/crystal.obj 0 0 0
//4
Assets/Pylon.obj Assets/Textures/pylon_diffuse.jpg Assets/Textures/pylon_lightmap.jpg 0 
//5
Assets/WoodPillar.obj Assets/Textures/wood_beam_diffuse.jpg 0 0
//6
Assets/square.obj Assets/Textures/floor_wood.jpg 0 0
//7
Assets/FloorEnd.obj Assets/Textures/floor_wood.jpg 0 0
//8 
Assets/bookstand.obj Assets/Textures/book_stand_diffuse.jpg 0 0 
//9 
Assets/bookcase.obj Assets/Textures/bookcase_1.jpg 0 0
//10 
Assets/bookcase2.obj Assets/Textures/bookcase_1.jpg 0 0
//11 
Assets/bookcase3.obj Assets/Textures/bookcase_1.jpg 0 0
//12 
Assets/bookcase4.obj Assets/Textures/bookcase_1.jpg 0 0
//13
Assets/elephant_trophy.obj Assets/Textures/elephant_head.jpg 0 0
//14
Assets/placeholderPile.obj 0 0 0
//15
Assets/rockwall.obj Assets/Textures/stone_diff.jpg 0 0



prop_data

# shapeindex, xpos, ypos, zpos, xrot, yrot, zrot, scale

//Spawn points.  Can be doors in the future or something
//14 25  0   4 0 0 0 1
//14 20  0 -14 0 0 0 1
//14 -25 0   0 0 0 0 1
//14 -20 0 -14 0 0 0 1

//Test crystal
//3  5 1 0 90 0 0 0.3

//pylon things
4   0  0  10 0 180 0 .7
4  20  0   8 0 180 0 .7
4 -20  0   3 0 180 0 .7
4  -3  0 -6 0 180 0 .7
4  12  0 -7 0 180 0 .7

// book cases.  10, 11, 12 are all wide
//wall
10 10 0 -14 0 0 0 1
11 4 0 -14 0 0 0 1
12 0 0 -14 0 0 0 1
9 -4 0 -14 0 0 0 1
9 -6 0 -14 0 0 0 1

//left side
10 -10 0 -7 0 0 0 1
11 -14 0 -7 0 0 0 1
10 -18 0 -7 0 0 0 1

10 -3 0 -9 0 90 0 1

11 -22 0 -1 0 90 0 1
10 -22 0  7 0 90 0 1
12 -22 0  3 0 90 0 1

10 -10 0 9 0 90 0 1
11 -10 0 13 0 90 0 1

//right side
11 5 0 -7 0 0 0 1
10 9 0 -7 0 0 0 1
11 15 0 -7 0 0 0 1
12 19 0 -7 0 0 0 1

10 22 0 4 0 90 0 1
12 22 0 0 0 90 0 1

10 10 0 9 0 90 0 1
10 10 0 13 0 90 0 1

// book holder
8 0 -1 0 0 0 0 .5

//elephants
13 10 5 -15 0 0 0 .3   
13 -10 5 -15 0 0 0 .3   

// side 1: back
// walls
15 0 2.3 -15 0 0 0 5 
15 5 2.3 -15 0 0 0 5 
15 10 2.3 -15 0 0 0 5 
15 15 2.3 -15 0 0 0 5 
15 20 2.3 -15 0 0 0 5 
15 25 2.3 -15 0 0 0 5 

15 -5 2.3 -15 0 0 0 4.55
15 -10 2.3 -15 0 0 0 4.55 
15 -15 2.3 -15 0 0 0 4.55 
15 -20 2.3 -15 0 0 0 4.55 
15 -25 2.3 -15 0 0 0 4.55 

15 0 7.5 -15 0 0 0 4.55 
15 5 7.5 -15 0 0 0 4.55
15 10 7.5 -15 0 0 0 4.55 
15 15 7.5 -15 0 0 0 4.55 
15 20 7.5 -15 0 0 0 4.55
15 25 7.5 -15 0 0 0 4.55


15 -5 7.5 -15 0 0 0 4.55
15 -10 7.5 -15 0 0 0 4.55 
15 -15 7.5 -15 0 0 0 4.55
15 -20 7.5 -15 0 0 0 4.55
15 -25 7.5 -15 0 0 0 4.55

// beams
5 -27.5 0 -15 0 0 0 1
5 -22.5 0 -15 0 0 0 1
5 -17.5 0 -15 0 0 0 1
5 -12.5 0 -15 0 0 0 1
5 -7.5 0 -15 0 0 0 1
5 -2.5 0 -15 0 0 0 1
5 2.5 0 -15 0 0 0 1
5 7.5 0 -15 0 0 0 1
5 12.5 0 -15 0 0 0 1
5 17.5 0 -15 0 0 0 1
5 22.5 0 -15 0 0 0 1
5 27.5 0 -15 0 0 0 1

5 -27.5 5 -15 0 0 0 1
5 -22.5 5 -15 0 0 0 1
5 -17.5 5 -15 0 0 0 1
5 -12.5 5 -15 0 0 0 1
5 -7.5 5 -15 0 0 0 1
5 -2.5 5 -15 0 0 0 1
5 2.5 5 -15 0 0 0 1
5 7.5 5 -15 0 0 0 1
5 12.5 5 -15 0 0 0 1
5 17.5 5 -15 0 0 0 1
5 22.5 5 -15 0 0 0 1
5 27.5 5 -15 0 0 0 1

// sideways beams
5 -27.5 5 -15.2 0 0 90 1
5 -22.5 5 -15.2 0 0 90 1
5 -17.5 5 -15.2 0 0 90 1
5 -12.5 5 -15.2 0 0 90 1
5 -7.5 5 -15.2 0 0 90 1
5 -2.5 5 -15.2 0 0 90 1
5 2.5 5 -15.2 0 0 90 1
5 7.5 5 -15.2 0 0 90 1
5 12.5 5 -15.2 0 0 90 1
5 17.5 5 -15.2 0 0 90 1
5 22.5 5 -15.2 0 0 90 1
5 27.5 5 -15.2 0 0 90 1


//side 3: right
15 27.5 2.5 -15 0 -90 0 5 
15 27.5 2.5 -10 0 -90 0 5 
15 27.5 2.5 -5 0 -90 0 5 
15 27.5 2.5  0 0 -90 0 5 

15 27.5 2.5 5 0 -90 0 5 
15 27.5 2.5 10 0 -90 0 5 
15 27.5 2.5 15 0 -90 0 5 

15 27.5 7.5 -15 0 -90 0 5 
15 27.5 7.5 -10 0 -90 0 5 
15 27.5 7.5 -5 0 -90 0 5 
15 27.5 7.5  0 0 -90 0 5 


15 27.5 7.5 5 0 -90 0 5 
15 27.5 7.5 10 0 -90 0 5 
15 27.5 7.5 15 0 -90 0 5 


// side 4: left

15 -27.5 2.5 -15 0 90 0 5 
15 -27.5 2.5 -10 0 90 0 5 
15 -27.5 2.5 -5 0 90 0 5 
15 -27.5 2.5  0 0 90 0 5 

15 -27.5 2.5 5 0 90 0 5 
15 -27.5 2.5 10 0 90 0 5 
15 -27.5 2.5 15 0 90 0 5 

15 -27.5 7.5 -15 0 90 0 5 
15 -27.5 7.5 -10 0 90 0 5 
15 -27.5 7.5 -5 0 90 0 5 
15 -27.5 7.5  0 0 90 0 5 

15 -27.5 7.5 5 0 90 0 5 
15 -27.5 7.5 10 0 90 0 5 
15 -27.5 7.5 15 0 90 0 5 


// floor

6 -25 0 0 0 0 0 5
6 -20 0 0 0 0 0 5
6 -15 0 0 0 0 0 5
6 -10 0 0 0 0 0 5
6 -5 0 0 0 0 0 5
6 0 0 0 0 0 0 5
6 5 0 0 0 0 0 5
6 10 0 0 0 0 0 5
6 15 0 0 0 0 0 5
6 20 0 0 0 0 0 5
6 25 0 0 0 0 0 5

6 -25 0 5 0 0 0 5
6 -20 0 5 0 0 0 5
6 -15 0 5 0 0 0 5
6 -10 0 5 0 0 0 5
6 -5 0 5 0 0 0 5
6 0 0 5 0 0 0 5
6 5 0 5 0 0 0 5
6 10 0 5 0 0 0 5
6 15 0 5 0 0 0 5
6 20 0 5 0 0 0 5
6 25 0 5 0 0 0 5

6 -25 0 10 0 0 0 5
6 -20 0 10 0 0 0 5
6 -15 0 10 0 0 0 5
6 -10 0 10 0 0 0 5
6 -5 0 10 0 0 0 5
6 0 0 10 0 0 0 5
6 5 0 10 0 0 0 5
6 10 0 10 0 0 0 5
6 15 0 10 0 0 0 5
6 20 0 10 0 0 0 5
6 25 0 10 0 0 0 5

6 -25 0 -5 0 0 0 5
6 -20 0 -5 0 0 0 5
6 -15 0 -5 0 0 0 5
6 -10 0 -5 0 0 0 5
6 -5 0 -5 0 0 0 5
6 0 0 -5 0 0 0 5
6 5 0 -5 0 0 0 5
6 10 0 -5 0 0 0 5
6 15 0 -5 0 0 0 5
6 20 0 -5 0 0 0 5
6 25 0 -5 0 0 0 5

6 -25 0 -10 0 0 0 5
6 -20 0 -10 0 0 0 5
6 -15 0 -10 0 0 0 5
6 -10 0 -10 0 0 0 5
6 -5 0 -10 0 0 0 5
6 0 0 -10 0 0 0 5
6 5 0 -10 0 0 0 5
6 10 0 -10 0 0 0 5
6 15 0 -10 0 0 0 5
6 20 0 -10 0 0 0 5
6 25 0 -10 0 0 0 5

6 -25 0 -15 0 0 0 5
6 -20 0 -15 0 0 0 5
6 -15 0 -15 0 0 0 5
6 -10 0 -15 0 0 0 5
6 -5 0 -15 0 0 0 5
6 0 0 -15 0 0 0 5
6 5 0 -15 0 0 0 5
6 10 0 -15 0 0 0 5
6 15 0 -15 0 0 0 5
6 20 0 -15 0 0 0 5
6 25 0 -15 0 0 0 5

7 -25 0 15 0 00 0 5
7 -20 0 15 0 00 0 5
7 -15 0 15 0 00 0 5
7 -10 0 15 0 00 0 5
7 -5 0 15 0 00 0 5
7 0 0 15 0 00 0 5
7 5 0 15 0 00 0 5
7 10 0 15 0 00 0 5
7 15 0 15 0 00 0 5
7 20 0 15 0 00 0 5
7 25 0 15 0 00 0 5

