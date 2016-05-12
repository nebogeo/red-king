%module redking
%{
#include "../src/model/model.h"
%}

%typedef float rk_real;

%include "../src/model/model.h"


%include carrays.i
%array_functions(rk_real, rk_real)

%include cpointer.i

%pointer_class(rk_real, rk_realp)
