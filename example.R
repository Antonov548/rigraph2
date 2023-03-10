library(igraph2)

v <- c(1, 2, 3)

igraph2_warning()

g <- make_graph(c(10, 2, 2, 3, 3, 4, 5, 6), directed = FALSE)

if (!is_igraph(g)) {
  stop('Not a graph object')
}

m <- as_edgelist(g)
print(m)

print.header_old(g)