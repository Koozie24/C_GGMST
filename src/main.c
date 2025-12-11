#include "stdio.h"
#include "math.h"
#include "float.h"
#include "../lib/structures.h"

Nodes graph_g[] = { //initializing a tree with 9 nodes that each contain varying number of points
    {.number_of_points = 3, .row_number = 1, .column_number = 1, .node_points = {{0.52, 0.61}, {0.34, 0.47}, {0.13, 0.92}} }, //pass number of points then pass point x/y, weight should default to zero for a global initialized int
    {.number_of_points = 2, .row_number = 1, .column_number = 2, .node_points = {{1.21, 0.72}, {1.65, 0.59}} },
    {.number_of_points = 0, .row_number = 1, .column_number = 3, .node_points = {} }, //empty node
    {.number_of_points = 0, .row_number = 2, .column_number = 1, .node_points = {} }, 
    {.number_of_points = 5, .row_number = 2, .column_number = 2, .node_points = {{1.18, 1.34}, {1.72, 1.47}, {1.96, 1.22}, {1.29, 1.83}, {1.33, 1.01}} },
    {.number_of_points = 2, .row_number = 2, .column_number = 3, .node_points = {{2.73, 1.22}, {2.99, 1.11}} },
    {.number_of_points = 2, .row_number = 3, .column_number = 1, .node_points = {{0.91, 2.27}, {0.38, 2.65}} },
    {.number_of_points = 3, .row_number = 3, .column_number = 2, .node_points = {{1.82, 2.16}, {1.09, 2.70}, {1.68, 2.39}} },
    {.number_of_points = 1, .row_number = 3, .column_number = 3, .node_points = {{2.51, 2.76}} }
}; 

/*
graph_g current structure is like this:

3 x x x
2 x x x
1 x x x

Or where E is an empty node and N has a point

3 N N N
2 E N N
1 N N E
  1 2 3
*/

const unsigned int NUMBER_OF_NODES_IN_TREE = sizeof(graph_g) / sizeof(graph_g[0]) - 1; 
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
            
            if((row < 1 || col < 1) || (row > MAX_ROW_INDEX || col > MAX_COL_INDEX)){ //check if row or col are out of bounds
                continue;
            }
            if(neighbor_index < 0 || neighbor_index > NUMBER_OF_NODES_IN_TREE){ //check if index is out of bounds
                continue;
            }

            if(graph_g[i].number_of_points == 0 || graph_g[neighbor_index].number_of_points == 0){ //case of empty node
                continue;
            }
      
            graph_g[i].node_neighbors[j] = neighbor_index; //set neighbor index in node neighbors
        }
    }

    /*
    for(int j = 0; j < sizeof(graph_g) / sizeof(graph_g[0]); j++){
        printf("graph_g[%d]: ", j);
        for(int i = 0; i < TOTAL_NEIGHBORS; i++){
            if(graph_g[j].node_neighbors[i] != -1){            
                printf("%d, ", graph_g[j].node_neighbors[i]);
            }
        }   //PRINTS EXPECTED RESULT FOR all j
        printf("\n\n");
    } */
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

    printf("Pick a number greater than or equal to 0 and less than or equal to: %d to pick a node as the root: \n", NUMBER_OF_NODES_IN_TREE - 1);
    scanf("%d", &input_node);
    while(1){
        node_exists_in_array, node_not_empty = 0; //reset flags to 0 before next check

        if(input_node >= 0 && input_node <= NUMBER_OF_NODES_IN_TREE - 1){ //check input is in the range of nodes
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
            printf("Invalid input, pick a number >= 0 and <= %d, that is a non-empty node: \n", NUMBER_OF_NODES_IN_TREE - 1);
            scanf("%d", &input_node);
        }
    
    }

    return input_node;
}

double euclidean_distance(double x1, double y1, double x2, double y2){
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

/*function takes an index of a parent and child node.  iterates over each point in both nodes and computes euclidean distance. Returns a double of the smallest value*/
double compute_edge_weight_between_nodes(int parent, int child){
    double lowest = DBL_MAX; //init lowest value to infiinity

    for(int i=0; i < graph_g[parent].number_of_points; i++){ //loop through all of parents points
        for(int j=0; j < graph_g[child].number_of_points; j++){//for each parent point loop through each child point
            double computed_euclidean_distance = euclidean_distance(graph_g[parent].node_points[i].x_pos, graph_g[parent].node_points[i].y_pos, graph_g[child].node_points[j].x_pos, graph_g[child].node_points[j].y_pos); //pass in xy of current parent point and xy of current child point
            if(computed_euclidean_distance < lowest) lowest = computed_euclidean_distance;
        }
    }

    return lowest;
}

/*function initializes the parent index values of each node to -2, this will later be set to -1 for root or 0...n nodes for all not root nodes*/
void initialize_parent_node_indicies(){
    for(int i=0; i < sizeof(graph_g)/sizeof(graph_g[0]); i++){
        graph_g[i].parent_node_index = -2; //initialzie the index to less than zero and not -1 - this is root node.
    }
}

/*function takes the integer index of a node and checks all of its neighbors for any unvisited. If no unvisited, marks the node as a leaf*/
void check_is_leaf_node(int node_index){
    int num_unvisited_neighbors = 0;
    for(int k=0; k < TOTAL_NEIGHBORS; k++){
        int this_neighbor = graph_g[node_index].node_neighbors[k];
        if(!graph_g[this_neighbor].visited && this_neighbor >= 0){
            num_unvisited_neighbors++;
            //printf("%d : unvisited ", this_neighbor);
        }
    }

    if(num_unvisited_neighbors == 0){
        graph_g[node_index].is_leaf = 1;
        //printf("%d, I am a leaf!", node_index);
    }
}

/*function takes an index for a parent node and child node. Loops through the neighbors of the parent until it finds the k postion (up/down/L/R/etc)
of the child and then maps the edge*/
void map_edges_between_nodes(int child_index, int parent_index){
   
    if(parent_index < 0 || child_index < 0) return;
     
    double current_edge_weight = compute_edge_weight_between_nodes(parent_index, child_index);
    for (int k=0; k < TOTAL_NEIGHBORS; k++){
        if(graph_g[parent_index].node_neighbors[k] == child_index){
            graph_g[parent_index].set_of_edges[k].child_index = child_index; //define the edge between parent and child
            graph_g[parent_index].set_of_edges[k].weight = current_edge_weight; //set the weight of the edge
        }
    }
}

/*function takes the integer indicies of the parent and child as well as the index of a parent point and then checks each point in the child node, sums the weight and euclidean distance, then checks for the lowest among the points*/
double compute_point_weight_sum(int parent_index, int child_index, int point_index){
    double lowest = DBL_MAX; //initialize lowest to infinity
    
    for(int i = 0; i < graph_g[child_index].number_of_points; i++){ //iterate over all the points in the given child node
        double current_weight = graph_g[child_index].node_points[i].weight + euclidean_distance(graph_g[parent_index].node_points[point_index].x_pos, graph_g[parent_index].node_points[point_index].y_pos, graph_g[child_index].node_points[i].x_pos, graph_g[child_index].node_points[i].y_pos); // sum the weight of the euclidean distance between two current points and the weight of the child
        
        if(current_weight < lowest){ //check for new low
            lowest = current_weight;
        }
    }

    return lowest;
}

void tree_dfs(int node_index, int depth, int parent_index) {

    if (node_index == -1 || graph_g[node_index].visited) return;

    check_is_leaf_node(node_index);
    //if(parent_index > -1) map_edges_between_nodes(node_index, parent_index); //map edges and set weights if not root node

    graph_g[node_index].visited = 1;
    graph_g[node_index].parent_node_index = parent_index; //set parent to index of previous node

    int best_neighbor_to_visit = -2;
    double current_best_weight = INFINITY;

    for (int i = 0; i < TOTAL_NEIGHBORS; i++) { //iterate through neighbors of current node
        int neighbor_index = graph_g[node_index].node_neighbors[i];
        if(neighbor_index != -1 && !graph_g[neighbor_index].visited){ //check neighbor has not been visited and not empty
            double current_weight = compute_edge_weight_between_nodes(node_index, neighbor_index); //computes weight of edge between current node and current neighbor
            if(current_weight < current_best_weight) best_neighbor_to_visit = neighbor_index; //store index of current best neighbor node
        }
    
    if(best_neighbor_to_visit != -1 && !graph_g[best_neighbor_to_visit].visited){ //check neighbor is not empty or visitied
        tree_dfs(neighbor_index, depth+1, node_index);
        map_edges_between_nodes(neighbor_index, node_index); //map edge and set weight from parent (node) to child(parent)
        if(graph_g[neighbor_index].is_leaf){
            printf("Edge: leaf %d -> parent %d | edge weight = %.3f\n", neighbor_index, node_index, graph_g[node_index].set_of_edges[i].weight);
        }
        else if(graph_g[node_index].is_root_node){
            printf("Edge: child %d -> root %d | edge weight = %.3f\n", neighbor_index, node_index, graph_g[node_index].set_of_edges[i].weight);
        }
        else{
                printf("Edge: child %d -> parent %d | edge weight = %.3f\n", neighbor_index, node_index, graph_g[node_index].set_of_edges[i].weight);
        }
    }
    }

    if(!graph_g[node_index].is_leaf){ 
        for (int j = 0; j < graph_g[node_index].number_of_points; j++){
            double total_weight = 0.0;
            for(int k = 0; k < TOTAL_NEIGHBORS; k++){ 
                int child_index = graph_g[node_index].node_neighbors[k];
                if(child_index != -1 && graph_g[child_index].parent_node_index == node_index){ //check child is a valid index and its parent actually is the parent of our child
                    total_weight += compute_point_weight_sum(node_index, child_index, j);
                }
            }
            graph_g[node_index].node_points[j].weight = total_weight; //set the weight of our point
        }
    }
}

int get_count_non_empty_nodes_in_graph(){
    int node_count = 0;

    for(int i=0; i< (sizeof(graph_g) / sizeof(graph_g[0])); i++ ){
        if(graph_g[i].number_of_points > 0) node_count++;
    }

    return node_count;
}

int count_remaining_nodes(){
    int node_count = 0;
    for (int i=0; i < (sizeof(graph_g) / sizeof(graph_g[0])); i++){
        if(!graph_g[i].visited && graph_g[i].node_points > 0) node_count++;
    }
    return node_count;
}

void map_edges_prims(int parent, int child, double weight){
    graph_g[child].parent_node_index = parent;
    graph_g[child].visited = 1;
    for (int k=0; k < TOTAL_NEIGHBORS; k++){
        if(graph_g[parent].node_neighbors[k] == child){
            graph_g[parent].set_of_edges[k].child_index = child; //define the edge between parent and child
            graph_g[parent].set_of_edges[k].weight = weight; //set the weight of the edge
        }
    }
}

void prims_ggmst(int root_node, int non_empty_node_count){
    int nodes_visited = 1;

    graph_g[root_node].visited = 1;
    //array to store nodes | adding root and update index position tracking
    int ggmst[non_empty_node_count]; 
    ggmst[0] = root_node;
    int index_position_in_array = 1;

    while(nodes_visited < non_empty_node_count){
        double minimum_edge_weight = DBL_MAX;
        int minimum_neigbor_index = -2;
        //int current_node_pt, current_neighbor_pt;
        int current_parent_node = -2;

        for(int i=0; i < index_position_in_array; i++){ //iterate over nodes in mst
            int parent = ggmst[i];
            if(graph_g[parent].number_of_points == 0) continue; //skip if for some reason no points in parent

            for(int j=0; j < TOTAL_NEIGHBORS; j++){ //iterate over neighbors of current node in mst
                int current_neighbor = graph_g[parent].node_neighbors[j];

                if(current_neighbor < 0) continue; //skip to next iteration if OOB index

                if(!graph_g[current_neighbor].visited && graph_g[current_neighbor].number_of_points > 0){ // check they have valid neighbors

                    for(int pt_c=0; pt_c < graph_g[parent].number_of_points; pt_c++){ //iterate throguh points in current node

                        for(int k=0; k<graph_g[current_neighbor].number_of_points; k++){ //iterate through points in current eligible neighbor
                            double weight_of_edge = euclidean_distance(graph_g[parent].node_points[pt_c].x_pos, graph_g[parent].node_points[pt_c].y_pos, graph_g[current_neighbor].node_points[k].x_pos, graph_g[current_neighbor].node_points[k].y_pos); //get weight

                            if(weight_of_edge < minimum_edge_weight){
                                minimum_edge_weight = weight_of_edge;
                                //current_node_pt = pt_c;
                               // current_neighbor_pt = k;
                                minimum_neigbor_index = current_neighbor;
                                current_parent_node = parent;
                            }
                        }
                    }
                }
            }
        }

        //if no edges found
        if(current_parent_node < 0 || minimum_neigbor_index < 0){
            printf("No valid edge was found: visited %d | of %d possible", nodes_visited, non_empty_node_count);
            break;
        }
       
        //store the node and map the edge
        ggmst[index_position_in_array] = minimum_neigbor_index;
        index_position_in_array++;

        map_edges_prims(current_parent_node, minimum_neigbor_index, minimum_edge_weight);
        nodes_visited++;
    }
}

void reset_visited_flag() {
  for (int i=0; i <= NUMBER_OF_NODES_IN_TREE; i++) {
    graph_g[i].visited = 0;
  }
}

/*Function explicitly sets indices to -2 and weights to 0*/
void initialzie_edges(){
    for(int i=0; i < (sizeof(graph_g) / sizeof(graph_g[0])); i++){
        for (int j=0; j < TOTAL_NEIGHBORS; j++){
            graph_g[i].set_of_edges[j].child_index = -2;
            graph_g[i].set_of_edges[j].weight = 0.0;
        }
    }
}

/*Function uses DFS to print a branched view of the tree using prims. Assumes run of prims_ggmst first.*/
void print_tree(int current_node, int depth){
    if(current_node < 0) return;

    for(int j=0; j < depth; j++){
        if(j == depth -1) printf("-  ");
        else printf("   ");
    }

    printf("%d\n", current_node);

    for(int i=0; i < TOTAL_NEIGHBORS; i++){
        int child_index = graph_g[current_node].set_of_edges[i].child_index;
        if(child_index != -2){
            print_tree(child_index, depth+1);
        }
    }
}

/*function takes and assumes populated edges by running prims_ggmst first. Uses dynammic programming approach to traverse tree from computes the minimum weight cost betwen points in all of the child nodes*/
void compute_prim_tree_weight(int current_node){
    if(current_node < 0) return;

    int store_children[TOTAL_NEIGHBORS]; //array to store children of current node
    int count_of_children = 0;

    for (int i=0; i<TOTAL_NEIGHBORS; i++){ //loop through edges
        int current_child_index = graph_g[current_node].set_of_edges[i].child_index;
        if(current_child_index >= 0){ //check is valid index
            store_children[count_of_children] = current_child_index;//store child and recurse
            count_of_children++;
            compute_prim_tree_weight(current_child_index);
        }
    }

    for(int j=0; j<graph_g[current_node].number_of_points; j++){ //iterate over points in node
        double total_weight = 0.0; 
        for(int k = 0; k < count_of_children; k++){  //iterate over the stored children 
            int child_index = store_children[k];
            total_weight += compute_point_weight_sum(current_node, child_index, j); //compute min distance between points 
            
        }
        graph_g[current_node].node_points[j].weight = total_weight; //set the weight of our point
    }
}

/*Function prompts user to pick which algorithm to run*/
int pick_prims_or_dfs(){
    int algorithm_select;
    while(1){
        algorithm_select = -1;
        printf("Please choose Prims (1) or DFS(2): \n");
        scanf("%d", &algorithm_select);

        if(algorithm_select == 1 || algorithm_select == 2) break;
    }

    return algorithm_select;

}

int main(){
    //max row and col actualy val
    MAX_ROW_INDEX = graph_g[NUMBER_OF_NODES_IN_TREE].row_number;
    MAX_COL_INDEX = graph_g[NUMBER_OF_NODES_IN_TREE].column_number;
    //number of rows and cols
    MAX_COL_NUMBER  = MAX_COL_INDEX;
    MAX_ROW_NUMBER = MAX_ROW_INDEX;

    const int root_node_index = pick_root_node_of_tree();
    graph_g[root_node_index].is_root_node = 1;
    graph_g[root_node_index].parent_node_index = -1; //set root node and its parent index as -1 - all nodes will have a positive parent index but root

    //setup datastructures 
    initialize_parent_node_indicies();
    find_neighbors();
    reset_visited_flag();
    initialzie_edges();

    int selection = pick_prims_or_dfs();

    //select prims or dfs algorithm
    switch(selection){
        case(1):
            int n_nodes = get_count_non_empty_nodes_in_graph();
            prims_ggmst(root_node_index, n_nodes);
            printf("View of the tree with Prims: \n");
            print_tree(root_node_index, 0);
            compute_prim_tree_weight(root_node_index);
            break;
        case(2):
            printf("Tree View\n");
            tree_dfs(root_node_index, 0, -1); //negative 1 for root parent
            break;
    }
    
    double minimum = INFINITY;
    for (int i = 0; i < graph_g[root_node_index].number_of_points; i++){ //loop through the points of root node
        if (graph_g[root_node_index].node_points[i].weight < minimum) //find minimum weight 
        minimum = graph_g[root_node_index].node_points[i].weight; 
    }

    printf("GGMST total weight = %.3f\n", minimum);
    return 0;
}