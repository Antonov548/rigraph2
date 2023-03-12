#include "graphalt.h"


static void delete_graph(SEXP xp){
  auto* c_graph = static_cast<igraph_t*>(R_ExternalPtrAddr(xp));
  igraph_destroy(c_graph);
  delete c_graph;
}

SEXP create_graph(igraph_t* graph) {
  SEXP xp = PROTECT(R_MakeExternalPtr(graph, R_NilValue, R_NilValue));

  R_RegisterCFinalizerEx(xp, delete_graph, TRUE);
  UNPROTECT(1);

  return xp;
}