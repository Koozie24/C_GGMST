## GGMST Definition and Purpose

The generalized minimum spanning tree problem (GMST) is a generalization of the well-known
Minimum Spanning Tree (MST) problem. An instance of the GMST is given by an undirected
graph G = (V, E) where the vertex set V is partitioned into k clusters Vi, i = 1, …, k,
and a weight w(e) ∈ R+ is assigned to every edge e ∈ E. The goal is to find a minimum-
weight tree that includes exactly one vertex from each cluster.

The GMST occurs in telecommunications network planning, where a network of node clusters
needs to be connected via a tree architecture using exactly one node per cluster. More
precisely, local subnetworks must be interconnected by a global network. (Bhattacharya, 1).
