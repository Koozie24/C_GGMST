#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <stdbool.h>

typedef struct{
    int x_pos;
    int y_pos;
    double weight;
    int visited;
    int is_root_point;
}point; //defining a struct for points in a node
        // Tree structure using compass points


//----------------------------------------------------------------------------------------------
//
// I am leaving this untouched for now.
//
//-----------------------------------------------------------------------------------------------
enum Neighbors{UP, DOWN, LEFT, RIGHT, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN, TOTAL_NEIGHBORS};



typedef struct{
    size_t number_of_points;
    int row_number;
    int column_number;
    int is_root_node;
    double node_min_weight;
    point node_points[5];
    int visited;
    int is_leaf;
    int node_neighbors[TOTAL_NEIGHBORS];
    int parent_node_index;
}nodes; //defining a struct for nodes that contains an array of points, with an arbitrary maximum of 5 points per node


typedef struct  {
    nodes north;
    nodes nothEast;
    nodes east;
    nodes southEast;
    nodes south;
    nodes southWest;
    nodes west;
    nodes northWest;
    bool isRootNode;
} Tree;         // The tree structure based on compass points


//--------------------------------------------------------------
//
// This is a doubly linked list vhat we can used to keep track
// of nodes we visited. May replace with a hash table depending
// on what Dr. Raifey recommends.
// 
//--------------------------------------------------------------
typedef struct {
    nodes previous;
    nodes bext;
    float distance_from_prev;
} visitedNodes;

nodes graph_g[] = { //initializing a tree with 4 nodes that each contain varying number of points
    {.number_of_points = 3, .row_number = 0, .column_number = 0, .node_points = {{2,3}, {6,1}, {5,2}} }, //pass number of points then pass point x/y, weight should default to zero for a global initialized int
    {.number_of_points = 2, .row_number = 0, .column_number = 1, .node_points = {{1,1}, {8,5}} },
    {.number_of_points = 0, .row_number = 0, .column_number = 2, .node_points = {} }, //empty node
    {.number_of_points = 0, .row_number = 1, .column_number = 0, .node_points = {} }, 
    {.number_of_points = 5, .row_number = 1, .column_number = 1, .node_points = {{9,3}, {2,7}, {7,2}, {4,6}, {3,3}} },
    {.number_of_points = 2, .row_number = 1, .column_number = 2, .node_points = {{5,2}, {4,0}} },
    {.number_of_points = 2, .row_number = 2, .column_number = 0, .node_points = {{9,4}, {3,5}} },
    {.number_of_points = 3, .row_number = 2, .column_number = 1, .node_points = {{2,2}, {0,0}, {8,3}} },
    {.number_of_points = 1, .row_number = 2, .column_number = 2, .node_points = {{1,6}} }
};  

/*
graph_g current structure is like this:

2 x x x
1 x x x
0 x x x

Or where E is an empty node and N has a point

2 N N N
1 E N N
0 N N E
*/

const unsigned int NUMBER_OF_nodeS_IN_TREE = sizeof(graph_g) / sizeof(graph_g[0]) - 1; 
unsigned int MAX_ROW_NUMBER;
unsigned int MAX_COL_NUMBER;
unsigned int MAX_ROW_INDEX;
unsigned int MAX_COL_INDEX;
unsigned int TREE_HEIGHT;

/*Standardization for adjacency table:

up = current_row + 1 , currnet_col
down = current_row - 1, current col 
left = current_row, current_col - 1
right = current_row, current_col + 1
left_up = current_row + 1, current_col - 1
left_down = current_row - 1, current_col - 1
right_up = current_row + 1, current_col + 1
right_down = current_row - 1, current_col + 1

initialize neighbor values to -1 (invalid incidies) | otherwise initialize to indicies of neighbor

adjacent node indicies can be calculated by taking the value of the current node index (i) and adding/subtracting row and column height from it to verify if it is a valid index:
up = i + max row height: (index 0 (xy: 0,0) and you add max height of 3 gives us index 3 (xy: 2,0))
down = i - row max
left = i - 1
right = i + 1
L up =  i + (rowmax - 1)
L right =  i - (row max -1)
R up = i + (row max + 1)
R down = i - (row max + 1)
*/
void find_neighbors(){
    //loop through graph_g
    for(int i=0; i < (sizeof(graph_g) / sizeof(graph_g[0])); i++){
        
        int current_row = graph_g[i].row_number;
        int current_col = graph_g[i].column_number;

        graph_g[i].parent_node_index = -1; //go ahead and init to -1

        //up, down, left, right, left up, left down, right up, right down
        //set row and column numbers of current adjacent neighbors
        int adjacent_nodes[8][2] = {
            {current_row+1, current_col}, //up
            {current_row-1, current_col}, //down
            {current_row, current_col-1}, //L
            {current_row, current_col+1}, //r
            {current_row+1, current_col-1}, //left up
            {current_row-1, current_col-1}, //left down
            {current_row+1, current_col+1}, //right up
            {current_row-1, current_col+1}}; //right down
        //set indicies of current adjacen neighbors by adding or subtracting the length of the column to go up or down and 1 from index to go L/R
        int adjacent_node_indicies[8] = {
            i + MAX_COL_NUMBER, 
            i - MAX_COL_NUMBER, 
            i - 1, 
            i + 1, 
            (i - 1) + MAX_COL_NUMBER, 
            (i - 1) - MAX_COL_NUMBER, 
            (i + 1) + MAX_COL_NUMBER, 
            (i + 1) - MAX_COL_NUMBER};

        ///iterate over graph_g's total neighbors enum
        for(int j=0; j < TOTAL_NEIGHBORS; j++){
            int row = adjacent_nodes[j][0];
            int col = adjacent_nodes[j][1];
            int neighbor_index = adjacent_node_indicies[j];

            graph_g[i].node_neighbors[j] = -1; //set to -1 by default
            
            if((row < 0 || col < 0) || (row > MAX_ROW_INDEX || col > MAX_COL_INDEX)){ //check if row or col are out of bounds
                continue;
            }
            if(neighbor_index < 0 || neighbor_index > NUMBER_OF_nodeS_IN_TREE + 1){ //check if index is out of bounds
                continue;
            }

            if(graph_g[i].number_of_points == 0 || graph_g[neighbor_index].number_of_points == 0){ //case of empty node
                continue;
            }
      
            graph_g[i].node_neighbors[j] = neighbor_index; //set neighbor index in node neighbors
        }
    }

    for(int j = 0; j < sizeof(graph_g) / sizeof(graph_g[0]); j++){
        printf("graph_g[%d]: ", j);
        for(int i = 0; i < TOTAL_NEIGHBORS; i++){
            if(graph_g[j].node_neighbors[i] != -1){            
                printf("%d, ", graph_g[j].node_neighbors[i]);
            }
        }   //PRINTS EXPECTED RESULT FOR all j
        printf("\n");
    }
}

//helper function checks if input node has at least one point
int check_number_of_points_in_node(int selected_node){
    short number_points_in_node = graph_g[selected_node].number_of_points;

    if(number_points_in_node > 0){
        return 1;
    }
    else{
        return 0;
    }
}

//function to scan a valid root node index in graph_g
int pick_root_node_of_tree(){
    int input_node;
    short node_exists_in_array = 0;
    short node_not_empty = 0;

    printf("Pick a number greater than or equal to 0 and less than or equal to: %d to pick a node as the root: \n", NUMBER_OF_nodeS_IN_TREE - 1);
    scanf("%d", &input_node);
    while(1){
        node_exists_in_array, node_not_empty = 0; //reset flags to 0 before next check

        if(input_node >= 0 && input_node <= NUMBER_OF_nodeS_IN_TREE - 1){ //check input is in the range of nodes
            node_exists_in_array = 1;
        }
        short check_has_points = check_number_of_points_in_node(input_node); //check input is a non-empty node
        if(check_has_points){
            node_not_empty = 1;
        }

        if(check_has_points == 1 && node_exists_in_array == 1){ 
            graph_g[input_node].is_root_node = 1;
            break;
        }
        else{
            printf("Invalid input, pick a number >= 0 and <= %d, that is a non-empty node: \n", NUMBER_OF_nodeS_IN_TREE - 1);
            scanf("%d", &input_node);
        }
    
    }

    return input_node;
}

//function to scan a valid root point
int pick_root_point_of_node(int node_index){
    int input_point;
    int number_of_points = sizeof(graph_g[node_index].node_points) / sizeof(graph_g[node_index].node_points[0]);
    short point_exists_in_node = 0;
    
    printf("Pick a number greater than or equal to 0 and less than or equal to: %d to pick a point as the root: \n", number_of_points - 1);
    scanf("%d", &input_point);

    while(!point_exists_in_node){

        if(input_point >=0 && input_point <= number_of_points - 1){
            point_exists_in_node = 1;
            graph_g[node_index].node_points[input_point].is_root_point = 1; //set root in struct
        }
        
        else{
            printf("Invalid input, pick a number >= 0 and <= %d, to pick a point as the root point: \n", number_of_points - 1);
            scanf("%d", &input_point);
        }
    }
    
    return input_point;
}

//helper function get the row value of the root of the tree based on given input
int set_root_row(int root_cel_index){
    return graph_g[root_cel_index].row_number;
}

//helper function to get the height of the current tree by taking the row number of the last element in graph_g
int get_height_of_tree(){
    int last_element_graph_g = sizeof(graph_g) / sizeof(graph_g[0]) - 1;

    return graph_g[last_element_graph_g].row_number;
}

double euclidean_distance(int x1, int y1, int x2, int y2){
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

double compute_euclidean_distance_of_adjacent_nodes_for_given_point(point *current_node_point, int current_node_index){

    double current_minimum_for_point = INFINITY;

    nodes current_node = graph_g[current_node_index];
    int number_of_neighbors = sizeof(current_node.node_neighbors) / sizeof(current_node.node_neighbors[0]);

    for(int i=0; i < number_of_neighbors; i++){

        nodes current_neighbor_to_node = graph_g[current_node.node_neighbors[i]]; //get the node of current neighbor by accessing index in node neighbors

        for(int j=0; j < current_neighbor_to_node.number_of_points; j++){
            double distance_to_current_neighbor_point = euclidean_distance(current_node_point->x_pos, current_node_point->y_pos, current_neighbor_to_node.node_points[j].x_pos, current_neighbor_to_node.node_points[j].y_pos);
        }
    }
}

void print_tree(int node_index, int depth) {
  if (node_index == -1 || graph_g[node_index].visited) return;

  graph_g[node_index].visited = 1;

  for(int i = 0; i < graph_g[node_index].number_of_points; i++){
    printf("%d, %d\n", graph_g[node_index].node_points[i].x_pos, graph_g[node_index].node_points[i].y_pos);
  }

  // use indentition based on depth
  for (int i = 0; i < depth; i++) printf("  ");
  printf("└── node [%d,%d] (Index %d, points: %zu)\n",
	graph_g[node_index].row_number,
        graph_g[node_index].column_number,
        node_index,
	graph_g[node_index].number_of_points);
  // Recursively print each neighbor
  for (int i = 0; i < TOTAL_NEIGHBORS; i++) {
    int neighbor_index = graph_g[node_index].node_neighbors[i];
    if (neighbor_index != -1 && !graph_g[neighbor_index].visited) {
      print_tree(neighbor_index, depth+1);

      //loop over all points in node
      double minimum = INFINITY;
      int minimum_point_index = 0;
      for(int j=0; j < graph_g[node_index].number_of_points; j++){
        point current_node_point = graph_g[node_index].node_points[j];
        
        //for each point get min euclidean distance to all neighbor nodes' points
        double current_point_weight = compute_euclidean_distance_of_adjacent_nodes_for_given_point(&current_node_point, node_index);

        if(current_point_weight < minimum){
            minimum = current_point_weight;
            minimum_point_index = j;
        }



      }

      //set the weight of the minimum weight point
      graph_g[node_index].node_points[minimum_point_index].weight = minimum;
    }
  }
}

void reset_visited_flag() {
  for (int i=0; i <= NUMBER_OF_nodeS_IN_TREE; i++) {
    graph_g[i].visited = 0;
  }
}

int main(){
    //max row and col actualy val
    MAX_ROW_INDEX = graph_g[NUMBER_OF_nodeS_IN_TREE].row_number;
    MAX_COL_INDEX = graph_g[NUMBER_OF_nodeS_IN_TREE].column_number;
    //number of rows and cols
    MAX_COL_NUMBER  = MAX_COL_INDEX + 1;
    MAX_ROW_NUMBER = MAX_ROW_INDEX + 1;
    const int root_node_index = 1;
    // const int root_node_index = pick_root_node_of_tree();
    // const int root_point_index = pick_root_point_of_node();
    find_neighbors();
    reset_visited_flag();

    printf("Tree View\n");
    print_tree(root_node_index, 0);

    return 0;
}