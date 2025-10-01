#include "stdio.h"
#include "stdlib.h"
#include "math.h"

typedef struct{
    int x_pos;
    int y_pos;
    double weight;
    int visited;
    int is_root_node;
}Node; //defining a struct for nodes in a cell
        // Tree structure using compass points

enum Neighbors{UP, DOWN, LEFT, RIGHT, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN, TOTAL_NEIGHBORS};

typedef struct{
    size_t number_of_nodes;
    int row_number;
    int column_number;
    int is_root_cell;
    double cell_min_weight;
    Node cell_nodes[5];
    int visited;
    int is_leaf;
    int cell_neighbors[TOTAL_NEIGHBORS];
    int parent_cell_index;
}Cells; //defining a struct for cells that contains an array of nodes, with an arbitrary maximum of 5 nodes per cell

Cells graph_g[] = { //initializing a tree with 4 cells that each contain varying number of nodes
    {.number_of_nodes = 3, .row_number = 0, .column_number = 0, .cell_nodes = {{2,3}, {6,1}, {5,2}} }, //pass number of nodes then pass node x/y, weight should default to zero for a global initialized int
    {.number_of_nodes = 2, .row_number = 0, .column_number = 1, .cell_nodes = {{1,1}, {8,5}} },
    {.number_of_nodes = 0, .row_number = 0, .column_number = 2, .cell_nodes = {} }, //empty cell
    {.number_of_nodes = 0, .row_number = 1, .column_number = 0, .cell_nodes = {} }, 
    {.number_of_nodes = 5, .row_number = 1, .column_number = 1, .cell_nodes = {{9,3}, {2,7}, {7,2}, {4,6}, {3,3}} },
    {.number_of_nodes = 2, .row_number = 1, .column_number = 2, .cell_nodes = {{5,2}, {4,0}} },
    {.number_of_nodes = 2, .row_number = 2, .column_number = 0, .cell_nodes = {{9,4}, {3,5}} },
    {.number_of_nodes = 3, .row_number = 2, .column_number = 1, .cell_nodes = {{2,2}, {0,0}, {8,3}} },
    {.number_of_nodes = 1, .row_number = 2, .column_number = 2, .cell_nodes = {{1,6}} }
};  

/*
graph_g current structure is like this:

2 x x x
1 x x x
0 x x x

Or where E is an empty cell and N has a node

2 N N N
1 E N N
0 N N E
*/

const unsigned int NUMBER_OF_CELLS_IN_TREE = sizeof(graph_g) / sizeof(graph_g[0]) - 1; 
unsigned int MAX_ROW_NUMBER;
unsigned int MAX_COL_NUMBER;
unsigned int MAX_ROW_INDEX;
unsigned int MAX_COL_INDEX;

//function takes pointers to a cell and a node along with the index to the cell in graph_g. finds the cells adjacent to the given cell and iterates over nodes each cell to find the minimum euclidean distance to our node. returns the minimum weight.
double compute_euclidean_distance_of_adjacent_cells_for_given_node(Cells *start_cell, Node *node, int current_cell_index){
    Node current_node = *node; 
    Cells current_cell = *start_cell; //deref pointers

    //if current cell is 1,1 we look at all nodes in cells 1,2 | 1,0 | 2,1 | 0,1 or x,y+1 | x,y-1 | x+1,y | x-1,y
    int adjacent_cells[4][2] = {{current_cell.column_number, current_cell.row_number + 1}, {current_cell.column_number, current_cell.row_number - 1}, {current_cell.column_number+1, current_cell.row_number}, {current_cell.column_number-1, current_cell.row_number}};
    int adjacent_cell_indicies[4] = {current_cell_index + MAX_ROW_NUMBER, current_cell_index - MAX_ROW_NUMBER, current_cell_index + 1, current_cell_index - 1};
    double current_minimum_value = INFINITY;

    for(int i = 0; i < 4; i++){
        
        if((adjacent_cells[i][0] <= MAX_COL_NUMBER && adjacent_cells[i][1] <= MAX_ROW_NUMBER) && graph_g[adjacent_cell_indicies[i]].cell_nodes > 0){ //check we are not accessing cell out of bounds or an empty cell
            for(int j = 0; j < sizeof(graph_g[adjacent_cell_indicies[i]].cell_nodes)/sizeof(graph_g[adjacent_cell_indicies[i]].cell_nodes[0]); j++){ //iterate over cell nodes
                double current_euclidean_distance = euclidean_distance(current_node.x_pos, current_node.y_pos, graph_g[adjacent_cell_indicies[i]].cell_nodes[j].x_pos, graph_g[adjacent_cell_indicies[i]].cell_nodes[j].y_pos);
                
                //check for a stored weight in the cell - if it is add to weight of the node ****unsure if this is a correct implementation of the pseudocode*****
                if(graph_g[adjacent_cell_indicies[i]].cell_min_weight){
                    current_euclidean_distance += graph_g[adjacent_cell_indicies[i]].cell_min_weight;
                }

                if(current_euclidean_distance < current_minimum_value){ //check if new min found
                    current_minimum_value = current_euclidean_distance;
                }
            }
        }
    }
    return current_minimum_value;
}


void print_tree(int cell_index, int depth) {
  if (cell_index == -1 || graph_g[cell_index].visited) return;

  graph_g[cell_index].visited = 1;
  // for (i=0; i< graph_g[cell_index].number_of_nodes; i++)
///    printf("%d , %d ", graph_g[cell_index].cell_nodes[i][0],graph_g[cell_index].cell_nodes[i][1]);
  // use indentition based on depth
  for (int i = 0; i < depth; i++) printf("  ");
  printf("└── Cell [%d,%d] (Index %d, Nodes: %zu)\n",
	graph_g[cell_index].row_number,
        graph_g[cell_index].column_number,
        cell_index,
	graph_g[cell_index].number_of_nodes);
  // Recursively print each neighbor
  for (int i = 0; i < TOTAL_NEIGHBORS; i++) {
    int neighbor_index = graph_g[cell_index].cell_neighbors[i];
    if (neighbor_index != -1 && !graph_g[neighbor_index].visited) {
      print_tree(neighbor_index, depth+1);
	  // how do I print all the points inside this cell? 
		// for each point "i" inside compute graph_g[cell_index] we want to compute weight which is graph_g[cell_index].cell_nodes[i].weight
		// for (i=0; i< graph_g[cell_index].number_of_nodes; i++)
		//  {
		     // graph_g[cell_index].cell_nodes[i].weight
		//  }
    }
  }
}