---
layout: post
title: "BWA-MEM for long error-prone reads"
description: ""
category: 
tags: []
---
{% include JB/setup %}

A recent paper published by [Phil Ashton et al][1] has triggered some
discussions which subsequently moved to my domain: read mapping. Phil then
asked me to clarify how the upcoming bwa-mem works with Oxford Nanopore (ONP)
reads. Here we go.

Although the very first version of bwa-mem worked with PacBio reads (well, not
crashing), the alignment it produced was too fragmented to be useful. I
initially thought the long exact seeds used by bwa-mem would not be sensitive
enough to the ~15% error rate of PacBio reads, but [Homolog.us][homolog]
[pointed out][2] that [BLASR][blasr] is also using long exact seeds.
I then realized that it is also possible for the bwa-mem algorithm to work with
PacBio data. With more and more interesting PacBio data sets coming out, I
decided to give a try.

There are two major changes in BWA-MEM to support PacBio data better. Firstly,
we have to use a relaxed scoring matrix such that Smith-Waterman (SW) can give
a positive score on a valid match. In 0.7.9 and 0.7.10, the scoring scheme is:
match=2, mismatch=-5, gapOpen=-2 and gapExt=-1. Secondly, I added a heuristic
to filter intial seeds to reduce unsuccessful seed extensions. For PacBio
reads, bwa-mem performs SSE2-SW in a small window around each seeds and then
reject seeds if the SW score is too small (threshold proportional to option
`-W`). This is similar to the X-dropoff heuristic of BLAST. In addition to
these, bwa-mem also implements a gap patching heuristic whereby it tries to
connect two colinear local hits with a global alignment even if the resulting
alignment is not optimal. This heuristic helps to let alignment walk through
low-quality regions and thus reduce fragmentation. Homolog.us is right in that
bwa-mem could work well with PacBio reads.

ONT reads pose new challenges due to its higher error rate. The intial
one-direction (1D) reads have an error rate higher than 30%. The 2D reads is a
little better, but still has more errors than PacBio for now. The PacBio mode
does not work very well. This calls for improvements to bwa-mem. The
ONT-specific changes are relatively simple. Firstly, we use shorter seed
lengths and more relaxed threshold `-W` as a consequence of higher error rate.
Secondly, we modified the scoring matrix to match=1, mismatch=-1, gapOpen=-1
and gapExt=-1, based on a [recent paper][sc]. This setting turns out to be
better for PacBio, too.

The ONT mode of bwa-mem is largely comparable to [LAST][last], the mapper
recommended by several groups. Given the same scoring system, they generate
identical SW scores most of time. When scores are different, LAST tends to be the
winner - on this small fraction of alignments with different scores, bwa-mem is
more likely to miss low-quality hits or fails to extend a partial alignment to
the right place. For bacterial data, bwa-mem and LAST are also about the same
in speed. For human PacBio reads, though, bwa-mem is times faster. It is more
geared towards human data.

LAST is probably the only mapper that works efficiently and accurately with
query sequences ranged from 100bp to 100Mbp without much parameter tuning. This
is very impressive. As of now, bwa-mem does not work well for queries longer
than ~10Mbp.

To use the ONT mode of bwa-mem, one needs to acquire a recent bwa [from
github][bwa] and run it with `bwa mem -x ont2d ref.fa reads.fq`. Its official
release via 0.7.11 is coming soon.


[1]: http://www.nature.com/nbt/journal/vaop/ncurrent/full/nbt.3103.html
[2]: http://www.homolog.us/blogs/blog/2013/10/28/bwa-mem-good-blasr-aligning-pacbio-reads-part-2/
[homolog]: http://www.homolog.us/blogs/
[blasr]: https://github.com/PacificBiosciences/blasr
[last]: http://last.cbrc.jp
[sc]: http://www.ncbi.nlm.nih.gov/pmc/articles/PMC4226419/
[bwa]: https://github.com/lh3/bwa
