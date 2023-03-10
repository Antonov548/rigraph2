make_graph <- function(edges, ..., n = max(edges), directed = TRUE) {
  res <- .Call(
          C_R_igraph_create, as.integer(edges- 1), as.integer(n),
          as.logical(directed)
        )

  res
}

make_empty_graph <- function(n=0, directed=TRUE) {
  # Argument checks
  if (is.null(n)) {
    stop("number of vertices must be an integer")
  }

  n <- suppressWarnings(as.integer(n))
  if (is.na(n)) {
    stop("number of vertices must be an integer")
  }

  directed <- as.logical(directed)
  
  on.exit( .Call(`C_R_igraph_finalizer`) )
  # Function call
  res <- .Call(`C_R_igraph_empty`, n, directed)

  res
}

make_empty_graph_old <- function(n=0, directed=TRUE) {
  # Argument checks
  if (is.null(n)) {
    stop("number of vertices must be an integer")
  }

  n <- suppressWarnings(as.integer(n))
  if (is.na(n)) {
    stop("number of vertices must be an integer")
  }

  directed <- as.logical(directed)
  
  on.exit( .Call(`C_R_igraph_finalizer2`) )
  # Function call
  res <- .Call(`C_R_igraph_empty2`, n, directed)

  res
}