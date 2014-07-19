---
layout: post
title: "A proposal of the Grapical Fragment Assembly format"
description: ""
category:
tags: []
---
{% include JB/setup %}

### Introduction

Almost three years ago, there was a lengthy discussion in the Assemblathon
mailing list about a generic format for fragment assemmbly. The end product is
[the FASTG format][fastg]. In the discussion, I have expressed several major
concerns with the format. The top one is that it is mathematically wrong. Three
years later, FASTG is still not widely used. At this point, [Adam
Phillippy][adamtalk] and [Pall Melsted][pmelsted] openly called for a generic
assembly format again. I also feel the pressing necessity of standardization, so
decided to give a try myself. This is the Graphical Fragment Assembly format, or
GFA in abbreviation.

In this post, I will start from the theoretical basis of assembly graph,
describe the format and finally discuss the potential issues with the proposal.

I showed an earlier version of this format to Richard Durbin, Daniel Zerbino and
Benedict Paten last night in Oxford. That version was a variant of FASTA. When I
was formalizing the format in this post, I found FASTA is too crowded and too
limited. Following the suggestion of Daniel, I finally adopted a format similar
to [ASQG][asqg] and the PSMC output.

### Theory

DNA sequence assembly is often (though not always) represented as a graph.
There are multiple types of graphs including de Bruijn graph, overlap graph,
unitig graph and string graph. They are all [birected graph][bigraph]. Briefly,
in this graph, each vertex is a sequence and each arc is an overlap. Because
DNA sequences have two strands, an arc may have four directions, representing
the four possible overlaps: forward-forward, forward-reverse, reverse-forward
and reverse-reverse. It should be noted that a k-mer de Bruijn graph is
equivalent to an overlap graph for k-mer reads with (k-1)-mer overlaps.
It is a bidirected graph, too.

The critical problem with FASTG is that it puts sequneces on arcs/edges. It is
unable to describe a simple topology such as `A->B; C->B; C->D` without adding a
dummy node, which breaks the theoretical elegance of assembly graphs. Due to the
historical confusion between vertices and edges, I will avoid using these
terminologies. I will use a *segment* for a piece of sequence and a *link* for a
connection between segments.

### The GFA format

Although we can describe an assembly graph with bidirected arcs, I find in
practice, it is easier and more explicit to describe links between the ends of
segments. [Gene Myers][gmyers] took a similar approach in his [string graph
paper][stringg]. Based on this observation, I *uniquely* label the 5'-end and
the 3'-end of each segment. The following shows an assembly graph with seven
segments in GFA:

    H  VN:Z:1.0
    S  1  2  CGATGCAA  *
    L  2  3  5M
    S  3  4  TGCAAAGTAC  *
    L  3  6  *
    S  5  6  TGCAACGTATAGACTTGTCAC  *  RC:i:4
    L  6  8  1M1D2M1S
    S  7  8  GCATATA  *
    L  7  9  *
    S  9 10  CGATGATA  *
    S 11 12  ATGA  *
    C  9 11  2  4M

If we name a segment with the two *ordered* integers, the example above is
equivalent to a bidirected graph `1:2>->3:4; 5:6>->3:4; 5:6>-<7:8<->9:10` with
`11:12` contained in `9:10`. The `H` line is the header. An `S` line describes a
segment which consists of 5'-end label, 3'-end label, sequence and
pseudo-quality. An `L` line represents a link which consists of the labels of
the two ends and a CIGAR that describes the overlap alignment taking the first
end as the target/upper sequence. The CIGAR can describe symmetric overlaps
(e.g. `5M`), assembly gaps (e.g. `10N`), gapped overlaps, open-end alignments
(e.g. `1M1D2M1S`; heading `S` for clipping on the second sequence and tailing
`S` on the first), or unaligned overlaps (e.g. `5S10I8D2S`; no `M` operators).
It is related to but different from the CIGAR used in SAM. A `C` line represents
a containment.

For all lines, additional information is described with tags in a format
identical to SAM. Predefined tags include:

    Line  Tag  Type  Meaing
    -----------------------------------------------------------------------
     H    VN    Z    Version number
     H    QT    A    Type of pseudo-quality. Valid values: `Q`, `D` or `K`
     S    RC    i    # reads assembled into the segment
    L/C   MQ    i    Mapping quality of the overlap/containment

### Discussions

1. If this format cannot encode your assembly, please let me know. Thank you.
Suggestions on making GFA work would be appreciated even more. :-)

2. It is unusual to uniquely label the two ends of a segment. [ABySS][abyss], [SGA][sga] and
most other assemblers uniquely label a segment. In my view, end-labeling has a
few advantages: a) it requires fewer operations for reverse-complementing and
unambiguous merging; b) by representing a bidirected arc with `A+,B-`, we are
still converting `A` to two labels; c) my own assembler only works with
end-labeling. I think it should always be easy to convert the segment-labeling
to the end-labeling but not vice versa. Unless there are strong arguments
against end-labeling, I will keep it.

3. Use a string to label an end. I like integers for efficiency, but don't
object to strings in principle.

4. I don't like the CIGAR I proposed. It is too complex. If you can find a
cleaner way to describe all kinds of overlaps and gaps, please let me know.
These complex overlaps are not uncommon in a long-read assembly or for
scaffolding.

5. In FASTG, we can encode a simple "bubble" with `ACGT[C,T]TAGT`. Although GFA
can describe this assembly, it needs to add three more segments and four links,
which are quite heavy. One option is to allow such simple bubbles on the `S`
line with a specific header tag indicating that the file contains small bubbles.
Is it a good idea? How many assemblers can take advantage of this potential
addition?

6. If we can agree on a format, I can write a parser and a few basic tools such
as flip, unambiguous merge and perhaps more complex operations such as tip
trimming and bubble popping if I have time.

7. Any other suggestions?

### Update

Considering to replace end-labeling with the more common segment-labeling. The
example above will look like (better or worse?):

    H  VN:Z:1.0
    S  1  CGATGCAA  *
    L  1  2  ++  5M
    S  2  TGCAAAGTAC  *
    L  3  2  ++  *
    S  3  TGCAACGTATAGACTTGTCAC  *  RC:i:4
    L  3  4  +-  1M1D2M1S
    S  4  GCATATA  *
    L  4  5  -+  *
    S  5  CGATGATA  *
    S  6  ATGA  *
    C  5  6  ++  2  4M

[fastg]: http://fastg.sourceforge.net
[adamtalk]: http://www.iscb.org/ismb-mm/media-ismb2014/talks
[pmelsted]: http://pmelsted.wordpress.com/2014/07/17/dear-assemblers-we-need-to-talk-together/
[bigraph]: http://en.wikipedia.org/wiki/Bidirected_graph
[asqg]: https://github.com/jts/sga/wiki/ASQG-Format
[stringg]: http://bioinformatics.oxfordjournals.org/content/21/suppl_2/ii79.abstract
[gmyers]: http://en.wikipedia.org/wiki/Eugene_Myers
[abyss]: http://www.bcgsc.ca/platform/bioinfo/software/abyss
[sga]: https://github.com/jts/sga
