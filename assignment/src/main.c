#include "Solving.h"
#include "Z3Tools.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    Z3_context ctx = makeContext();

    Z3_ast var = getNodeVariable(ctx, 1, 2, 3, 4);
    printf("The value of %s is %d\n",Z3_ast_to_string(ctx,var));
    printf("Is is Satisfiable? %d\n",isFormulaSat(ctx, var));




    Z3_del_context(ctx);
    return 0;
}
