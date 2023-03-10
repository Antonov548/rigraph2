/* -*- mode: C -*-  */
/*
   IGraph library.
   Copyright (C) 2009-2012  Gabor Csardi <csardi.gabor@gmail.com>
   334 Harvard street, Cambridge, MA 02139 USA

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc.,  51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA

*/

#ifndef IGRAPH_FOREIGN_H
#define IGRAPH_FOREIGN_H

#include "igraph_decls.h"
#include "igraph_constants.h"
#include "igraph_datatype.h"
#include "igraph_error.h"
#include "igraph_types.h"
#include "igraph_strvector.h"

#include <stdio.h>

__BEGIN_DECLS

/* -------------------------------------------------- */
/* Read and write foreign formats                     */
/* -------------------------------------------------- */

IGRAPH_EXPORT igraph_error_t igraph_read_graph_edgelist(igraph_t *graph, FILE *instream,
                                             igraph_integer_t n, igraph_bool_t directed);
IGRAPH_EXPORT igraph_error_t igraph_read_graph_ncol(igraph_t *graph, FILE *instream,
                                         const igraph_strvector_t *predefnames, igraph_bool_t names,
                                         igraph_add_weights_t weights, igraph_bool_t directed);
IGRAPH_EXPORT igraph_error_t igraph_read_graph_lgl(igraph_t *graph, FILE *instream,
                                        igraph_bool_t names, igraph_add_weights_t weights,
                                        igraph_bool_t directed);
IGRAPH_EXPORT igraph_error_t igraph_read_graph_pajek(igraph_t *graph, FILE *instream);
IGRAPH_EXPORT igraph_error_t igraph_read_graph_graphml(igraph_t *graph, FILE *instream,
                                            igraph_integer_t index);
IGRAPH_EXPORT IGRAPH_DEPRECATED igraph_error_t igraph_read_graph_dimacs(igraph_t *graph, FILE *instream,
                                           igraph_strvector_t *problem,
                                           igraph_vector_int_t *label,
                                           igraph_integer_t *source,
                                           igraph_integer_t *target,
                                           igraph_vector_t *capacity,
                                           igraph_bool_t directed);
IGRAPH_EXPORT igraph_error_t igraph_read_graph_dimacs_flow(igraph_t *graph, FILE *instream,
                                           igraph_strvector_t *problem,
                                           igraph_vector_int_t *label,
                                           igraph_integer_t *source,
                                           igraph_integer_t *target,
                                           igraph_vector_t *capacity,
                                           igraph_bool_t directed);
IGRAPH_EXPORT igraph_error_t igraph_read_graph_graphdb(igraph_t *graph, FILE *instream,
                                            igraph_bool_t directed);
IGRAPH_EXPORT igraph_error_t igraph_read_graph_gml(igraph_t *graph, FILE *instream);
IGRAPH_EXPORT igraph_error_t igraph_read_graph_dl(igraph_t *graph, FILE *instream,
                                       igraph_bool_t directed);

typedef unsigned int igraph_write_gml_sw_t;

enum {
    IGRAPH_WRITE_GML_DEFAULT_SW          = 0x0, /* default settings */
    IGRAPH_WRITE_GML_ENCODE_ONLY_QUOT_SW = 0x1  /* only encode " characters, nothing else */
};

IGRAPH_EXPORT igraph_error_t igraph_write_graph_edgelist(const igraph_t *graph, FILE *outstream);
IGRAPH_EXPORT igraph_error_t igraph_write_graph_ncol(const igraph_t *graph, FILE *outstream,
                                          const char *names, const char *weights);
IGRAPH_EXPORT igraph_error_t igraph_write_graph_lgl(const igraph_t *graph, FILE *outstream,
                                         const char *names, const char *weights,
                                         igraph_bool_t isolates);
IGRAPH_EXPORT igraph_error_t igraph_write_graph_graphml(const igraph_t *graph, FILE *outstream,
                                             igraph_bool_t prefixattr);
IGRAPH_EXPORT igraph_error_t igraph_write_graph_pajek(const igraph_t *graph, FILE *outstream);
IGRAPH_EXPORT IGRAPH_DEPRECATED igraph_error_t igraph_write_graph_dimacs(const igraph_t *graph, FILE *outstream,
                                            igraph_integer_t source, igraph_integer_t target,
                                            const igraph_vector_t *capacity);
IGRAPH_EXPORT igraph_error_t igraph_write_graph_dimacs_flow(const igraph_t *graph, FILE *outstream,
                                            igraph_integer_t source, igraph_integer_t target,
                                            const igraph_vector_t *capacity);
IGRAPH_EXPORT igraph_error_t igraph_write_graph_gml(const igraph_t *graph, FILE *outstream,
                                                    igraph_write_gml_sw_t options,
                                                    const igraph_vector_t *id, const char *creator);
IGRAPH_EXPORT igraph_error_t igraph_write_graph_dot(const igraph_t *graph, FILE *outstream);
IGRAPH_EXPORT igraph_error_t igraph_write_graph_leda(const igraph_t *graph, FILE *outstream,
                                          const char* vertex_attr_name, const char* edge_attr_name);

/* -------------------------------------------------- */
/* Convenience functions for temporary locale setting */
/* -------------------------------------------------- */

typedef struct igraph_safelocale_s *igraph_safelocale_t;

IGRAPH_EXPORT igraph_error_t igraph_enter_safelocale(igraph_safelocale_t *loc);
IGRAPH_EXPORT void  igraph_exit_safelocale(igraph_safelocale_t *loc);

__END_DECLS

#endif
