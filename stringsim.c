#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct string_package {
  int *old_string;
  int *string;
  int offset;
} stgpk;

void *render_string(stgpk *);
void *string_calculator(stgpk *);
void initialize(stgpk *);

#define LEN 150

const float LENGTH = (float)LEN;
const float HIEGHT = 60;
const float CYCLES = 100;
const float TERMS = 3;
const float DIST = 10;
const float TENSION = 60;
const float LINEAR_DENSITY = 0.01;
const float PI = 3.14159;
const float PLUCK_H = 1.3;

int main(int argc, char **argv) { //=================
  system("clear");

  stgpk pack;
  clock_t start, end;
  double cpu_time_used;
  initialize(&pack);

  for (int iteration = 0; iteration < CYCLES; ++iteration) {
    pack.offset = iteration;
    start = clock();
    string_calculator(&pack);
    end = clock();
    cpu_time_used = (double)(end - start) / CLOCKS_PER_SEC;
    start = clock();
    render_string(&pack);
    end = clock();
    printf("Calculation time: %f\n", cpu_time_used);
    printf("Render time:      %f\n", (double)(end - start) / CLOCKS_PER_SEC);
    for (int i = 0; i < LENGTH; ++i) {
    }
    usleep(100000);
  }
  return 0;
} //=======================================================

void *string_calculator(stgpk *pack) {
  float k = pack->offset;
  float sum;
  for (int i = 0; i < LENGTH; ++i) {
    sum = 0;
    for (int n = 1; n < TERMS + 1; ++n) {
      sum += (2 * PLUCK_H * pow(LENGTH, 2) /
              (pow(n * PI, 2) * DIST * (LENGTH - DIST)) *
              sin(DIST * n * PI / LENGTH)) *
             sin(n * PI * i / LENGTH) *
             cos(PI * n * sqrt(TENSION / LINEAR_DENSITY) * k / LENGTH) *
             HIEGHT / 2;
    }
    sum *= exp(-0.04 * k);
    sum -= (float)HIEGHT / 2;
    pack->string[i] = round(sum);
  }
  return NULL;
}

void *render_string(stgpk *pack) {
  for (int i = 0; i < LENGTH; ++i) {
    printf("\033[%d;%dH \033[%d;%dH*", -pack->old_string[i] + 1, i + 2,
           -pack->string[i] + 1, i + 2);
    pack->old_string[i] = pack->string[i];
  }
  printf("\033[%d;H", (int)HIEGHT + 2);
  return NULL;
}

void initialize(stgpk *pack) {

  static int old_str[LEN] = {};
  static int string[LEN];
  pack->old_string = old_str;
  pack->string = string;
  pack->offset = 0;

  for (int i = 0; i < HIEGHT; ++i) {
    printf("|");
    for (int j = 0; j < LENGTH; ++j) {
      printf(" ");
    }
    printf("|\n");
  }
}
/*
pack->string[i] = round(((cos((i - k / 3) * 2 * PI * FREQ / LENGTH) +
                          sin((i + k / 3) * 2 * PI * FREQ / 1.3 / LENGTH)) /
                         2 * exp(-0.000 * k) * HIEGHT / 2) -
                        (double)HIEGHT / 2);
                        */
