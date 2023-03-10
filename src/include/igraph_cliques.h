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

#ifndef IGRAPH_CLIQUES_H
#define IGRAPH_CLIQUES_H

#include "igraph_decls.h"
#include "igraph_error.h"
#include "igraph_types.h"
#include "igraph_datatype.h"
#include "igraph_vector_list.h"

__BEGIN_DECLS

/* -------------------------------------------------- */
/* Cliques, maximal independent vertex sets           */
/* -------------------------------------------------- */

IGRAPH_EXPORT igraph_error_t igraph_maximal_cliques(
   const igraph_t *graph, igraph_vector_int_list_t *res,
   igraph_integer_t min_size, igraph_integer_t max_size
);
IGRAPH_EXPORT igraph_error_t igraph_maximal_cliques_file(const igraph_t *graph,
                                              FILE *outfile,
                                              igraph_integer_t min_size,
                                              igraph_integer_t max_size);
IGRAPH_EXPORT igraph_error_t igraph_maximal_cliques_count(const igraph_t *graph,
                                               igraph_integer_t *res,
                                               igraph_integer_t min_size,
                                               igraph_integer_t max_size);
IGRAPH_EXPORT igraph_error_t igraph_maximal_cliques_subset(
   const igraph_t *graph, const igraph_vector_int_t *subset,
   igraph_vector_int_list_t *res, igraph_integer_t *no,
   FILE *outfile, igraph_integer_t min_size, igraph_integer_t max_size
);
IGRAPH_EXPORT igraph_error_t igraph_maximal_cliques_hist(const igraph_t *graph,
                                              igraph_vector_t *hist,
                                              igraph_integer_t min_size,
                                              igraph_integer_t max_size);

IGRAPH_EXPORT igraph_error_t igraph_cliques(const igraph_t *graph, igraph_vector_int_list_t *res,
                                 igraph_integer_t min_size, igraph_integer_t max_size);
IGRAPH_EXPORT igraph_error_t igraph_clique_size_hist(const igraph_t *graph, igraph_vector_t *hist,
                                          igraph_integer_t min_size, igraph_integer_t max_size);
IGRAPH_EXPORT igraph_error_t igraph_largest_cliques(const igraph_t *graph,
                                         igraph_vector_int_list_t *cliques);
IGRAPH_EXPORT igraph_error_t igraph_clique_number(const igraph_t *graph, igraph_integer_t *no);
IGRAPH_EXPORT igraph_error_t igraph_weighted_cliques(const igraph_t *graph,
                                          const igraph_vector_t *vertex_weights, igraph_vector_int_list_t *res,
                                          igraph_real_t min_weight, igraph_real_t max_weight, igraph_bool_t maximal);
IGRAPH_EXPORT igraph_error_t igraph_largest_weighted_cliques(const igraph_t *graph,
                                                  const igraph_vector_t *vertex_weights, igraph_vector_int_list_t *res);
IGRAPH_EXPORT igraph_error_t igraph_weighted_clique_number(const igraph_t *graph,
                                                const igraph_vector_t *vertex_weights, igraph_real_t *res);
IGRAPH_EXPORT igraph_error_t igraph_independent_vertex_sets(const igraph_t *graph,
                                                 igraph_vector_int_list_t *res,
                                                 igraph_integer_t min_size,
                                                 igraph_integer_t max_size);
IGRAPH_EXPORT igraph_error_t igraph_largest_independent_vertex_sets(const igraph_t *graph,
                                                         igraph_vector_int_list_t *res);
IGRAPH_EXPORT igraph_error_t igraph_maximal_independent_vertex_sets(const igraph_t *graph,
                                                         igraph_vector_int_list_t *res);
IGRAPH_EXPORT igraph_error_t igraph_independence_number(const igraph_t *graph, igraph_integer_t *no);

/**
 * \typedef igraph_clique_handler_t
 * \brief Type of clique handler functions.
 *
 * Callback type, called when a clique was found.
 *
 * See the details at the documentation of \ref
 * igraph_cliques_callback().
 *
 * \param clique The current clique. The clique is owned by the clique search
 *   routine. You do not need to destroy or free it if you do not want to store
 *   it; however, if you want to hold on to it for a longer period of time, you
 *   need to make a copy of it on your own and store the copy itself.
 * \param arg This extra argument was passed to \ref
 *   igraph_cliques_callback() when it was called.
 * \return Error code; \c IGRAPH_SUCCESS to continue the search or
 *   \c IGRAPH_STOP to stop the search without signaling an error.
 */
typedef igraph_error_t igraph_clique_handler_t(const igraph_vector_int_t *clique, void *arg);

IGRAPH_EXPORT igraph_error_t igraph_cliques_callback(const igraph_t *graph,
                                          igraph_integer_t min_size, igraph_integer_t max_size,
                                          igraph_clique_handler_t *cliquehandler_fn, void *arg);

IGRAPH_EXPORT igraph_error_t igraph_maximal_cliques_callback(const igraph_t *graph,
                                                  igraph_clique_handler_t *cliquehandler_fn, void *arg,
                                                  igraph_integer_t min_size, igraph_integer_t max_size);


__END_DECLS

#endif
