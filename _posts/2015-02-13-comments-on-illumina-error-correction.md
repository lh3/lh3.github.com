---
layout: post
title: "Correcting Illumina sequencing errors: extended background"
description: ""
category:
tags: []
---
{% include JB/setup %}

I enjoy writing 2-page Application Notes these days. It takes less time to
write, giving me more time on solving other problems. More importantly, I don't
need to fight to claim significance and novelty which are subjective most of
time. The downside of writing short manuscripts is the lack of extensive
discussions. Here are something I have not said in my [new error correction
preprint][preprint].

#### The role of error correction

Error correction is only used for *de novo* assembly but almost nothing else. It
is usually wise not to apply error correction for mapping based analyses because
mapping looks at the full length of a read, but error correction, especially
k-mer based algorithms, only uses local information and is not as powerful. When
most Illumina data are processed through mapping based pipelines, error
correction is of limited uses. Then why do I care about error correction?

I have long believed that one day we will keep processed data only and throw
away raw sequence reads just as we have trashed images and intensities and
reduced the resolution of base qualities. With more and more high-coverage human
data coming out, a *de novo* assembly, rather than a gVCF, would be the best
form to keep because 1) like a gVCF, an assembly is much smaller than raw data;
2) unlike VCF, a perfect assembly is a lossless representation of raw reads; 3)
hence a perfect assembly encodes all types of variants, not only SNPs and short
INDELs; 4) unlike VCF, a *de novo* assembly is not bound to a particular
reference genome; 5) and is free of artifacts caused by the reference; 6)
mapping and variant calling from assembled unitigs is much more efficient and in
some way better. I have been working on this idea from time to time since 2011
and [published a paper][fermi] in 2012, but I keep coming back to error
correction.

#### My past efforts in sequencing error correction

The 2012 paper is not very successful. Although on calling short variants, it
was better than other assembly-based approaches at that time, it was much less
sensitive in comparison to mapping based variant calling. I later realized that
this was mainly caused by the aggressive error corrector. The corrector I
implemented for that paper corrected many heterozygotes away. A couple of year
later, I reimplemented the correction algorithm but made it more conservative.
This is the error corrector in fermi2. The sensitivity was much improved.
Assembly-based variant calling started to compete well with mapping-based
calling on both sensitivity and specificity.

#### Development of error correctors in 2014

The fermi2 error corrector was developed in late 2013 when there were few
Illumina error correctors fast and lightweight enough to handle whole-genome
human data. This situation was changed in 2014. In May, [BLESS][bless] was
published with a substantially improved version coming later in October that
uses KMC2 as the k-mer counter. [Lighter][lighter] was out in September.
[Bloocoo][bloocoo] was on Bioinformatics Advanced access channel in October as
part of GATB. We have three performant error correctors in less than a year.
Fiona and Trowel were also published this year, but they are not capable of
correcting human data. In addition, [Molnar and Ilie (2014)][review], the
developers of HiTEC and RACER, for the first time evaluated error correction for
whole-genome human data, though due to timing, they have not included the five
new error correctors mentioned above (the review included an old version of
BLESS, which is slow, single-threaded and does not work with reads of variable
lengths).

#### The motivation of developing BFC

Although BLESS, Bloocoo and Lighter are all faster and more lightweight than the
error corrector implemented in fermi2, I have two potential concerns, one
theoretical and the other practical. Firstly, these tools use greedy algorithms
in that they rarely (if at all) revert a correction at an earlier step even if
doing so helps to correct the rest of the read. The algorithm I implemented in
fermi and fermi2 are theoretically better (for details, see [the
preprint][preprint]), I think. Secondly, Illumina sequencing occasionally
produces systematic errors. These are recurrent sequencing errors that usually
have low base quality. Being aware of base quality during the k-mer counting
phase would practically help to fix these errors. These concerns together with
the poor performance of the fermi2 corrector motivated me to develop a new error
corrector, BFC. Honestly, I didn't know how much the two concerns would matter
in practical applications before I started to implement BFC.

#### The development of BFC

The BFC algorithm has been detailed in [the preprint][preprint]. I will add
something untold.

When I started to evaluate BFC, I was only aware of SGA, Lighter and the old
BLESS. Lighter-1.0.4 is very slow on compressed input. Old BLESS does not work
as the test data have reads of variable lengths, while new BLESS is so
challenging to build that I gave it up initially. I was quite happy to see
BFC was several times faster than these tools.

I was overoptimistic. When I monitored the Lighter run more closely, I found it
was mostly single-threaded. I then tried Lighter on uncompressed input. It was
six times as fast as the run on compressed input. I told the Lighter developers
my findings and suggested solutions. They were very responsive and quickly
improved the performance. The timing reported in the preprint was from the new
Lighter.

Having seen Lighter could be much faster, I decided to get BLESS compiled. It
was a right decision. The new BLESS is working well, especially when it uses
long k-mers. This has inspired further explorations: use of KMC2 and long k-mers
for error correction. I implemented a variant of BFC, BFC-bf, to take KMC2
counts as input and keep the counts in a bloom filter. I have to ignore base
quality during the counting phase as KMC2 is not aware of base quality in the
way I would prefer. The correction accuracy is not as good as BFC. Nonetheless,
BFC-bf helps to confirm that the apparently better correction accuracy is not
purely due to the use of base quality during k-mer counting. BFC-bf also makes
it easier to use long k-mers.

The role of k-mer length in error correction is complicated. Since assembling
corrected reads takes much more computing time and my development time, I have
not run *de novo* assembly often at the beginning. I assumed that low under- and
over-correction rates should lead to better assemblies. I was wrong again.
At least for fermi2, short k-mers used for correction lead to better assemblies
(for reasons, see the discussions in the preprint). I have also tried to combine
two k-mer lengths but only with limited success.

Here I should admit that a weakness of the manuscript is that I have not run *de
novo* assemblies for all correctors. This is mostly because I do not have enough
computing resources. Another key reasons is the assembly result varies with
assemblers. It is too much for me to try *M* assemblers on *N* correctors. I did
briefly try fermi2 assembler on *E. coli* data corrected by a few (not all)
tools. BFC is better, which is a relief but not conclusive. I need to try more
assemblers on more data sets to get a firm view. This is for future.



[preprint]: http://arxiv.org/abs/1502.03744
[fermi]: http://www.ncbi.nlm.nih.gov/pubmed/22569178
[bless]: https://sourceforge.net/projects/bless-ec/
[bloocoo]: https://gatb.inria.fr/software/bloocoo/
[lighter]: https://github.com/mourisl/Lighter
[review]: http://www.ncbi.nlm.nih.gov/pubmed/25183248
