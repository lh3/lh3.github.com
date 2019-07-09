---
layout: post
title: "On a reference pan-genome model"
description: ""
category: 
tags: []
---
{% include JB/setup %}

In the last weekend, I made [gfatools][gfatools] and [minigraph][minigraph]
open to the public. Both repos come with some documentations, but they haven't
explained the background and motivation behind. This blog post gives a more
complete picture.

The primary assembly of GRCh38, our current human reference genome, is largely
the concatenation of individual haplotype segments. It aims to model a single
human genome and lacks thousands of structural variations (SVs). These SVs are
causing a multitude of problems which have been documented in many papers. A
solution is to construct a pan-genome reference. The question is "how?".

My answer to that is [rGFA][rgfa]. rGFA is a text format and more importantly a
data model. It introduces the concept of *stable coordiate*, which is
persistent under the sequence split and insertion operations. We can
incrementally "add" a new genome to a graph without breaking the old coordinate
system. At the same time, if we start with a linear reference genome, an
augmented graph naturally inherits the coordinate system from the linear
reference. We can have the benefits of both linear and graphical
representations.

[Minigraph][minigraph] proves the above is more than just an idea; it is
practically working at least to some extent (constructing a graph from 15 human assemblies in an hour). Along this line of work,
[GAF][gaf] is a first text format to describe sequence-to-graph mapping.
The sister repo [gfatools][gfatools] implements a few utilities to work with
rGFA. These are all connected by design.

There are much more to be done: minigraph has [many limitations][limit];
gfatools lacks important functionalies; GAF alone is inadequate and can't play
the same role as SAM; the starting linear reference genome has a lot of room
for improvement given the advances in sequencing technologies. With the same
data model, there can also be alternative approaches to graph construction
(e.g. via VCF, compact de Bruijn graph, mutliple-sequence alignment or all-pair
alignment). Minigraph is more of a *proof-of-concept* starting point. Community
efforts are the only way to build a pan-genome reference that is practical,
accurate, and comprehensive enough to represent genome diversity and ultimately
help us to understand genetics better.

[gfatools]: https://github.com/lh3/gfatools
[minigraph]: https://github.com/lh3/minigraph
[rgfa]: https://github.com/lh3/gfatools/blob/master/doc/rGFA.md
[gaf]: https://github.com/lh3/gfatools/blob/master/doc/rGFA.md#the-graph-alignment-format-gaf
[limit]: https://github.com/lh3/minigraph#limit
