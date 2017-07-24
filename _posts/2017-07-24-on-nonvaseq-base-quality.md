---
layout: post
title: "On NovaSeq Base Quality"
description: ""
category: 
tags: []
---
{% include JB/setup %}

### Introduction

Illumina Inc. released [NovaSeq][novaseq] earlier this year and provided sample
data [at BaseSpace][nova-data] several months later. Different from the HiSeq
series, NovaSeq uses [2-color chemistry][2-color]. It has been observed that
the NextSeq series, which also uses 2-color chemistry, [produced data of worse
quality][nextseq-bad]. One naturally wonders if NovaSeq has a similar problem.
This post might give you some hints.

### Data and methods

I am looking at three whole-genome Illumina runs for human sample NA12878,
with data produced on HiSeq 2500, HiSeq X Ten and NovaSeq, respectively.
The HiSeq X Ten and NovaSeq runs used a PCR-free protocol. Their data are
currently available at BaseSpace. The HiSeq 2500 run did not specify whether
PCR was applied, but based on the low PCR duplicate rate after alignment, I
believe it used a PCR-free protocol as well. The HiSeq 2500 data is also
available, but the link to the data is hidden.

I am focusing on the *empirical base quality* (emQ) rather than raw base
quality in the original FASTQ file. Raw base quality can be deceiving if it is
not well calibrated, whereas emQ ideally reflects the true base error rate. To
estimate emQ, we traverse the pileup of high-coverage data, exclude obvious
variant sites and count the rest of differences from the reference as
sequencing errors. In implementation, I regarded a site as a potential variant
if 35% of high-quality bases at the site are different from the reference. This
treatment is not perfect, but is easy to implement and often adequate unless we
care about base quality well over Q40 (see also Results).

### Results

The NovaSeq FASTQs only consist of four possible quality values: 2, 12, 23 and 37.
The overall emQ for HiSeq 2500, HiSeq X Ten and NovaSeq is Q27, Q23 and
Q24, respectively. As is [observed before][x10-qual], HiSeq X Ten has a higher
empirical error rate than older HiSeq machines. NovaSeq is slightly better than
HiSeq X Ten but not as good as HiSeq 2500 or the Platinum Genomes data.

The solid black in panel A-C in the following figure shows emQ at each cycle

![][img-novaseq]

It is clear that emQ drops with cycles and drops even more on the second read.
The black line is largely determined by the fraction of low-quality bases.
It is related to the fraction of mappable reads. However, for variant calling,
we usually ignore low-quality bases because they tend to be affected by
systematic errors. In panel A-C, the four solid color lines indicate
empirical base quality for high-quality A/C/G/T bases. Recall that all NovaSeq
Q30 bases have quality 37. The four solid lines in (A) suggest that these Q37
bases are about right but are overestimated in particular towards the end of
read2.

When calling variants from high-coverage samples, we almost always ignore
singleton errors (errors that occur only once at a site). We more care about
how often two or more high-quality errors occur at the same site. In panel A-C,
the four dashed color lines show the emQ if we ignore singleton errors.  Both
HiSeq X Ten and NovaSeq look good. Their dashed lines are probably hitting the
limit of this analysis. They will probably stay higher if we exclude NA12878
variants in a more sophisticated way. I speculate that the lower coverage of
HiSeq 2500 lowers its dashed lines. As HiSeq 2500 is not my focus here, I have
not dig into the details.

Finally, panel (D) shows the frequency of erroneous base changes. Ideally, we
would like to see four horizontal lines at y=33%: when there is an error, the
error is randomly chosen from the three other types of bases. This is far from
the truth in reality. At the head of read1, few A base errors are true C bases
in the sample (blue line), but at the tail of read2, the trend is inversed.
While this observation is certainly not ideal, it is not that bad as long as
we rarely see two high-quality errors at the same site (dashed lines in panel
A).

### Discussions and conclusions

The public NovaSeq data from BaseSpace is broadly comparable to HiSeq X Ten
data in terms of empirical base quality. Like HiSeq X Ten, NovaSeq also
overestimates base quality, but personally I do not see this a big issue.
For high-coverage data, what is more important is the rate of systematic errors
and other mapping artifacts. Moderate inaccuracy in base quality rarely
matters except in artifactual benchmarks.

It should be noted that data quality varies, sometimes greatly, between runs
and across sequencing facilities. Having analyzed one run from each machine
model, this analysis may not be easily generalized to data produced elsewhere.
It is recommended to redo the analysis on your own data.

[novaseq]: https://www.illumina.com/systems/sequencing-platforms/novaseq.html
[nova-data]: https://basespace.illumina.com/datacentral
[2-color]: https://sequencing.qcfail.com/articles/illumina-2-colour-chemistry-can-overcall-high-confidence-g-bases/
[nextseq-bad]: http://seqanswers.com/forums/showthread.php?t=40741
[x10-qual]: http://lh3.github.io/2014/11/03/on-hiseq-x10-base-quality
[img-novaseq]: http://lh3lh3.users.sourceforge.net/images/novaseq-qual.png
