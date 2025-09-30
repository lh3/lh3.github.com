---
layout: post
title: "Our journey through low-complexity regions"
description: ""
category: 
tags: []
---
{% include JB/setup %}

Those who inspect SVs often have probably noticed many SVs fall in LCRs or tandem repeats and they are challenging to call.
How many and how challenging? I could not find a good answer in the literature, so
decided to work with Qian (Alvin) Qin to measure by ourselves.

To start with, we need to identify LCRs in the human genome.
This turned out to be a non-trivial problem.
At first, we attempted to use published tandem repeat catalogs.
However, targeting short-read tandem repeat calling,
existing catalogs contain millions of fragmented regions that do not look highly repetitive.
We did not know how to filter them given the large differences between these catalogs.
Another practical issue is that these catalogs are defined on GRCh38 only
and are hard to be reproduced for another assembly.

We then considered to find LCRs with standalone tools like TRF and ULTRA,
but there was a similar problem: the overlaps between these tools are not large.
We did not know what regions to choose: union, intersection, or something in between?
This question was further complicated by the fact
that each of these tools employs its own empirical model.
It is difficult to interpret the differences between tools.
Furthermore, the tandem repeating finding tools ignore LCRs without clear tandem pattern.
Some of these tools, such as TRF and ULTRA, are also resource demanding.
TRF may even fall into a near dead loop if option "-l" is not large enough, but a large "-l" consumes a lot of memory.
We are not happy with existing tools, so decided to develop our own.

This is [longdust][ld].
During its development, we realized that longdust, TRF and ULTRA can have large overlaps
if we focus on highly repetitive LCRs.
The differences between tools mostly come from borderline regions which we do not care too much for the SV analysis.
In retrospect, TRF would probably be good enough for our purpose if we had thrown enough computing and known how to work with it.
Nonetheless, longdust is still worth our effort given its high performance
and more importantly, its explicit definition of string complexity.

After a long detour, we came back to our original problem on SV calling.
At that point, we understood the bahaviors of LCR finding tools better
and were more confident in choosing the right strategy to find LCRs.
In the end, we used longdust combined with the HPRC graph for LCRs missing from GRCh38.
I am not going to repeat [our preprint][lcr-preprint].
I will just show the summary figure:

![][fig]

Note that LCRs do not only affect SV calling; they are also a major source of SNP calling errors
as I showed [in 2014][2014-paper] and [recently][easy].
They need to be analyzed separately, and carefully.

[ld]: https://github.com/lh3/longdust
[lcr-preprint]: https://arxiv.org/abs/2509.23057
[fig]: https://i.ibb.co/6RrTHJd3/lcr-eval.png
[2014-paper]: https://academic.oup.com/bioinformatics/article/30/20/2843/2422145
[easy]: https://academic.oup.com/gigascience/article/doi/10.1093/gigascience/giaf103/8247769
[sdust]: https://pubmed.ncbi.nlm.nih.gov/16796549
