#include "stdio.h"
#include "stdlib.h"
#include "math.h"

/*
Algorithm 2: Optimal GGMST solution for a given spanning tree of cells
Data: A spanning tree T of non-empty cells
Result: An optimal weight of the GGMST tree with the same structure as T
1 Choose an arbitrary cell Cr as the root of T ;
2 for each leaf Ci of T do
3   for each p ∈ Xi do
4       W (p) = 0;
5    end
6 end
7 CurrentLevel = height of T ;
8 while CurrentLevel ≥ root level do
9   for each node Ci of CurrentLevel do
10      Let Ci1 , . . . , Cik be children of Ci in T ;
11      for each p ∈ Xi do
12          W (p) = ∑kj=1 minq∈Xij {W (q) + w(ep,q )};
13      end
14  end
15  CurrentLevel = CurrentLevel − 1;
16 end
17 return minp∈Xr W (p)
*/

typedef struct{
    int x_pos;
    int y_pos;
    double weight;
    int visited;
    int is_root_node;
}Node; //defining a struct for nodes in a cell

typedef struct {
    Node north;
    Node northeast;
    Node east;
    Node southeast;
    Node south;
    Node southwest;
    Node west;
    Node northwest;

} Tree;         // Tree structure using compass points
typedef struct{
    size_t number_of_nodes;
    int row_number;
    int column_number;
    int is_root_cell;
    double cell_min_weight;
    Node cell_nodes[5];
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
    {.number_of_nodes = 2, .row_number = 2, .column_number = 2, .cell_nodes = {{1,6}} }
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

//helper function checks if input cell has at least one node
int check_number_of_nodes_in_cell(int selected_cell){
    short number_nodes_in_cell = tree_t[selected_cell].number_of_nodes;

    if(number_nodes_in_cell > 0){
        return 1;
    }
    else{
        return 0;
    }
}

//function to scan a valid root cell index in tree_t
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
            tree_t[input_cell].is_root_cell = 1;
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
    int number_of_nodes = sizeof(tree_t[cell_index].cell_nodes) / sizeof(tree_t[cell_index].cell_nodes[0]);
    short node_exists_in_cell = 0;
    
    printf("Pick a number greater than or equal to 0 and less than or equal to: %d to pick a node as the root: \n", number_of_nodes - 1);
    scanf("%d", &input_node);

    while(!node_exists_in_cell){

        if(input_node >=0 && input_node <= number_of_nodes - 1){
            node_exists_in_cell = 1;
            tree_t[cell_index].cell_nodes[input_node].is_root_node = 1; //set root in struct
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
    return tree_t[root_cel_index].row_number;
}

//helper function to get the height of the current tree by taking the row number of the last element in tree_t
int get_height_of_tree(){
    int last_element_tree_t = sizeof(tree_t) / sizeof(tree_t[0]) - 1;

    return tree_t[last_element_tree_t].row_number;
    
}

double euclidean_distance(int x1, int y1, int x2, int y2){
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

//function takes pointers to a cell and a node along with the index to the cell in tree_t. finds the cells adjacent to the given cell and iterates over nodes each cell to find the minimum euclidean distance to our node. returns the minimum weight.
double compute_euclidean_distance_of_adjacent_cells_for_given_node(Cells *start_cell, Node *node, int current_cell_index){
    Node current_node = *node; 
    Cells current_cell = *start_cell; //deref pointers

    //if current cell is 1,1 we look at all nodes in cells 1,2 | 1,0 | 2,1 | 0,1 or x,y+1 | x,y-1 | x+1,y | x-1,y
    int adjacent_cells[4][2] = {{current_cell.column_number, current_cell.row_number + 1}, {current_cell.column_number, current_cell.row_number - 1}, {current_cell.column_number+1, current_cell.row_number}, {current_cell.column_number-1, current_cell.row_number}};
    int adjacent_cell_indicies[4] = {current_cell_index + MAX_ROW_NUMBER, current_cell_index - MAX_ROW_NUMBER, current_cell_index + 1, current_cell_index - 1};
    double current_minimum_value = INFINITY;

    for(int i = 0; i < 4; i++){
        
        if((adjacent_cells[i][0] <= MAX_COL_NUMBER && adjacent_cells[i][1] <= MAX_ROW_NUMBER) && tree_t[adjacent_cell_indicies[i]].cell_nodes > 0){ //check we are not accessing cell out of bounds or an empty cell
            for(int j = 0; j < sizeof(tree_t[adjacent_cell_indicies[i]].cell_nodes)/sizeof(tree_t[adjacent_cell_indicies[i]].cell_nodes[0]); j++){ //iterate over cell nodes
                double current_euclidean_distance = euclidean_distance(current_node.x_pos, current_node.y_pos, tree_t[adjacent_cell_indicies[i]].cell_nodes[0].x_pos, tree_t[adjacent_cell_indicies[i]].cell_nodes[0].y_pos);
                
                //check for a stored weight in the cell - if it is add to weight of the node ****unsure if this is a correct implementation of the pseudocode*****
                if(tree_t[adjacent_cell_indicies[i]].cell_min_weight){
                    current_euclidean_distance += tree_t[adjacent_cell_indicies[i]].cell_min_weight;
                }

                if(current_euclidean_distance < current_minimum_value){ //check if new min found
                    current_minimum_value = current_euclidean_distance;
                }
            }
        }
    }
    return current_minimum_value;
}

//all weights init to 0 at compile time - no need to intialize again
void optimal_ggmst_algorithm_two(const int HEIGHT, const int ROOT_ROW){
    int current_tree_level = HEIGHT;

    while(current_tree_level >= ROOT_ROW){
        for(int i = 0; i < NUMBER_OF_CELLS_IN_TREE; i++){ //iterate over cells 
            if(tree_t[i].row_number == current_tree_level){
                double cell_minimium = INFINITY;
                for(int j = 0; j < (sizeof(tree_t[i].cell_nodes)/sizeof(tree_t[i].cell_nodes[0])); j++){ //iterate over nodes in cell
                    if(!tree_t[i].cell_nodes[j].visited){
                        //pass current node to helper function to compute minimum euclidean distance to all nodes of adjacent cells.
                        double current_node_minimum_euclidean_distance = compute_euclidean_distance_of_adjacent_cells_for_given_node(&tree_t[i], &tree_t[i].cell_nodes[j], i);
                        //mark node visited
                        tree_t[i].cell_nodes[j].visited = 1;
                        tree_t[i].cell_nodes[j].weight = current_node_minimum_euclidean_distance;

                        //compare min weight returned with current minimum for cell.
                        if(current_node_minimum_euclidean_distance < cell_minimium){
                            cell_minimium = current_node_minimum_euclidean_distance;
                        }
                    }
                }
                tree_t[i].cell_min_weight = cell_minimium;

            }
        }

        current_tree_level--;
    }

}

int main(){
    int cell_root = pick_root_cell_of_tree();
    int root_node = pick_root_node_of_cell(cell_root);
    const int ROOT_ROW_OF_TREE = set_root_row(cell_root);
    const int TREE_HEIGHT = get_height_of_tree();
    MAX_ROW_NUMBER = tree_t[NUMBER_OF_CELLS_IN_TREE].row_number;
    MAX_COL_NUMBER = tree_t[NUMBER_OF_CELLS_IN_TREE].column_number;
    printf("The cell root is index: %d. and the root node is at index: %d\n", cell_root, root_node);

    optimal_ggmst_algorithm_two(TREE_HEIGHT, ROOT_ROW_OF_TREE);

    return 0;
}
