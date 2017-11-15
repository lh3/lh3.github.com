---
layout: post
mathjax: true
title: "Immature thoughts on assembly De Bruijn graphs"
description: ""
category: 
tags: []
---
{% include JB/setup %}

By [mathematical definition][dbg], a *k*-order (or *k*-dimensional) De Bruijn
graph, or ${\rm DBG}(k)$ in brief, over the DNA alphabet uses *k*-mers at vertices. It
has $4^k$ vertices and $4^{k+1}$ edges. DBG(k) has two interesting properties.
First, DBG(k) is the [line graph][line-graph] of DBG(k-1). Intuitively, this
means an edge in DBG(k-1) uniquely corresponds to a vertex in DBG(k) and that
the edge adjacency of DBG(k-1) is precisely modeled by vertex adjacency of
DBG(k). Second, DBG(k) is both [Eulerian][euler] and [Hamiltonian][hamilton].
A Eulerian path in DBG(k-1) corresponds to a Hamiltonian path in DBG(k).

Given a set of sequences *S*, let $S(k)$ be the set of k-mers present in
sequences in *S*. We can [vertex-induce][induce] a subgraph from DBG(k) by
keeping vertices in S(k) together with edges connecting vertices in S(k). We
denote this graph by DBGv(k|S).  DBGv(k|S) can be regarded as an overlap graph
consisting of k-mers at vertices with (k-1)-mers overlaps.

Alternatively, we can edge-induce a subgraph by keeping edges in $S(k+1)$
together with [incident][incident] vertices. We denote this graph by DBGe(k|S).
DBGe(k|S) cannot be considerd as an overlap graph because there may be no
edges between two k-mers even if they have a (k-1)-mer overlap. To this end,
DBGe(k|S) is a subgraph of DBGv(k|S).

DBGv(k|S) is the line graph of DBGe(k-1|S). This property has an important
implication in implementation. One common way to store a DBG is to keep a
collection of *k*-mers. We traverse the graph by shifting a *k*-mer and probing
its presence/absence in the collection. Such an algorithm actually implements
both DBGv(k|S) and DBGe(k-1|S) at the same time.

In summary, the "De Bruijn graph" in "De Bruijn graph based assembler" is not
the De Bruijn graph by mathematical definition. Assembly De Bruijn graphs are
subgraphs. There are two different ways to induce such subgraphs, but in
implementation, they often behave the same. In DBG, are sequences on vertices
or on edges? The correct answer is: depending on how you look at the graph.

[dbg]: https://en.wikipedia.org/wiki/De_Bruijn_graph
[line-graph]: https://en.wikipedia.org/wiki/Line_graph
[euler]: https://en.wikipedia.org/wiki/Eulerian_path
[hamilton]: https://en.wikipedia.org/wiki/Hamiltonian_path
[incident]: https://en.wikipedia.org/wiki/Incidence_(graph)
[induce]: https://en.wikipedia.org/wiki/Induced_subgraph
