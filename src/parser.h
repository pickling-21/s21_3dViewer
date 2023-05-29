#ifndef PARSER_H
#define PARSER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 50

typedef struct {
  double x;
  double y;
  double z;
} Point;

typedef struct {
  Point **one_point;
  int amount_of_points;
} Surface;
typedef struct {
  double x_max, x_min;
  double y_max, y_min;
  double z_max, z_min;
} ExtremeValues;

int main88();
void destoy_surface(Surface *all_surfaces, int count_surfaces);

Point *point_reading(char *file, int *top_pointers,
                     ExtremeValues *extreme_values);
void coordinate_recording_counter(int *count_coord, int top_pointers,
                                  double *result, Point *all_points);
void points_in_space(char *buffer_str, int count_surfaces, int top_pointers,
                     Surface *all_surfaces, Point *all_points);
Surface *surface_formation(char *file, int top_pointers, int *count_surfaces,
                           Surface *all_surfaces, Point *all_points);
void point_address_calculation(char chr, int *res, int *flag_mines);
void point_address_modification(int *res, int top_pointers);
Point *point_processing(FILE *f, int *top_pointers, int *size_all_points,
                        ExtremeValues *extreme_values, Point *all_points);
void to_double(double *result, char chr, int *flag_dot, int *flag_mines);
void finding_extreme_values(double result, int count_coord, int top_pointers,
                            ExtremeValues *extreme_values);
void figure_centering(int top_pointers, ExtremeValues *extreme_values,
                      Point *all_points);
double delta_max(double a, double b, double c);

#endif  // PARSER_H