/*
 * printout.h
 * Print out the data collected from the monte carlo program
 * for further investigation in matlab
 */
#include <stdio.h>
#include <string.h>
extern int grid_res;
extern double **I_Ref_H, **I_Ref_V, **I_Ref_P, **I_Ref_M, **I_Ref_R, **I_Ref_L;
extern double **Q_Ref_H, **Q_Ref_V, **Q_Ref_P, **Q_Ref_M, **Q_Ref_R, **Q_Ref_L;
extern double **U_Ref_H, **U_Ref_V, **U_Ref_P, **U_Ref_M, **U_Ref_R, **U_Ref_L;
extern double **V_Ref_H, **V_Ref_V, **V_Ref_P, **V_Ref_M, **V_Ref_R, **V_Ref_L;

void printout(const char* prefix) {
    char op_file_name[] = "output.m";
    int total_size = strlen(op_file_name) + strlen(prefix) + 1;
    char* filename = new char[total_size];
    strncpy(filename, prefix, total_size);
    strncat(filename, op_file_name, total_size);
    /* Printing Output Matrices */
    FILE* op_matrix;
    op_matrix = fopen(filename, "w");
    // Output Profile 1
    fprintf(op_matrix, "I_R_1 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", I_Ref_H[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "Q_R_1 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", Q_Ref_H[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "U_R_1 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", U_Ref_H[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "V_R_1 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", V_Ref_H[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");
    // Add MATLAB processing script for easy visualization
    //fprintf(op_matrix, "figure();\nsubplot(2,2,1);\nimagesc(I_R_1);\ntitle('I_1'); \nsubplot(2,2,2);\nimagesc(Q_R_1);\ntitle('Q_1'); \nsubplot(2,2,3);\nimagesc(U_R_1);\ntitle('U_1'); \nsubplot(2,2,4);\nimagesc(V_R_1);\ntitle('V_1'); \n");

    // Output Profile 2

    fprintf(op_matrix, "I_R_2 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", I_Ref_V[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "Q_R_2 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", Q_Ref_V[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "U_R_2 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", U_Ref_V[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "V_R_2 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", V_Ref_V[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");
    // Add MATLAB processing script for easy visualization
    //fprintf(op_matrix, "figure();\nsubplot(2,2,1);\nimagesc(I_R_2);\ntitle('I_2'); \nsubplot(2,2,2);\nimagesc(Q_R_2);\ntitle('Q_2'); \nsubplot(2,2,3);\nimagesc(U_R_2);\ntitle('U_2'); \nsubplot(2,2,4);\nimagesc(V_R_2);\ntitle('V_2'); \n");

    fprintf(op_matrix, "I_R_3 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", I_Ref_M[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "Q_R_3 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", Q_Ref_M[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "U_R_3 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", U_Ref_M[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "V_R_3 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", V_Ref_M[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    // Output Profile 3
    
    fprintf(op_matrix, "I_R_4 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", I_Ref_P[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "Q_R_4 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", Q_Ref_P[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "U_R_4 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", U_Ref_P[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "V_R_4 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", V_Ref_P[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");
    // Add MATLAB processing script for easy visualization
    //fprintf(op_matrix, "figure();\nsubplot(2,2,1);\nimagesc(I_R_3);\ntitle('I_3'); \nsubplot(2,2,2);\nimagesc(Q_R_3);\ntitle('Q_3'); \nsubplot(2,2,3);\nimagesc(U_R_3);\ntitle('U_3'); \nsubplot(2,2,4);\nimagesc(V_R_3);\ntitle('V_3'); \n");

    // Output Profile 5
    fprintf(op_matrix, "I_R_5 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", I_Ref_R[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "Q_R_5 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", Q_Ref_R[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "U_R_5 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", U_Ref_R[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "V_R_5 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", V_Ref_R[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");
    // Add MATLAB processing script for easy visualization
//    fprintf(op_matrix, "figure();\nsubplot(2,2,1);\nimagesc(I_R_4);\ntitle('I_4'); \nsubplot(2,2,2);\nimagesc(Q_R_4);\ntitle('Q_4'); \nsubplot(2,2,3);\nimagesc(U_R_4);\ntitle('U_4'); \nsubplot(2,2,4);\nimagesc(V_R_4);\ntitle('V_4'); \n");

    fprintf(op_matrix, "I_R_6 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", I_Ref_L[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "Q_R_6 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", Q_Ref_L[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "U_R_6 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", U_Ref_L[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fprintf(op_matrix, "V_R_6 = [");
    for (int i = 0; i < grid_res; i ++) {
        for (int j = 0; j < grid_res; j ++) {
            fprintf(op_matrix, "%f ", V_Ref_L[i][j]);    
        }
        fprintf(op_matrix,";\n");
    }
    fprintf(op_matrix,"];\n");

    fclose(op_matrix);
    delete [] filename;
}
