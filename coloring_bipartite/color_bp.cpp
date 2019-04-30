#include<iostream>
#include<stdio.h>
#include<vector>
#include<algorithm>
// To execute use the command g++ color_bp.cpp && ./a.out < a.dat

using namespace std;

int element_in_row(int edges[50][50], int row_index, int color_value){
    for(int i = 0; i < 50; i++){
        if(color_value == edges[row_index][i]){
            return i;
        }
    }
    return -1;
}

int element_in_column(int edges[50][50], int column_index, int color_value){
    for(int i = 0; i < 50; i++){
        if(color_value == edges[i][column_index]){
            return i;
        }
    }
    return -1;
}

int color_not_in_row(int edges[50][50], int row_index, int max_color){
    for(int i = 0; i < 50; i++){
        for(int j = 0; j < max_color; j++){
            if(element_in_row(edges, row_index, j) == -1){
                return j;
            }
        }
    }   
    return -1;
}

int color_not_in_column(int edges[50][50], int column_index, int max_color){
    for(int i = 0; i < 50; i++){
        for(int j = 0; j < max_color; j++){
            if(element_in_column(edges, column_index, j) == -1){
                return j;
            }
        }
    }   
    return -1;
}

void print_array(int edges[50][50], int row, int column){
    for(int i = 0; i < column; i++){
        // printing out the 2D array
        printf("edgeColors[%d]=", i);
        for(int j = 0; j < row; j++){
            printf("%d ", edges[i][j]);
        }  
        printf("\n");
    }

    printf("\n");
}

// void switch_color(int & a, int & b){
//     int temp = a;
//     a = b;
//     b = temp;
// }

void find_element_in_row(int edges[50][50], int row, int old_color, int new_color);

void find_element_in_column(int edges[50][50], int column, int old_color, int new_color){
    int existing_row = element_in_column(edges, column, old_color);
    if(existing_row != -1){
        // printf("l:%d ", existing_row);
        find_element_in_row(edges, existing_row, old_color, new_color);
        edges[existing_row][column] = new_color;
    }
}

void find_element_in_row(int edges[50][50], int row, int old_color, int new_color){
    int existing_column = element_in_row(edges, row, new_color);
    if(existing_column != -1){
        printf("l:%d r:%d ", row, existing_column);
        find_element_in_column(edges, existing_column, old_color, new_color);
        edges[row][existing_column] = old_color;
    }
}

void find_alternating_paths(int edges[50][50], int row, int column, int max_color){
    int source_color = edges[row][column];
    int new_color = color_not_in_column(edges, column, max_color);
    
    printf("b=%d r:%d ", new_color, column);

    int existing_row = row;
    int existing_column = column;

    find_element_in_row(edges, row, source_color, new_color);
    edges[row][column] = new_color;
}

int main(int argc, char const *argv[])
{
    int nl, nr, m;
    cin >> nl >> nr >> m;

    int edges[m][3];
    int edgeColors[50][50];
    int outgoingEdges[m];
    int incomingEdges[m];

    fill_n(outgoingEdges, m , 0);
    fill_n(incomingEdges, m , 0);
    
    for(int i = 0; i < 50; i++){
        fill_n(edgeColors[i], 50, -1);
    }
    
    int delta = 0;

    for(int i = 0; i < m; i++){
        int leftV, rightV;
        cin >> leftV >> rightV;

        outgoingEdges[leftV]++;
        incomingEdges[rightV]++;

        edgeColors[leftV][rightV] = -2;
    }
    
    int maxOutgoingEdges = *max_element(outgoingEdges, outgoingEdges + m);
    int maxIncomingEdges = *max_element(incomingEdges, incomingEdges + m);

    delta = maxOutgoingEdges;
    if(maxIncomingEdges > delta){
        delta = maxIncomingEdges;
    }

    // print_array(edgeColors, nr, nl);

    int count = -1;

    for(int i = 0; i <= nl; i++){
        for(int j = 0; j <= nr; j++){
            for(int k = 0; k < delta; k++){
                if(edgeColors[i][j] != -1 && element_in_row(edgeColors, i, k) == -1 && element_in_column(edgeColors, j, k) == -1){
                    edgeColors[i][j] = k;
                    count++;
                    printf("Case 1: edge %d (%d %d) gets color %d\n", count, i, j, k);
                    break;
                }
            }

            if(edgeColors[i][j] == -2){
                count++;
                int new_color = color_not_in_row(edgeColors, i, delta);
                int inspect_row = element_in_column(edgeColors, j, new_color);
                printf("Case 2: edge %d (%d %d) gets color a=%d, swaps with ", count, i, j, new_color);
                find_alternating_paths(edgeColors, inspect_row, j, delta);
                edgeColors[i][j] = new_color;
                printf("\n");
            }
        }
    }

    // print_array(edgeColors, nr, nl);

    for(int i=0; i<nl; i++){
        for(int j=0; j<nl; j++){
            if(edgeColors[i][j] != -1){
                printf("%d %d %d\n", i, j, edgeColors[i][j]);
            }
        }
    }

    return 0;
}
