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
/*
tydef struct{
    int up, down, left, right, left_up, left_down, right_up, right_down;
} Neighbors; //indicies of cell neighbors in tree_t to current cell */

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

Cells tree_t[] = { //initializing a tree with 4 cells that each contain varying number of nodes
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
tree_t current structure is like this:

2 x x x
1 x x x
0 x x x

Or where E is an empty cell and N has a node

2 N N N
1 E N N
0 N N E
*/

const unsigned int NUMBER_OF_CELLS_IN_TREE = sizeof(tree_t) / sizeof(tree_t[0]); 
unsigned int MAX_ROW_NUMBER;
unsigned int MAX_COL_NUMBER;

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
void initialize_neighbors(){
    //loop through tree_t
    for(int i=0; i < sizeof(tree_t) / sizeof(tree_t[0]); i++){
        //up, down, left, right, left up, left down, right up, right down
        //set row and column numbers of current adjacent neighbors
        int adjacent_cells[8][2] = {{tree_t[i].row_number+1, tree_t[i].column_number}, {tree_t[i].row_number-1, tree_t[i].column_number}, {tree_t[i].row_number, tree_t[i].column_number-1}, {tree_t[i].row_number, tree_t[i].column_number+1}, {tree_t[i].row_number+1, tree_t[i].column_number-1}, {tree_t[i].row_number-1, tree_t[i].column_number-1}, {tree_t[i].row_number+1, tree_t[i].column_number+1}, {tree_t[i].row_number-1, tree_t[i].column_number+1}};
        //set indicies of current adjacen neighbors
        int adjacent_cell_indicies[8] = {i + MAX_ROW_NUMBER, i - MAX_ROW_NUMBER, i - 1, i + 1, i + (MAX_ROW_NUMBER - 1), i - (MAX_ROW_NUMBER - 1), i + (MAX_ROW_NUMBER + 1), i - (MAX_ROW_NUMBER + 1)};
        ///iterate over tree_t's total neighbors enum
        for(int j=0; j < TOTAL_NEIGHBORS; j++){
            tree_t[i].cell_neighbors[j] = -1; //set to -1 by default

            if(tree_t[adjacent_cell_indicies[j]].number_of_nodes == 0){ //case of empty cell
                continue;
            }
            //checking if rows and cols are no more than max and that indicies not less than 0
            if((adjacent_cells[j][0] <= MAX_ROW_NUMBER && adjacent_cells[j][1] <= MAX_COL_NUMBER) && adjacent_cell_indicies[j] >= 0) {
                tree_t[i].cell_neighbors[j] = adjacent_cell_indicies[j];
            }
        }
           
    }
    for(int i = 0; i < TOTAL_NEIGHBORS; i++){
        printf("%d, ", tree_t[0].cell_neighbors[i]);
    }   //PRINTS EXPECTED RESULT
}

void BFS(){

}

int main(){
    MAX_ROW_NUMBER = tree_t[NUMBER_OF_CELLS_IN_TREE - 1].row_number + 1;
    MAX_COL_NUMBER = tree_t[NUMBER_OF_CELLS_IN_TREE - 1].column_number + 1;
    const int root_cell_index = 1;
    initialize_neighbors();




    return 0;
}