//3350
//program: rainforest.cpp
//author:  Gordon Griesel
//date:    2013 to 2021
//
//to do list:
// 1. need credits for student work in get-alpha-channel function.
//    done
//
//This program demonstrates the use of OpenGL and XWindows
//
//Texture maps are displayed.
//Press B to see bigfoot roaming his forest.
//

void wetness level(float color) {
    color[1] += 0.01;
    color[2] -= 0.01;
    if (color[1] > 1.0) {
	color[1] = 1.0
    }
    if (color[2] > 1.0) {
	color[2] =1.0;
    }

}
