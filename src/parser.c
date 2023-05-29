#include "parser.h"
// int main() { main88(); }
int main88() {
  //    char *file = {"RailwayTrack.obj"};
  char *file = {
      "/Users/pickling/0projects/3D/s21_3dViewer/src/models/Shrek.obj"};
  //    char *file = {"Shrek.obj"};

  ExtremeValues extreme_values;
  Point *all_points;
  int top_pointers = 0;
  int count_surfaces = 0;
  all_points = point_reading(file, &top_pointers, &extreme_values);
  figure_centering(top_pointers, &extreme_values, all_points);

  Surface *all_surfaces;
  all_surfaces = (Surface *)malloc(SIZE * sizeof(Surface));
  all_surfaces = surface_formation(file, top_pointers, &count_surfaces,
                                   all_surfaces, all_points);

  /* вывести крайние координаты точек */
  printf("x_max: %f x_min: %f\n", extreme_values.x_max, extreme_values.x_min);
  printf("y_max: %f y_min: %f\n", extreme_values.y_max, extreme_values.y_min);
  printf("z_max: %f z_min:%f\n", extreme_values.z_max, extreme_values.z_min);
  /* вывести все координаты точек */
  printf("|==========================|\n");
  for (int i = 0; i <= top_pointers; ++i)
    printf("v%d: %1.3f %1.3f %1.3f\n", i + 1, all_points[i].x, all_points[i].y,
           all_points[i].z);
  printf("|==========================|\n");
  /* вывести все поверхности по точкам */
  printf("\n|====================|\n");
  for (int i = 0; i < count_surfaces; ++i) {
    printf("\t|f%d:|\n", i + 1);
    for (int j = 0; j < all_surfaces[i].amount_of_points; ++j) {
      printf("v%ld: %1.3f %1.3f %1.3f\n",
             all_surfaces[i].one_point[j] - all_points + 1,
             all_surfaces[i].one_point[j]->x, all_surfaces[i].one_point[j]->y,
             all_surfaces[i].one_point[j]->z);
    }
    printf("|====================|\n");
  }

  for (int i = 0; i < count_surfaces; ++i) {
    free(all_surfaces[i].one_point);
  }
  free(all_surfaces);
  free(all_points);
  return 0;
}

void figure_centering(int top_pointers, ExtremeValues *extreme_values,
                      Point *all_points) {
  double center_x, center_y, center_z;
  double dmax, coefficient;
  center_x = extreme_values->x_min +
             ((extreme_values->x_max - extreme_values->x_min) / 2);
  center_y = extreme_values->y_min +
             ((extreme_values->y_max - extreme_values->y_min) / 2);
  center_z = extreme_values->z_min +
             ((extreme_values->z_max - extreme_values->z_min) / 2);
  dmax = delta_max(extreme_values->x_max - extreme_values->x_min,
                   extreme_values->y_max - extreme_values->y_min,
                   extreme_values->z_max - extreme_values->z_min);
  coefficient = (1 - (1 * (-1))) / dmax;
  for (int i = 0; i <= top_pointers; ++i) {
    all_points[i].x -= center_x;
    all_points[i].x *= coefficient;
    all_points[i].y -= center_y;
    all_points[i].y *= coefficient;
    all_points[i].z -= center_z;
    all_points[i].z *= coefficient;
  }
}

double delta_max(double x, double y, double z) {
  double back = x;
  if (x < y) back = y;
  if (y < z) back = z;
  return back;
}

Point *point_reading(char *file, int *top_pointers,
                     ExtremeValues *extreme_values) {
  FILE *f = NULL;
  int size_all_points = SIZE;
  Point *all_points = (Point *)malloc(size_all_points * sizeof(Point));
  if (((f = fopen(file, "r")) != NULL) && (all_points != NULL)) {
    all_points = point_processing(f, top_pointers, &size_all_points,
                                  extreme_values, all_points);
  }
  fclose(f);
  return all_points;
}

Point *point_processing(FILE *f, int *top_pointers, int *size_all_points,
                        ExtremeValues *extreme_values, Point *all_points) {
  char c = 0, past_char = 0;
  char buffer_str[1000];
  while ((c = fgetc(f)) != EOF) {
    if ((c == ' ') && (past_char == 'v')) {
      if ((*top_pointers) == (*size_all_points)) {
        (*size_all_points) += SIZE;
        all_points =
            (Point *)realloc(all_points, (*size_all_points) * sizeof(Point));
      }
      fgets(buffer_str, 99, f);
      int count_coord = 0;
      int flag_result = 0;
      int flag_dot = 0;
      int flag_mines = 0;
      double result = 0;
      for (int i = 0; count_coord < 3; ++i) {
        if ((buffer_str[i] != ' ') && (buffer_str[i] != '\n') &&
            (buffer_str[i] != '\0') && (buffer_str[i] != 13)) {
          to_double(&result, buffer_str[i], &flag_dot, &flag_mines);
          flag_result = 1;
        } else if (flag_result != 0) {
          if (flag_mines == 1) {
            result *= -1;
          }
          finding_extreme_values(result, count_coord, *top_pointers,
                                 extreme_values);
          flag_mines = flag_result = flag_dot = 0;
          coordinate_recording_counter(&count_coord, *top_pointers, &result,
                                       all_points);
          if (count_coord == 3) {
            (*top_pointers)++;
          }
        }
      }
    }
    past_char = c;
  }
  (*top_pointers)--;
  return all_points;
}

void finding_extreme_values(double result, int count_coord, int top_pointers,
                            ExtremeValues *extreme_values) {
  if (top_pointers == 0) {
    switch (count_coord) {
      case 0:
        extreme_values->x_max = result;
        extreme_values->x_min = result;
        break;
      case 1:
        extreme_values->y_max = result;
        extreme_values->y_min = result;
        break;
      case 2:
        extreme_values->z_max = result;
        extreme_values->z_min = result;
        break;
    }
  } else {
    switch (count_coord) {
      case 0:
        if (result > extreme_values->x_max) extreme_values->x_max = result;
        if (result < extreme_values->x_min) extreme_values->x_min = result;
        break;
      case 1:
        if (result > extreme_values->y_max) extreme_values->y_max = result;
        if (result < extreme_values->y_min) extreme_values->y_min = result;
        break;
      case 2:
        if (result > extreme_values->z_max) extreme_values->z_max = result;
        if (result < extreme_values->z_min) extreme_values->z_min = result;
        break;
    }
  }
}

void to_double(double *result, char chr, int *flag_dot, int *flag_mines) {
  if ((chr != '.') && (chr != '-')) {
    if ((*flag_dot) == 0) {
      (*result) = (*result) * 10 + (chr - 48);
    } else {
      (*result) += (chr - 48) * pow(10, -(*flag_dot));
      (*flag_dot)++;
    }
  } else if (chr == '.') {
    (*flag_dot) = 1;
  } else {
    (*flag_mines) = 1;
  }
}

void coordinate_recording_counter(int *count_coord, int top_pointers,
                                  double *result, Point *all_points) {
  switch (*count_coord) {
    case 0:
      all_points[top_pointers].x = *result;
      break;
    case 1:
      all_points[top_pointers].y = *result;
      break;
    case 2:
      all_points[top_pointers].z = *result;
      break;
  }
  (*count_coord)++;
  *result = 0;
}

Surface *surface_formation(char *file, int top_pointers, int *count_surfaces,
                           Surface *all_surfaces, Point *all_points) {
  FILE *f;
  int size_all_surfaces = SIZE;
  if ((f = fopen(file, "r")) != NULL) {
    char c;
    char past_char = 0;
    char buffer_str[1000];
    while ((c = fgetc(f)) != EOF) {
      if ((c == ' ') && (past_char == 'f')) {
        if ((*count_surfaces) == size_all_surfaces) {
          size_all_surfaces += SIZE;
          all_surfaces = (Surface *)realloc(
              all_surfaces, size_all_surfaces * sizeof(Surface));
        }
        fgets(buffer_str, 999, f);
        points_in_space(buffer_str, (*count_surfaces), top_pointers,
                        all_surfaces, all_points);
        (*count_surfaces)++;
      }
      past_char = c;
    }
  }
  return all_surfaces;
}

void points_in_space(char *buffer_str, int count_surfaces, int top_pointers,
                     Surface *all_surfaces, Point *all_points) {
  int res = 0;
  int count_point_in_surface = 0;
  int number_of_points = 0;
  int flag_mines = 0;
  int flag_slash = 0;
  for (int i = 0; buffer_str[i] != 0; ++i) {
    if ((buffer_str[i] == ' ') && (buffer_str[i + 1] != ' '))
      number_of_points++;
  }
  all_surfaces[count_surfaces].one_point =
      (Point **)malloc(number_of_points * sizeof(Point *));
  for (int i = 0; count_point_in_surface < number_of_points; ++i) {
    if ((buffer_str[i] != ' ') && (buffer_str[i] != '\n') &&
        (buffer_str[i] != '\0') && (buffer_str[i] != 13)) {
      if (buffer_str[i] == '/') {
        flag_slash = 1;
      }
      if (flag_slash == 0) {
        point_address_calculation(buffer_str[i], &res, &flag_mines);
      }
    } else if (res != 0) {
      flag_slash = 0;
      point_address_modification(&res, top_pointers);
      all_surfaces[count_surfaces].one_point[count_point_in_surface] =
          all_points + res;
      count_point_in_surface++;
      res = 0;
    }
  }
  all_surfaces[count_surfaces].amount_of_points = number_of_points;
}

void point_address_modification(int *res, int top_pointers) {
  if ((*res) < 0) {
    (*res) += top_pointers + 1;
  } else {
    (*res) -= 1;
  }
}

void point_address_calculation(char chr, int *res, int *flag_mines) {
  if (chr != '-') {
    (*res) = (*res) * 10 + (chr - 48);
    if (*flag_mines) {
      (*res) *= -1;
      (*flag_mines) = 0;
    }
  } else {
    (*flag_mines) = 1;
  }
}
