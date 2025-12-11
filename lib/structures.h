#include <stdlib.h>
#ifndef STRUCTURES_H
#define STRUCTURES_H


typedef struct{
    double x_pos;
    double y_pos;
    double weight;
    int visited;
    int is_root_point;
}Point; //defining a struct for points in a node

enum Neighbors{UP, DOWN, LEFT, RIGHT, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN, TOTAL_NEIGHBORS};

typedef struct{
    int child_index; //index of child on edge or -1 if none
    double weight; //weight of the edge
}Edges;

typedef struct{
    size_t number_of_points;
    int row_number;
    int column_number;
    int is_root_node;
    double node_min_weight;
    Point node_points[5];
    int visited;
    int is_leaf;
    int node_neighbors[TOTAL_NEIGHBORS];
    int parent_node_index;
    Edges set_of_edges[8];
}Nodes; //defining a struct for Nodes that contains an array of points, with an arbitrary maximum of 5 points per node

#endif //STRUCTURES_H