#include "Solving.h"
#include "Z3Tools.h"
#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>

Z3_ast getNodeVariable(Z3_context ctx, int number, int position, int k, int node){
    char s[30];
    snprintf(s, 30, "X%d%d%d%d", number, position, k, node);
    Z3_ast var = mk_bool_var(ctx, s);
    return var;
}

void free_tab(void* tab, lentab){
  for(int i=0; i<lentab; i++){
    free(tab[i]);
  }
}

Z3_ast Path(Z3_context ctx, Graph* graphs, unsigned int numGraphs, int pathLength){
    Z3_ast* kappa1; //il existe au moins une node
    Z3_ast* kappa2; //cette node est a la position j avec 0<j<pathLength
    Z3_ast* kappa3; //pour chaque graphe
    Z3_ast phi1; //phi1
    kappa3=(Z3_ast*)malloc(numGraphs*sizeof(Z3_ast));
    for(int i=0; i<numGraphs; i++){
      kappa2=(Z3_ast*)malloc(pathLength*sizeof(Z3_ast*));
        for(int j=0; i<pathLength; j++){
            int len_graph = orderG(graphs[i]);
            kappa1 = (Z3_ast*)malloc(len_graph*sizeof(Z3_ast*));
            for(int n = 0; n<len_graph; n++){
                kappa1[n] = getNodeVariable(ctx, i, j, pathLength, n);
            }
            kappa2[j]=Z3_mk_or(ctx, len_graph, kappa1);
            free_tab(kappa1, len_graph);
        }
        kappa3[i]=Z3_mk_and(ctx, pathLength, kappa2);
        free_tab(kappa2, pathLength);
    }
    phi1=Z3_mk_and(ctx, numGraphs, kappa3);
    free_tab(kappa3,numGraphs);
    return phi1;
}

Z3_ast SimplePath(Z3_context ctx, Graph* graphs, unsigned int numGraphs, int pathLength){
  Z3_ast phi2;
  Z3_ast* kappa1;
  Z3_ast* kappa2;
  Z3_ast* kappa3 = (Z3_ast*)malloc(numGraphs*sizeof(Z3_ast));
  for(int i=0; i<numGraphs; i++){
    kappa2=(Z3_ast*)malloc(pathLength*sizeof(Z3_ast));
    for(int j=0; j<pathLength; j++){
      int len_graph = orderG(graphs[i]);
      kappa1 = (Z3_ast*)malloc(len_graph*len_graph*sizeof(Z3_ast));
      Z3_ast tmp[2];
      int iterator = 0;
      for(int q = 0; q<len_graph; q++){
        for(int r = q; r<len_graph; r++){
          tmp[0] = Z3_mk_not(ctx, getNodeVariable(ctx, i, j, pathLength, q));
          tmp[1] = Z3_mk_not(ctx, getNodeVariable(ctx, i, j, pathLength, r));
          kappa1[iterator] = Z3_mk_or(ctx, 2, tmp);
          iterator++;
        }
      }
      kappa2[j]=Z3_mk_and(ctx, len_graph*len_graph, kappa1);
      free_tab(kappa1,len_graph*len_graph);
    }
    kappa3[i]=Z3_mk_and(ctx, pathLength, kappa2);
    free_tab(kappa2,pathLength);
  }
  phi2 = Z3_mk_and(ctx, numGraphs, kappa3);
  free_tab(kappa3,numGraphs);
  return phi2;
}

Z3_ast ValidPath(Z3_context ctx, Graph* graphs, unsigned int numGraphs, int pathLength){
  Z3_ast* kappa1; //Pour chaque graphe
  Z3_ast* kappa2; //Il existe un chemin allant de j à k
  Z3_ast* kappa3=(Z3_ast*)malloc(numGraphs*sizeof(Z3_ast));; //Pour chaque node s (source) et t (destination)
  Z3_ast phi3; //phi3
  for(int i=0; i<numGraphs; i++){
    for(int j=0; j<pathLength; j++){
      int len_graph = orderG(graphs[i]);
      kappa1 = (Z3_ast*)malloc(len_graph*len_graph*sizeof(Z3_ast));
      Z3_ast tmp[2];
      int iterator = 0;
      for(int s = 0; s<len_graph; s++){
        for(int t = q; t<len_graph; t++){
          tmp[0] = getNodeVariable(ctx, i, 0, pathLength, s);
          tmp[1] = getNodeVariable(ctx, i, pathLength, pathLength, t);
          kappa1[iterator] = Z3_mk_and(ctx, 2, tmp);
          iterator++;
        }
      }
      kappa2[j]=Z3_mk_and(ctx, len_graph*len_graph, kappa1);
      free_tab(kappa1,len_graph*len_graph);
    }
    kappa3[i]=Z3_mk_or(ctx, pathLength, kappa2);
    free_tab(kappa2,pathLength);
  }
  phi3=Z3_mk_and(ctx, pathLength, kappa3);
  free_tab(kappa3,numGraphs);
  return phi3;
}

Z3_ast PathLenK(Z3_context ctx, Graph* graphs, unsigned int numGraphs, int pathLength){
  Z3_ast* kappa1;
  Z3_ast* kappa2;
  Z3_ast* kappa3 = (Z3_ast*)malloc(numGraphs*sizeof(Z3_ast));
  Z3_ast phi4;
  for(int i=0; i<numGraphs; i++){
    kappa2 = (Z3_ast*)malloc((orderG(graphs[i]))*sizeof(Z3_ast));
    for(int q=0; q<orderG(graphs[i]); q++){
      kappa1 = (Z3_ast*)malloc(pathLength*sizeof(Z3_ast));
      for(int j=0;j<pathLength;j++){
        kappa1[j]=getNodeVariable(ctx, i, j, pathLength, q);
      }
      kappa2[i]=Z3_mk_or(ctx, pathLength, kappa1);
      free_tab(kappa1, pathLength);
    }
    kappa3[i]=Z3_mk_and(ctx,(orderG(graphs[i])),kappa2);
    free_tab(kappa2,orderG(graphs[i]));
  }
  phi4=Z3_mk_and(ctx,numGraphs,kappa3);
  free_tab(kappa3,numGraphs);
  return phi4;
}

Z3_ast graphsToPathFormula( Z3_context ctx, Graph *graphs,unsigned int numGraphs, int pathLength){
    Z3_ast phi;
    Z3_ast underPhi[4];
    underPhi[0] = Path(ctx, graphs, numGraphs, pathLength);
    underPhi[1] = SimplePath(ctx, graphs, numGraphs, pathLength);
    underPhi[2] = ValidPath(ctx, graphs, numGraphs, pathLength);
    underPhi[3] = PathLenK(ctx, graphs, numGraphs, pathLength);
    phi = Z3_mk_and(ctx, 4, underPhi);
    return phi;
}

Z3_ast graphsToFullFormula( Z3_context ctx, Graph *graphs,unsigned int numGraphs){
  int max_k = orderG(graphs[0]);

  for(int i=1; i<numGraphs; i++){
    if(max_k>orderG(graphs[i])){
      max_k = orderG(graphs[i]);
    }
  }

  Z3_ast kappa[max_k];
  for(int i=0;i<max_k;i++){
    kappa[i]=graphsToPathFormula(ctx, graphs, numGraphs, i);
  }
  Z3_ast phi=Z3_mk_or(ctx, max_k, kappa);
  return phi;
}
