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

adjacent cell indicies can be calculated by taking the value of the current cell index (i) and adding/subtracting row and column height from it to verify if it is a valid index:
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

        graph_g[i].parent_cell_index = -1; //go ahead and init to -1

        //up, down, left, right, left up, left down, right up, right down
        //set row and column numbers of current adjacent neighbors
        int adjacent_cells[8][2] = {
            {current_row+1, current_col}, //up
            {current_row-1, current_col}, //down
            {current_row, current_col-1}, //L
            {current_row, current_col+1}, //r
            {current_row+1, current_col-1}, //left up
            {current_row-1, current_col-1}, //left down
            {current_row+1, current_col+1}, //right up
            {current_row-1, current_col+1}}; //right down
        //set indicies of current adjacen neighbors by adding or subtracting the length of the column to go up or down and 1 from index to go L/R
        int adjacent_cell_indicies[8] = {
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
            int row = adjacent_cells[j][0];
            int col = adjacent_cells[j][1];
            int neighbor_index = adjacent_cell_indicies[j];

            graph_g[i].cell_neighbors[j] = -1; //set to -1 by default
            
            if((row < 0 || col < 0) || (row > MAX_ROW_INDEX || col > MAX_COL_INDEX)){ //check if row or col are out of bounds
                continue;
            }
            if(neighbor_index < 0 || neighbor_index > NUMBER_OF_CELLS_IN_TREE + 1){ //check if index is out of bounds
                continue;
            }

            if(graph_g[i].number_of_nodes == 0 || graph_g[neighbor_index].number_of_nodes == 0){ //case of empty cell
                continue;
            }
      
            graph_g[i].cell_neighbors[j] = neighbor_index; //set neighbor index in cell neighbors
        }
    }

    for(int j = 0; j < sizeof(graph_g) / sizeof(graph_g[0]); j++){
        printf("graph_g[%d]: ", j);
        for(int i = 0; i < TOTAL_NEIGHBORS; i++){
            if(graph_g[j].cell_neighbors[i] != -1){            
                printf("%d, ", graph_g[j].cell_neighbors[i]);
            }
        }   //PRINTS EXPECTED RESULT FOR all j
        printf("\n");
    }
}

//helper function checks if input cell has at least one node
int check_number_of_nodes_in_cell(int selected_cell){
    short number_nodes_in_cell = graph_g[selected_cell].number_of_nodes;

    if(number_nodes_in_cell > 0){
        return 1;
    }
    else{
        return 0;
    }
}

//function to scan a valid root cell index in graph_g
int pick_root_cell_of_tree(){
    int input_cell;
    short cell_exists_in_array = 0;
    short cell_not_empty = 0;

    printf("Pick a number greater than or equal to 0 and less than or equal to: %d to pick a cell as the root: \n", NUMBER_OF_CELLS_IN_TREE - 1);
    scanf("%d", &input_cell);
    while(1){
        cell_exists_in_array, cell_not_empty = 0; //reset flags to 0 before next check

        if(input_cell >= 0 && input_cell <= NUMBER_OF_CELLS_IN_TREE - 1){ //check input is in the range of cells
            cell_exists_in_array = 1;
        }
        short check_has_nodes = check_number_of_nodes_in_cell(input_cell); //check input is a non-empty cell
        if(check_has_nodes){
            cell_not_empty = 1;
        }

        if(check_has_nodes == 1 && cell_exists_in_array == 1){ 
            graph_g[input_cell].is_root_cell = 1;
            break;
        }
        else{
            printf("Invalid input, pick a number >= 0 and <= %d, that is a non-empty cell: \n", NUMBER_OF_CELLS_IN_TREE - 1);
            scanf("%d", &input_cell);
        }
    
    }

    return input_cell;
}

//function to scan a valid root node
int pick_root_node_of_cell(int cell_index){
    int input_node;
    int number_of_nodes = sizeof(graph_g[cell_index].cell_nodes) / sizeof(graph_g[cell_index].cell_nodes[0]);
    short node_exists_in_cell = 0;
    
    printf("Pick a number greater than or equal to 0 and less than or equal to: %d to pick a node as the root: \n", number_of_nodes - 1);
    scanf("%d", &input_node);

    while(!node_exists_in_cell){

        if(input_node >=0 && input_node <= number_of_nodes - 1){
            node_exists_in_cell = 1;
            graph_g[cell_index].cell_nodes[input_node].is_root_node = 1; //set root in struct
        }
        
        else{
            printf("Invalid input, pick a number >= 0 and <= %d, to pick a node as the root node: \n", number_of_nodes - 1);
            scanf("%d", &input_node);
        }
    }
    
    return input_node;
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

double compute_euclidean_distance_of_adjacent_cells_for_given_node(Node *current_cell_node, int current_cell){

    double current_minimum_for_node = INFINITY;
}

void print_tree(int cell_index, int depth) {
  if (cell_index == -1 || graph_g[cell_index].visited) return;

  graph_g[cell_index].visited = 1;

  for(int i = 0; i < graph_g[cell_index].number_of_nodes; i++){
    printf("%d, %d\n", graph_g[cell_index].cell_nodes[i].x_pos, graph_g[cell_index].cell_nodes[i].y_pos);
  }

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

      //loop over all nodes in cell
      double minimum = INFINITY;
      int minimum_node_index = 0;
      for(int j=0; j < graph_g[cell_index].number_of_nodes; j++){
        Node current_cell_node = graph_g[cell_index].cell_nodes[j];
        
        //for each node get min euclidean distance to all neighbor cells' nodes
        double current_node_weight = compute_euclidean_distance_of_adjacent_cells_for_given_node(&current_cell_node, cell_index);

        if(current_node_weight < minimum){
            minimum = current_node_weight;
            minimum_node_index = j;
        }



      }

      //set the weight of the minimum weight node
      graph_g[cell_index].cell_nodes[minimum_node_index].weight = minimum;
    }
  }
}

void reset_visited_flag() {
  for (int i=0; i <= NUMBER_OF_CELLS_IN_TREE; i++) {
    graph_g[i].visited = 0;
  }
}

int main(){
    //max row and col actualy val
    MAX_ROW_INDEX = graph_g[NUMBER_OF_CELLS_IN_TREE].row_number;
    MAX_COL_INDEX = graph_g[NUMBER_OF_CELLS_IN_TREE].column_number;
    //number of rows and cols
    MAX_COL_NUMBER  = MAX_COL_INDEX + 1;
    MAX_ROW_NUMBER = MAX_ROW_INDEX + 1;
    const int root_cell_index = 1;
    // const int root_cell_index = pick_root_cell_of_tree();
    // const int root_node_index = pick_root_node_of_cell();
    find_neighbors();
    reset_visited_flag();

    printf("Tree View\n");
    print_tree(root_cell_index, 0);

    return 0;
}