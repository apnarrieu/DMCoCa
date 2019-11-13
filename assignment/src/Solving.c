#include "Solving.h"
#include "Z3Tools.h"
#include <stdlib.h>
#include <stdio.h>

Z3_ast getNodeVariable(Z3_context ctx, int number, int position, int k, int node){
    char s[30];
    snprintf(s, 30, "X%d%d%d%d", number, position, k, node);
    Z3_ast var = mk_bool_var(ctx, s);
    return var;
}

Z3_ast Path(Z3_context ctx, Graph* graphs, unsigned int numGraphs, int pathLength){
    Z3_ast* kappa1; //il existe au moins une node
    Z3_ast* kappa2; //cette node est a la position j avec 0<j<pathLength
    Z3_ast* kappa3; //pour chaque graphe
    Z3_ast phi1; //phi1
    for(int i=0; i<numGraphs; i++){
        kappa3=malloc(numGraph*sizeof(Z3_ast))
        for(int j=0; i<pathLength; j++){
            kappa2()
            int len_graph = graphs[i]->numNodes;
            kappa1 = malloc(len_graph*sizeof(Z3_ast));
            for(int n = 0; n<len_graph; n++){
                kappa1[n] = getNodeVariable(ctx, i, j, pathLength, n);
            }
            kappa2[j]=Z3_mk_or(ctx, len_graph, kappa1);
            free(kappa1);
        }
        kappa3[i]=Z3_mk_and(ctx, pathLength, kappa2);
        free(kappa2);
    }
    phi1=Z3_mk_and(ctx, pathLength, kappa3);
    free(kappa3);
    return phi1;
}

Z3_ast SimplePath(Z3_context ctx, Graph* graphs, unsigned int numGraphs, int pathLength){
  Z3_ast = phi2;
  return phi2;
}

Z3_ast ValidPath(Z3_context ctx, Graph* graphs, unsigned int numGraphs, int pathLength){
  Z3_ast = phi3;
  return phi3;
}

Z3_ast PathLenK(Z3_context ctx, Graph* graphs, unsigned int numGraphs, int pathLength){
  Z3_ast = phi4;
  return phi4;
}

/*Z3_ast graphsToPathFormula( Z3_context ctx, Graph *graphs,unsigned int numGraphs, int pathLength){
    Z3_ast var = NULL;
    for(int i=0; i<numGraphs; i++){
        for(int j=0; i<pathLength; j++){

            var = getNodeVariable(ctx, i, j, pathLength, );
        }
    }
    return var;
}*/
