#include "stdlib.h"

typedef struct{
    int x_pos;
    int y_pos;
    int weight;
}Node; //defining a struct for nodes in a cell

typedef struct{
    size_t number_of_nodes;
    int row_number;
    int column_number;
    Node cell_nodes[5];
}Cells; //defining a struct for cells that contains an array of nodes, with an arbitrary maximum of 5 nodes per cell

Cells tree_t[] = {
    {.number_of_nodes = 3, .row_number = 0, .column_number = 0, .cell_nodes = {{2,3}, {6,1}, {5,2}} }, //pass number of nodes then pass node x/y, weight should default to zero for a global initialized int
    {.number_of_nodes = 2, .row_number = 0, .column_number = 1, .cell_nodes = {{1,1}, {8,5}} },
    {.number_of_nodes = 0, .row_number = 0, .column_number = 2, .cell_nodes = {} }, //empty cell
    {.number_of_nodes = 0, .row_number = 1, .column_number = 0, .cell_nodes = {} }, 
    {.number_of_nodes = 5, .row_number = 1, .column_number = 1, .cell_nodes = {{9,3}, {2,7}, {7,2}, {4,6}, {3,3}} },
    {.number_of_nodes = 2, .row_number = 1, .column_number = 2,.cell_nodes = {{5,2}, {4,0}} }
}; 
//initializing a tree with 4 cells that each contain varying number of nodes

//function to scan a valid root cell index in tree_t
int pick_root_cell_of_tree(){
    int input_cell;
    unsigned int number_of_cells_in_tree = sizeof(tree_t);
    short valid_cell_input = 0;

    printf("Pick a number less than or equal to: %d and greater than or equal to 0 to pick a cell as the root: \n", number_of_cells_in_tree);
    scanf("%d", &input_cell);
    while(!valid_cell_input){
        if(input_cell >= 0 && input_cell <= number_of_cells_in_tree){
            valid_cell_input = 1;
        }
        else{
            printf("Invalid input, pick a number >= 0 and <= %d: \n", number_of_cells_in_tree);
            scanf("%d", &input_cell);
        }
    }

    return input_cell;

}

int pick_root_node_of_cell(int cell_index){
    int input_node;
    int number_of_nodes = sizeof(tree_t[cell_index].cell_nodes);
    
    
    return input_node;
}


int main(){
    int cell_root = pick_root_cell_of_tree();
    int root_node = pick_root_node_of_cell(cell_root);

    return 0;
}