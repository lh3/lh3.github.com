---
layout: post
title: "On the graphical representation of sequences"
description: ""
category: 
tags: []
---
{% include JB/setup %}

### Introduction

Ever since the advent of the so-called Next-Generation Sequencing (NGS), we have
been thinking about encoding all the population variations in a graph. That
was 2008. Now, six years later, the rapidly growing number of sequenced human
individuals continueously presses for the necessity of a graphical
representation of the existing sequences, which leads to many publications in
this direction, both in biology and in computer science.

The graph described above is a *population graph*. It captures variations
between *many* individuals/strains. A typical use of the graph is to
map the sequences of a new individual to the existing variations, in particular
large variations. Another different type of sequence graph is *assembly graph*.
It represents ambiguity in the assembly of a *single* individual. It aims, in
my view, to enable a modular approach to the development of assembly related
algorithms. Population graph and assembly graph serve different purposes. Of
course, there is not a clear boundary between the two types. The assembly of a
poly-ploid individual encodes variations between several haplotypes. We can
also assemble sequence reads from many individuals. The resulting graph is both
an assembly graph and population graph. I usually classify such a graph as a
population graph as its primary goal is to encode variations but not to derive
long contigs.

As a side note, I make a distinction between a population graph and an assembly
graph partly due to the suggestion of taking mapping as a killer application for
my proposed [GFA format][gfa]. From my point of view, GFA is designed to be a
lightweight assembly format but not for encoding variations from a large number
of individuals. I will come back to this topic later.

### Population graphs

There are two subclasses of population graphs depending on whether they rely on
a reference-guided multi-alignment.

#### Graphs derived from multi-alignment

A graph derived from a multi-alignment or similarly a VCF, inherits the
reference coordinates and annotations. It is closer to our current practice and
easier to understand. It is the more popular type of graph in the literature.
To construct a multi-alignment, we need the exact coordinates on the reference
genome. This poses several problems. Firstly, for the same set of sequences,
there can be more than one plausible alignments. I gave the following example
to [GA4GH][ga4gh]:


    Ref:    AAGCTA--CTAG----CT                 AAGCTA------CTAGCT
    Allele: AAGCTAGACTAGGAAGCT       or        AAGCTAGACTAGGAAGCT
	    (2 gap opens, 0 mismatch)          (1 gap open, 2 mismatches)

In the two alignments, the reference sequence and the allele sequence are
exactly the same. However, in an affine gap penalty model, both alignments can
be optimal depending on the scoring system (this is very common in protein
sequence alignment). From this example, we can see that alignment can be
ambiguous and thus the resulting graph can be ambiguous, too, which is not a
nice feature. This is worse when the alignment is constructed from VCF when the
actual allele sequence may be broken down to small variants. When we use a
canonical VCF to represent the two alignments, the skeletons will look like:

    ref 6  A AGA                               ref 6 A AGACTAG
	ref 10 G GGAAG                  or         ref 7 C G
	                                           ref 8 T A

Without the haplotype information, we will not know the two VCFs are derived
from the same allele. This also leads to the second question: when to break
haplotype structure. On one hand, it is costly and unnecessary to keep
arbitrarily long haplotypes. On the other hand, if we always go for minimal
variations, we will have the problem above, and additionally, create new
haplotypes not seen in data given that multi-nucleotide changes may be arising
from a different mechanism ([Harris and Nielsen, 2014][multimut]).

Thirdly and more importantly, multi-alignment does not capture structural
variations, in particular novel insertions missing from the reference genome,
and is sensitive to the errors and the version of the reference genome.

#### Graphs derived from assembly or context matching

Alternatively, a graph can be constructed from a set of sequences without
typical multi-alignment. Such a graph is immune to all the problems caused by
multi-alignment, but has the difficulty in incorporating the rich information
in the reference genome and annotation.

Personally, I more like graphs independent of alignments. These seem cleaner
theoretically. Nonetheless, from a practical point of view, alignment-based
graphs may be more useful in the short term.

#### Review of related works in computer science

To the best of my knowledge, [Siren et al (2008)][siren1] was the first attempt
to encode multiple genomes. The basic idea is very simple: to keep individual
genomes in a run-length encoded self-index, such as [CSA][csa] or [FM-index][fm].
The authors further polished the idea, added theoretical analyses and finally
published in a journal ([Makinen et al, 2010][veli1]). Later [Kreft and Navarro
(2012)][lzall] and [Ferrada et al (2012)][lzall2] proposed to use a variation
of [LZ77][lz77], the key algorithm behind GIF, PNG and zlib, to achieve a
similar goal. [Do et al (2014)][do] also use LZ77, but approach the problem
from a different angle. It uses the reference sequence to decompose individual
sequences, an idea originally developed for relative compression ([Kuruppu et
al, 2010][kuruppu1]). These works do not require a multi-alignment between
sequences or a linear alignment against a reference genome. The following do.

[Huang et al (2010)][huang1] assumed individual genomes are sharing long blocks
of identical sequences and indexed the differences. They devised a data
structure based on FM-index to enable fast pattern matching while retaining the
sample information. [Na et al (2013)][na] solved a similar problem with suffix
array of alignment. A concern with these two papers is that given many genomes,
shared blocks across all genomes may be quite short, which may hurt the
performance.  [Alatabbi et al (2012)][alatabbi] attempted to address this issue
with a multi-level q-gram.  The role of reference becomes explicit. A few
others ([Wandelt and Leser][wandelt]; [Barton et al, 2013][carl]; [Yang et al,
2013][yang]) followed a similar hashtable-based approach. 

Another class of methods, which even predate NGS, encode multi-alignment as a
grammar ([Claude and Navarro, 2011][grammar3]; [Abeliuk and Navarro,
2012][grammar1]; [Gagie et al, 2012][grammar2]). For example, we can encode
a biallelic indel as regular grammar "ACGT(AGTC|AT)AT". These methods are
frequently built upon other self-indices such as FM-index, LZ and suffix tree. 
[Siren et al (2010)][siren2] (and [a recent update][siren3]) proposed to
convert a multi-alignment to a directed acyclic graph (DAG) and index the graph
with an extended CSA.  It is similar to grammar-based indexing in that it
discards the sample information.

It should be noted that although there are many works on mapping against a
collection of related genomes, only a few provide an implementation and even
fewer provide an implementation practical for many human genomes. In contrast,
works in computational biology focus more on the practical aspect.

#### Review of related works in computational biology 

[Schneeberger et al (2009)][graph1] is a first effort to explicitly model
variations in a population DAG. Similar to [Huang et al (2010)][huang1],
it breaks the multi-alignment into entirely conserved regions and polymorphic
regions, but different from the previous work which directly keeps differential
sequences, it encodes these sequences in a graph and maps reads against the
graph. While Schneeberger et al provided a solution for a few *A. thaliana*
genomes, [Huang et al (2013)][graph2] provides the first practical
implementation, BWBBLE, for many human genomes. This paper encodes SNPs with
[IUB codes][iub] and INDELs as separate contigs. For a query with SNPs, exact
mapping can be done by searching multiple SA intervals compatible with the
query (e.g. TAAG and YAAK at different positions are both compatible with
TAAG). However, mapping for a query with INDELs, which is harder and of more
interest to me, seems unexplained. The different treatment of SNPs and INDELs
seems a little inconsistent. [Rahn et al (2014)][graph3] introduced Journaled
String Tree (JST) to consistently represent both SNPs and INDELs as edits to
the referene genome.  Different from the previous works, Rahn et al does not
index the graph. Instead, read mapping is achieved by traversing JST. This
strategy is similar to early Eland and MAQ. [Dilthey et al (2014)][graph4], in
my view, is an improvement to Schneeberger et al (2009). It is featured with
cleaner and more extensible graph construction. The connection to de Bruijn
graph is particularly interesting. I actually think we may simplify graph
construction further.

These works all require a multi-alignment as input. Worrying about the
instability of multi-alignment, [Paten et al (2014)][graph5] sought a very
different solution. They proposed context mapping to relate genomes. [Marcus et
al (2014)][graph6] use a graph of MEMs to describe the relationship between
genomes, though it is not for the purpose of mapping.

Another distinct approach to the construction of population graph is to
use assembly graph as a population graph ([Iqbal et al, 2012][graph6]).

### Assembly graphs and the GFA format

An assembly graph represents assembly ambiguities primarily caused by limited
read length. With PacBio reads, we can assemble most bacteria, model organisms
and humans into megabase-long contigs. For such contigs, we can do analyses
without considering the remaining ambiguity in the graph. Furthermore, we frequently
have other types of data such as physical maps, genetic maps, optical mapping
and Hi-C long-range information to resolve ambiguity. The graph aspect of
assembly is even less important.

Then why do we need assembly graphs? [Pall Melsted gave][pmelsted] the answer:
we need a common graph format to enable a modular approach to the development
of assembly algorithms. With a common format, we may be able to write a
scaffolder utilizing the existing relationship between contigs; with a common
format, we may have a generic module for graph simplification which is not so
trivial to implement; with a common format, we may take the best part from each
assembler to get better results; with a common format, we will be able to
develop a small component without writing a new assembler from scratch.
These will accelerate the development of assembly algorithms.

Take the SAM format as an analogy. Before SAM was widely adopted, there were
few generic tools; many mapper deverlopers had to write variant callers because
without variant calling, mapping itself is not of much use. After the adoption
of the SAM format, developers are able to focus on the part they are best at.
We end up with better mappers, better callers and more little tools. If we had
reached a consensus on the file format three years ago in the Assemblathon
mailing list, the success of SAM might have reoccurred. It is actually a little
late now, but there may still remain some benefits of having a common format.
That was why I proposed GFA.

GFA is not limited to an assembly format. It can represent arbitrary
relationship between sequences and is thus suitable for a population graph
format in theory. However, I would not take applications on population graphs,
such as graph mapping, as a killer application of GFA. There are many open
questions on population graphs. I cannot design a format for unsolved
questions. GFA aims to be an assembly format only, at least for now.

### Concluding remarks

Before I wrote this blog post, I had thought of a short article explaining the
difference between an assembly graph and a population graph. However, after I
started, many questions came into my mind: what is a population graph? what is
the use of it? how is it constructed? what do we expect to get from a
population graph? if we care about mapping, what output do we prefer? how could
we encode rearrangement and novel insertions? what is the current progress? and
how far are we from a practical solution? These questions motivated me to read
related works, which resulted in this extremely lengthy blog post, but in the
end, the answers to many of these questions remain unclear to me. More thoughts
needed...

(A final word. I kept arguing with myself when I wrote this blog post. Although
I have spent a lot of time, I have not paid enough attention to the wording,
clarity and the logic flow of the article. It may be hard to read. I
apologize.)

[gfa]: http://lh3.github.io/2014/07/19/a-proposal-of-the-grapical-fragment-assembly-format/
[sam]: http://samtools.sourceforge.net
[ga4gh]: http://genomicsandhealth.org
[siren1]: http://www.dcc.uchile.cl/~gnavarro/ps/spire08.3.pdf
[veli1]: http://www.ncbi.nlm.nih.gov/pubmed/20377446
[multimut]: http://arxiv.org/abs/1312.1395
[csa]: http://en.wikipedia.org/wiki/Compressed_suffix_array
[fm]: http://en.wikipedia.org/wiki/FM-index
[huang1]: http://link.springer.com/chapter/10.1007%2F978-3-642-14355-7_19
[do]: http://www.sciencedirect.com/science/article/pii/S0304397513005409
[alatabbi]: http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=6470220
[carl]: http://www-igm.univ-mlv.fr/~lecroq/articles/icibm2012.pdf
[na]: http://www-igm.univ-mlv.fr/~lecroq/articles/spire2013.pdf
[yang]: http://dl.acm.org/citation.cfm?id=2511216
[wandelt]: http://www.informatik.hu-berlin.de/forschung/gebiete/wbi/research/publications/2012/refcomprsearch.pdf
[lzall]: http://www.dcc.uchile.cl/~gnavarro/ps/tcs12.pdf
[lzall2]: http://arxiv.org/abs/1306.4037
[lz77]: http://en.wikipedia.org/wiki/LZ77_and_LZ78
[kuruppu1]: http://link.springer.com/chapter/10.1007%2F978-3-642-16321-0_20
[grammar1]: http://www.dcc.uchile.cl/~gnavarro/ps/spire12.2.pdf
[grammar2]: http://arxiv.org/pdf/1109.3954v6.pdf
[grammar3]: http://arxiv.org/pdf/1110.4493.pdf
[siren2]: http://arxiv.org/abs/1010.2656
[siren3]: http://www.cs.helsinki.fi/u/jltsiren/papers/Siren2014.pdf
[graph1]: http://www.ncbi.nlm.nih.gov/pubmed/19761611
[graph2]: http://www.ncbi.nlm.nih.gov/pubmed/23813006
[iub]: http://en.wikipedia.org/wiki/Nucleic_acid_notation
[graph3]: http://www.ncbi.nlm.nih.gov/pubmed/25028723
[graph4]: http://biorxiv.org/content/early/2014/07/08/006973
[graph5]: http://arxiv.org/abs/1404.5010
[graph6]: http://biorxiv.org/content/early/2014/04/06/003954
[graph7]: http://www.ncbi.nlm.nih.gov/pubmed/22231483
[pmelsted]: http://pmelsted.wordpress.com/2014/07/17/dear-assemblers-we-need-to-talk-together/
