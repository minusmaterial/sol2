#include "maths.h"

int crondel(int a, int b){
	if( a == b) return 1;
	else return 0;
};

int levi_civita(int a, int b, int c){
//Take 3 numbers, a, b, and c; one has value 1, the other 2, the third 3.  Are they in looping ascending order?  i.e. [123], or [231], or [312]?
	for(int offset = 0; offset < 3; offset++){
		if(((a + offset)%3 == 1)&&((b + offset)%3 == 2)&&((c + offset)%3 == 3)) return 1;
	}
	for(int offset = 0; offset < 3; offset++){
		if(((a + offset)%3 == 1)&&((b + offset)%3 == 3)&&((c + offset)%3 == 2)) return -1;
	}
	return 0;
};

int intlog2(int x){
	int result = 0;
	while(x >>= 1) result++;
	return result;
}

void mat4_to_vec3(mat4 * mat, vec3 * dest){
	for(int i = 0; i < 3; i++){
		(*dest)[i] = (*mat)[3][i];
	}
}

void mat4_to_vec4(mat4 * mat, vec4 * dest){
	for(int i = 0; i < 4; i++){
		(*dest)[i] = (*mat)[3][i];
	}
}

//completely pointless.  Oh well.
void mat4_rotate(vec4 axis, float angle, mat4 dest){
	printf("sin of 0.4: %f\n", sin(0.4));
	float x = axis[0];
	float y = axis[1];
	float z = axis[2];
	//remember, it's column-major
	dest[0][0] = cos(angle) + x*x*(1-cos(angle));
	dest[0][1] = y*x*(1-cos(angle)) + z*sin(angle);
	dest[0][2] = z*x*(1-cos(angle)) - y*sin(angle);
	dest[1][0] = x*y*(1-cos(angle)) - z*sin(angle);
	dest[1][1] = cos(angle) + y*y*(1-cos(angle));
	dest[1][2] = z*y*(1-cos(angle)) + x*sin(angle);
	dest[2][0] = x*z*(1-cos(angle)) + y*sin(angle);
	dest[2][1] = y*z*(1-cos(angle)) - x*sin(angle);
	dest[2][2] = cos(angle) + z*z*(1-cos(angle));
	//now the edges
	dest[0][3] = 0;
	dest[1][3] = 0;
	dest[2][3] = 0;
	dest[3][0] = 0;
	dest[3][1] = 0;
	dest[3][2] = 0;
	dest[3][3] = 1;
};
