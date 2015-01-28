---
layout: post
title: "The early history of the SAM/BAM format"
description: ""
category:
tags: []
---
{% include JB/setup %}

While I was looking for an ancient email on my old (first) macbook, I noticed
the numerous email exchanges during the early days of the SAM/BAM format. Here
is a brief summary. The ideas below were proposed by various people in the 1000
Genomes Project analysis group.

* 2008-10-21: SAM got its name.
* 2008-10-22: The first day: fixed columns and optional tags; extended CIGAR and
  binning index.
* 2008-10-24: Compression suggested. RAZF started.
* 2008-11-01: 2-byte tags; mate positions as fixed columns.
* 2008-11-03: Adopted the text/binary dual format. RAZF implemented.
* 2008-11-06: Sequence dictionary. The very first draft of SAM/BAM spec circulated.
* 2008-11-07: BGZF proposed. BAM got its name.
* 2008-11-10: Linear index.
* 2008-11-12: Read group.
* 2008-11-14: BGZF implemented. BAM on top of RAZF working.
* 2008-11-18: Combining binning and linear indices.
* 2008-11-20: sort/merge/pileup/faidx implemented.
* 2008-11-21: tview prototype working.
* 2008-12-08: Final draft sent to 1000g. Adopted the MIT license.
* 2008-12-22: First public release of samtools. It is still working on most BAMs nowadays.
