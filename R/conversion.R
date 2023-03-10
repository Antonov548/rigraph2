as_edgelist <- function(graph, names = TRUE) {
  if (!is_igraph(graph)) {
    stop("Not a graph object")
  }
  on.exit(.Call(C_R_igraph_finalizer))
  res <- matrix(.Call(C_R_igraph_get_edgelist, graph, TRUE),
    ncol = 2
  )
  res <- res + 1

  # Attributes currently not available
  # if (names && "name" %in% vertex_attr_names(graph)) {
  #   res <- matrix(V(graph)$name[res], ncol = 2)
  # }

  res
}