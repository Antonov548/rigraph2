test_that("test empty graph", {
  igraph2_warning()

  g <- make_empty_graph(n = 5)

  if (!is_igraph(g)) {
    stop('Not a graph object')
  }

  print.header(g)

})

test_that("test empty graph old", {
  igraph2_warning()

  g <- make_empty_graph_old(n = 5)

  if (!is_igraph(g)) {
    stop('Not a graph object')
  }

  print.header_old(g)

})

test_that("test graph with edges old", {
  igraph2_warning()

  g <- make_graph(c(1, 2, 2, 3, 3, 4, 5, 6), directed = FALSE)

  if (!is_igraph(g)) {
    stop('Not a graph object')
  }

  m <- as_edgelist(g)
  print(m)

  print.header_old(g)

})
