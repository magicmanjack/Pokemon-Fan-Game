#include "transform.h"
#include <cmath>
#include <algorithm>

#define PI 3.14159265
#define MAGIC_ANGLE (90.0 - 35.264)

double Transform::offsetX;
double Transform::offsetY;
double Transform::offsetZ;

void Transform::transform(double in[3]) {
    double rotateZ[3][3] = {{sin(45.0 * PI/180.0), -sin(45.0 * PI/180.0), 0.0},
							{cos(45.0 * PI/180.0), cos(45.0 * PI/180.0), 0.0},
							{0.0, 0.0, 0.0}};
	//^^Rotates 3D points 45 degrees around the Z-axis.
	double rotateX[3][3] = {{1.0, 0.0, 0.0},
							{0.0, cos(MAGIC_ANGLE * PI/180.0), -sin(MAGIC_ANGLE * PI/180.0)},
							{0.0, sin(MAGIC_ANGLE * PI/180.0), cos(MAGIC_ANGLE * PI/180.0)}};
	//^^Rotates 3D points (90.0 - 35.264) degrees around the X-axis.
	double operation1[3] = {0.0, 0.0, 0.0};
	for(int row = 0; row < 3; row++) {
		for(int column = 0; column < 3; column++) {
			operation1[row] += rotateZ[row][column] * in[column];
		}
	} 
	double operation2[3] = {0.0, 0.0, 0.0};
	for(int row = 0; row < 3; row++) {
		for(int column = 0; column < 3; column++) {
			operation2[row] += rotateX[row][column] * operation1[column];
		}
	}
	std::copy(operation2, operation2 + 3, in);
}

void Transform::offset(double in[3]) {
    in[0] += offsetX;
    in[1] += offsetY;
    in[2] += offsetZ;
    //^^The 3D point gets offset by the offset values.
}
 
