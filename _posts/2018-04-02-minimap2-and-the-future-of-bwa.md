---
layout: post
title: "Minimap2 and the future of BWA"
description: ""
category: 
tags: []
---
{% include JB/setup %}

My minimap2 paper has been accepted for publication in Bioinformatics. You can
find the latest LaTeX source [at OverLeaf][overleaf] or in the [tex
directory of minimap2][tex]. I am intentionally delaying the publication
process for personal reasons. It will take a while for you to see the published
version at Bioinformatics. I thought to write this blog post when the paper
comes out, but there have been a few discussions on minimap2 recently, so I
decide to write it now.

### Why minimap2?

I wrote a [blog post][bwamem-long] on long-read alignment with bwa-mem several
years ago. In short, bwa-mem was not designed for long reads initially. It
works, but not well. When I was developing [minimap][minimap1] for read
overlapping, I realized approximate mapping could achieve comparable accuracy
to bwa-mem at a much faster speed. I didn't expand minimap to a full-pledge
aligner because (1) I knew base-level alignment was going to be very slow and
(2) bwa-mem still worked fine. However, both reasons became invalid in the
coming years.

In early 2017, Nick Loman et al invented a [new protocol][ultra-long] to
sequence nanopore reads of 100kb in length. Bwa-mem failed miserably on such
ultra-long reads -- it was not "fine" at all. In addition, not long after I
published minimap, Suzuki and Kasahara released [minialign][minialign]. It
implements a banded base-level alignment algorithm that is practical for
long-read alignment and much faster than the alternatives. These events finally
motivated me to develop minimap2.

### The status of minimap2

For long reads, [minimap2][mm2] is a much better mapper than [bwa-mem][bwa] in almost every
aspect: it is >50X faster, more accurate, gives better alignment at long
gaps and works with ultra-long reads that fail bwa-mem. Minimap2 also goes
beyond a typical long-read mapper. It can achieve good full-genome alignment
(see the minimap2 paper, section 3.4) and is used by [QUAST-LG][quast-lg].
Minimap2 can also align high-quality cDNAs and noisy long RNA-seq reads
(section 3.2). PacBio has [started to consider][tofu-u2] minimap2 in their
Iso-seq pipeline. The feature set and the code base of minimap2 are also fairly
stable. **I see little reason to use bwa-mem for long reads in future**.

The story on short-read alignment is a little complex, though. I did plan to
replace bwa-mem with minimap2 on short-read alignment, too. In the minimap2
paper, I showed that minimap2 is 3X as fast as bwa-mem and achieves comparable
accuracy to bwa-mem on short variant calling (section 3.3). In the final round
of the review, an reviewer still argued that minimap2 wouldn't work well for
short reads. I didn't think so at the time given that Illumina Inc. has
independently evaluated minimap2 and observed that minimap2 is highly
competitive. Therefore, I didn't follow the suggestion of that reviewer.

However, [Andrew Carroll][andrew] at DNAnexus has recently showed me that
minimap2 was slower than bwa-mem on two NovaSeq runs at his hand. Part of the
reason, I guess, is that the two NovaSeq runs have a little higher error rate,
which triggers expensive heuristics in minimap2 more frequently. Furthermore, I
also realize that bwa-mem will be better than minimap2 at Hi-C alignment
because bwa-mem is more sensitive to short matches. In the end, I admit
**minimap2 is not ready to replace bwa-mem all around**. I owe that reviewer an
apology.

Generally, I still think minimap2 is a competitive short-read mapper and I will
use it often in my research projects. However, given that the performance of
minimap2 is not as consistent as bwa-mem for short reads of varying quality,
bwa-mem is still better for production uses, at least before I find a way to
improve minimap2.

### The future of bwa

**Bwa will stay**.
I am thinking to bring some minimap2 features to bwa-mem, such as fast
alignment extension and global alignment. This will make code cleaner and fix a
long-existing bug in bwa-mem: a tiny fraction of base-level alignment is
suboptimal. Nonetheless, implementing these features will not speed up bwa-mem
much because base-level alignment is not the computation bottleneck for short
reads. I am also likely to remove the bwa-sw algorithm and issue a deprecation
warning when the "pacbio" or the "ont2d" presets are used. In the mean time,
several talented developers at Intel Inc. are restructuring bwa-mem for
considerable performance boost at no loss of accuracy. I will work with them.
If this effort hopefully works out, the end product will become bwa-mem2. All
these won't happen soon, unfortunately.



[overleaf]: http://www.overleaf.com/read/ddwtrgmngxms
[tex]: https://github.com/lh3/minimap2/tree/master/tex
[minimap1]: https://github.com/lh3/minimap
[bwamem-long]: http://lh3.github.io/2014/12/10/bwa-mem-for-long-error-prone-reads
[minialign]: https://github.com/ocxtal/minialign
[ultra-long]: http://lab.loman.net/2017/03/09/ultrareads-for-nanopore/
[quast-lg]: http://cab.spbu.ru/software/quast-lg/
[tofu-u2]: https://github.com/PacificBiosciences/IsoSeq_SA3nUP/wiki/%5BBeta%5D-ToFU2:-running-and-installing-ToFU2
[andrew]: https://blog.dnanexus.com/author/acarroll/
[mm2]: https://github.com/lh3/minimap2
[bwa]: https://github.com/lh3/bwa
