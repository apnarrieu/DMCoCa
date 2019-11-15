#include "Solving.h"
#include "Z3Tools.h"
//#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Parsing.h>

int main(int argc, char* argv[]){

  Z3_context ctx = makeContext();

  if(strcmp(argv[1],"test")==0){
    Z3_ast var = getNodeVariable(ctx, 1, 2, 3, 4);
    if(strcmp(Z3_ast_to_string(ctx,var),"X1234")==0){
      printf("getNodeVariable passed\n");
    }
    else{
      printf("getNodeVariable not passed\n");
    }
  }

  if(strcmp(argv[1],"cd")==0){
    if(argc<=2){
      printf("no graph passed in the call of the program\n");
      return 1;
    }
    Graph* graphs = (Graph*)malloc(argc-2);
    int iterator = 0;
    for(int i=2; i<argc;i++){
      graphs[iterator] = getGraphFromFile(argv[i]);
      //printGraph(graphs[iterator]);
      iterator++;
    }
    Z3_ast Phi = graphsToFullFormula(ctx, graphs, argc-2);
    printf("The value of %s is %d\n",Z3_ast_to_string(ctx,Phi));
    printf("Is Phi satisfiable? %d\n", isFormulaSat(ctx, Phi));
  }

  Z3_del_context(ctx);

  return 0;
}
