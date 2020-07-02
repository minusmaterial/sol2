#pragma once
#include <stdio.h>
#include <cglm/cglm.h>

int crondel(int a, int b);

int levi_civita(int a, int b, int c);

int intlog2(int x);

void mat4_to_vec3(mat4 * mat, vec3 * dest);
void mat4_to_vec4(mat4 * mat, vec4 * dest);

void mat4_rotate(vec4 axis, float angle, mat4 dest);
