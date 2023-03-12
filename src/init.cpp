#include "uuid/uuid.h"

#include "igraph_datatype.h"
#include "igraph_interface.h"
#include "igraph_vector.h"
#include "igraph_constructors.h"
#include "igraph_conversion.h"
#include "igraph_vector_list.h"

#include "graphalt.h"

#include <math.h>
#include <vector>

#define R_IGRAPH_TYPE_VERSION "0.8.0"
#define R_IGRAPH_VERSION_VAR ".__igraph_version__."

#if IGRAPH_INTEGER_SIZE == 64
#error "Error"
#endif

SEXP R_igraph2_warning(void)
{
  Rf_warning("hello world");
  return R_NilValue;
}

SEXP R_igraph_add_env(SEXP graph) {
  SEXP result = graph;
  int i;
  uuid_t my_id;
  char my_id_chr[40];
  int px = 0;

  if (GET_LENGTH(graph) != 10) {
    PROTECT(result = NEW_LIST(10)); px++;
    for (i = 0; i < 9; i++) {
      SET_VECTOR_ELT(result, i, duplicate(VECTOR_ELT(graph, i)));
    }
    SET_ATTRIB(result, duplicate(ATTRIB(graph)));
    SET_CLASS(result, duplicate(GET_CLASS(graph)));
  }

  SET_VECTOR_ELT(result, 9, allocSExp(ENVSXP));

  uuid_generate(my_id);
  uuid_unparse_lower(my_id, my_id_chr);

  SEXP l1 = PROTECT(install("myid")); px++;
  SEXP l2 = PROTECT(mkString(my_id_chr)); px++;
  defineVar(l1, l2, VECTOR_ELT(result, 9));

  l1 = PROTECT(install(R_IGRAPH_VERSION_VAR)); px++;
  l2 = PROTECT(mkString(R_IGRAPH_TYPE_VERSION)); px++;
  defineVar(l1, l2, VECTOR_ELT(result, 9));

  UNPROTECT(px);

  return result;
}

SEXP R_igraph_to_SEXP(const igraph_t *graph) {

  SEXP result;
  long int no_of_nodes=igraph_vcount(graph);
  long int no_of_edges=igraph_ecount(graph);

  PROTECT(result=NEW_LIST(10));
  SET_VECTOR_ELT(result, 0, NEW_INTEGER(1));
  SET_VECTOR_ELT(result, 1, NEW_LOGICAL(1));
  SET_VECTOR_ELT(result, 2, NEW_INTEGER(no_of_edges));
  SET_VECTOR_ELT(result, 3, NEW_INTEGER(no_of_edges));
  SET_VECTOR_ELT(result, 4, NEW_INTEGER(no_of_edges));
  SET_VECTOR_ELT(result, 5, NEW_INTEGER(no_of_edges));
  SET_VECTOR_ELT(result, 6, NEW_INTEGER(no_of_nodes+1));
  SET_VECTOR_ELT(result, 7, NEW_INTEGER(no_of_nodes+1));

  INTEGER(VECTOR_ELT(result, 0))[0]=static_cast<uint32_t>(no_of_nodes);
  LOGICAL(VECTOR_ELT(result, 1))[0]=graph->directed;
  memcpy(INTEGER(VECTOR_ELT(result, 2)), graph->from.stor_begin,
         sizeof(igraph_integer_t)*(size_t) no_of_edges);
  memcpy(INTEGER(VECTOR_ELT(result, 3)), graph->to.stor_begin,
         sizeof(igraph_integer_t)*(size_t) no_of_edges);
  memcpy(INTEGER(VECTOR_ELT(result, 4)), graph->oi.stor_begin,
         sizeof(igraph_integer_t)*(size_t) no_of_edges);
  memcpy(INTEGER(VECTOR_ELT(result, 5)), graph->ii.stor_begin,
         sizeof(igraph_integer_t)*(size_t) no_of_edges);
  memcpy(INTEGER(VECTOR_ELT(result, 6)), graph->os.stor_begin,
         sizeof(igraph_integer_t)*(size_t) (no_of_nodes+1));
  memcpy(INTEGER(VECTOR_ELT(result, 7)), graph->is.stor_begin,
         sizeof(igraph_integer_t)*(size_t) (no_of_nodes+1));

  SET_CLASS(result, ScalarString(CREATE_STRING_VECTOR("igraph2")));

  // Ignroe attributes. No logic with initialization of attributes table
  /* Attributes */
  // SET_VECTOR_ELT(result, 8, reinterpret_cast<SEXP>(graph->attr));
  // REAL(VECTOR_ELT(reinterpret_cast<SEXP>(graph->attr), 0))[0] += 1;

  /* Environment for vertex/edge seqs */
  SET_VECTOR_ELT(result, 9, R_NilValue);
  R_igraph_add_env(result);

  UNPROTECT(1);
  return result;
}

int R_SEXP_to_vector(SEXP sv, igraph_vector_int_t *v) {
  v->stor_begin=INTEGER(sv);
  v->stor_end=v->stor_begin+GET_LENGTH(sv);
  v->end=v->stor_end;
  return 0;
}

int R_SEXP_to_vector(SEXP sv, igraph_vector_t *v) {
  v->stor_begin=REAL(sv);
  v->stor_end=v->stor_begin+GET_LENGTH(sv);
  v->end=v->stor_end;
  return 0;
}

std::vector<igraph_integer_t> to_vector(SEXP sv)
{
  std::vector<igraph_integer_t> result;
  auto* values = INTEGER(sv);
  auto size = GET_LENGTH(sv);

  result.resize(size);
  for (int i = 0; i < size; ++i) result[i] = static_cast<igraph_integer_t>(values[i]);
  return result;
}

SEXP R_igraph_create(SEXP edges, SEXP pn, SEXP pdirected) {
  igraph_t g;
  igraph_vector_int_t v;
  igraph_integer_t n=(igraph_integer_t) INTEGER(pn)[0];
  igraph_bool_t directed=LOGICAL(pdirected)[0];
  SEXP result;

  R_SEXP_to_vector(edges, &v);

  igraph_create(&g, &v, n, directed);
  PROTECT(result=R_igraph_to_SEXP(&g));
  igraph_destroy(&g);

  UNPROTECT(1);
  return result;
}


int R_SEXP_to_igraph(SEXP graph, igraph_t *res) {

  res->n=(igraph_integer_t) INTEGER(VECTOR_ELT(graph, 0))[0];
  res->directed=LOGICAL(VECTOR_ELT(graph, 1))[0];
  R_SEXP_to_vector(VECTOR_ELT(graph, 2), &res->from);
  R_SEXP_to_vector(VECTOR_ELT(graph, 3), &res->to);
  R_SEXP_to_vector(VECTOR_ELT(graph, 4), &res->oi);
  R_SEXP_to_vector(VECTOR_ELT(graph, 5), &res->ii);
  R_SEXP_to_vector(VECTOR_ELT(graph, 6), &res->os);
  R_SEXP_to_vector(VECTOR_ELT(graph, 7), &res->is);

  // Ignroe attributes. No logic with initialization of attributes table
  /* attributes */
  //REAL(VECTOR_ELT(VECTOR_ELT(graph, 8), 0))[0] = 1; /* R objects refcount */
  //REAL(VECTOR_ELT(VECTOR_ELT(graph, 8), 0))[1] = 0; /* igraph_t objects */
  //res->attr=VECTOR_ELT(graph, 8);

  return 0;
}

SEXP R_igraph_finalizer(void) {
  IGRAPH_FINALLY_FREE();
  return R_NilValue;
}

SEXP R_igraph_get_edgelist(SEXP graph, SEXP pbycol) {

  igraph_t g;
  igraph_vector_int_t res;
  igraph_bool_t bycol=LOGICAL(pbycol)[0];
  SEXP result;

  R_SEXP_to_igraph(graph, &g);
  igraph_vector_int_init(&res, 0);
  igraph_get_edgelist(&g, &res, bycol);
  PROTECT(result=NEW_INTEGER(igraph_vector_int_size(&res)));
  igraph_vector_int_copy_to(&res, INTEGER(result));
  igraph_vector_int_destroy(&res);

  UNPROTECT(1);
  return result;
}

SEXP R_igraph_finalizer2(void) {
  IGRAPH_FINALLY_FREE();
  SEXP l1 = PROTECT(install("getNamespace"));
  SEXP l2 = PROTECT(ScalarString(mkChar("igraph2")));
  SEXP l3 = PROTECT(lang2(l1, l2));
  SEXP rho = PROTECT(EVAL(l3));
  SEXP l4 = PROTECT(install(".igraph.progress"));
  SEXP l5 = PROTECT(ScalarReal(0.0));
  SEXP l6 = PROTECT(ScalarString(mkChar("")));
  SEXP l7 = PROTECT(ScalarLogical(1));
  SEXP l8 = PROTECT(lang4(l4, l5, l6, l7));
  eval(l8, rho);
  UNPROTECT(9);
  return R_NilValue;
}

// empty graph using vectors
SEXP R_igraph_empty2(SEXP n, SEXP directed) {
  igraph_t c_graph;
  igraph_integer_t c_n;
  igraph_bool_t c_directed;
  SEXP graph;

  SEXP r_result;
                                        /* Convert input */
  c_n=INTEGER(n)[0];
  c_directed=LOGICAL(directed)[0];
                                        /* Call igraph */
  igraph_empty(&c_graph, c_n, c_directed);

                                        /* Convert output */
  IGRAPH_FINALLY(igraph_destroy, &c_graph);
  PROTECT(graph=R_igraph_to_SEXP(&c_graph));
  igraph_destroy(&c_graph);
  IGRAPH_FINALLY_CLEAN(1);
  r_result = graph;

  UNPROTECT(1);
  return (r_result);
}

// empty igraph using raw pointer
SEXP R_igraph_empty(SEXP n, SEXP directed) {
                                        /* Declarations */
  igraph_t* c_graph;
  igraph_integer_t c_n;
  igraph_bool_t c_directed;
  SEXP graph = R_NilValue;
                                        /* Convert input */
  c_n=INTEGER(n)[0];
  c_directed=LOGICAL(directed)[0];
                                        /* Call igraph */
  c_graph = new igraph_t{};
  igraph_empty(c_graph, c_n, c_directed);

  graph = create_graph(c_graph);
  SET_CLASS(graph, ScalarString(CREATE_STRING_VECTOR("igraph2")));

  return graph;
}

SEXP R_igraph_vcount(SEXP graph) {
  igraph_t* c_graph = static_cast<igraph_t*>(R_ExternalPtrAddr(R_altrep_data1(graph)));
  igraph_integer_t c_result;
  SEXP r_result;

  c_result = igraph_vcount(c_graph);

  PROTECT(r_result=NEW_INTEGER(1));
  INTEGER(r_result)[0]=c_result;

  UNPROTECT(1);
  return(r_result);
}

SEXP R_igraph_vcount2(SEXP graph) {
                                        /* Declarations */
  igraph_t c_graph;
  igraph_integer_t c_result;
  SEXP r_result = R_NilValue;
                                        /* Convert input */
  R_SEXP_to_igraph(graph, &c_graph);
                                        /* Call igraph */
  c_result=  igraph_vcount(&c_graph);

                                        /* Convert output */

  PROTECT(r_result=NEW_INTEGER(1));
  INTEGER(r_result)[0]=c_result;

  UNPROTECT(1);
  return(r_result);
}

static const R_CallMethodDef CallEntries[] = {
    {"R_igraph2_warning", (DL_FUNC) &R_igraph2_warning, 0},
    {"R_igraph_empty", (DL_FUNC) &R_igraph_empty, 2},
    {"R_igraph_empty2", (DL_FUNC) &R_igraph_empty2, 2},
    {"R_igraph_finalizer", (DL_FUNC) &R_igraph_finalizer, 0},
    {"R_igraph_finalizer2", (DL_FUNC) &R_igraph_finalizer2, 0},
    {"R_igraph_vcount", (DL_FUNC) &R_igraph_vcount, 1},
    {"R_igraph_vcount2", (DL_FUNC) &R_igraph_vcount2, 1},
    {"R_igraph_create", (DL_FUNC) &R_igraph_create, 3},
    {"R_igraph_get_edgelist", (DL_FUNC) &R_igraph_get_edgelist, 2},

    {NULL, NULL, 0}
};

extern "C" void attribute_visible R_init_igraph2(DllInfo *dll) {
  register_graph(dll, "graph", "igraph2");
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}