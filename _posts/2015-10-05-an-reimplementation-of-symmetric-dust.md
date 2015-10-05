---
layout: post
title: "A reimplementation of symmetric DUST"
description: ""
category: 
tags: []
---
{% include JB/setup %}

I have just [reimplemented][sdust] the [symmetric DUST algorithm][paper]
(SDUST) for masking low-complexity regions. The program depends on [kdq.h][kdq]
(double-ended queue) and [kvec.h][kvec] (simple vector); the command line
interface further requires [kseq.h][kseq] for FASTA/Q parsing. As I have tried
on human chr11, the output is identical to the output by [NCBI's
dustmasker][dm] except at assembly gaps. The speed is four times as fast.
I have also compared this implementation to [mdust][mdust], which is supposed
to be a reimplementation of the original asymmetric DUST. The mdust result
under the same score threshold seems to differ significantly from
SDUST/dustmasker. I haven't looked into the cause.

I understand the basis of the SDUST algorithm, which is quite elegant, but I
haven't fully understood all the implementation details. I was just literally
translating the pseudocode in the paper to C, with occassional reference to the
dustmasker source code. If you have any problems, please let me know.

[sdust]: https://github.com/lh3/minimap/blob/master/sdust.c
[mdust]: ftp://occams.dfci.harvard.edu/pub/bio/tgi/software/seqclean/
[paper]: http://www.ncbi.nlm.nih.gov/pubmed/16796549
[kvec]: https://github.com/lh3/minimap/blob/master/kvec.h
[kdq]: https://github.com/lh3/minimap/blob/master/kdq.h
[kseq]: https://github.com/lh3/minimap/blob/master/kseq.h
[dm]: http://www.ncbi.nlm.nih.gov/IEB/ToolBox/CPP_DOC/lxr/source/src/app/dustmasker/
