---
layout: post
title: "On HiSeq X10 Base Quality"
description: ""
category:
tags: []
---
{% include JB/setup %}

Illumina has recently released [four lanes of NA12878 data][NA12878] from HiSeq
X10. I was playing with this data set and found that my program had bad accuracy
on two of them. I initially thought the data quality was different, so wrote
some code to investigate the data quality. It turns out that my program was
buggy, but the finding of the HiSeq X10 data quality might be of its own
interest, which I am sharing here.

### HiSeq X10 data quality

When I looked at the HiSeq X10 alignment in samtools tview, my first impression
is that the error rate is visually higher than the previous Illumina data I have
seen. This might be the cause of the [2-channel system][ch2] (as is opposed to
the previous 4-channel). However, a closer look at the base quality suggested I
might be wrong. The average base quality of these HiSeq X10 data is Q37.0 for
reads mapped to chr11. This is compared favorably to NA12878 from [Platinum
Genomes][pgen] (Q36.4) and the CHM1 data used in my paper (Q34.9).

Mean quality is in effect the the geometric mean of error rate, but what I
observed in tview is the arithmetic mean. Could that be the cause? It is not.
The arithmetic mean of HiSeq X10 data is Q24.4, still better than NA12878
(Q24.2) and CHM1 (Q17.6).

However, I still trust my eyes more. I started to believe the base quality HiSeq
X10 reads is overestimated (or the older quality is underestimated). [MAQ][maq]
has a subcommand "mapcheck" to estimate the empirical base quality from read
mapping. I don't have this for BAM, so I implemented one. In this
implementation, if 35% of >=Q20 bases are different from the reference base,
the site is considered to be a variant site and is ignored.

My eyes are right after all. The following table shows the empirical arithmetic
means of different data sets and also stratified by low (<Q20) and high base
quality:

|Dataset|emQ|% <Q20 bases|emQ <Q20|emQ >=Q20|
|-------|---|------------|--------|---------|
|Platinum Genomes|Q26|2.5%|~Q13   |~Q34     |
|CHM1            |Q25|4.3%|~Q12   |~Q33     |
|HiSeq X10 L1    |Q23|4.7%|~Q10   |~Q30     |
|HiSeq X10 L3    |Q23|4.6%|~Q10   |~Q30     |

We can see that the empirical quality of HiSeq X10 data is obviously lower -
almost twice as low. This is consistent with my feeling. Note that on older
data, the empirical quality could go higher if there were no variants. The 35%
rule is not good enough.

Another a bit worrying sign of the new data is the systematic compositional
bias. In particular, 68% of high-quality C bases in FASTQ should really be A but
only 8% be G. There were compositional biases in older Illumina data, but not as
bad. Does this affect variant calling? *Crude* evaluation using [Genome In A
Bottle][giab] (GIAB) suggests the variant calls are still decent. Nonetheless,
more careful comparisons are needed to draw a definite conclusion.

### Base quality resolution

Another visible difference of HiSeq X10 data is the reduced resolution of base
qualities. There are only seven distinct quality values as is opposed to nearly
40 in the previous data. It has long been discussed whether this would impact
the accuracy of variant calls with NCBI being one of the first advocates. I used
to evaluate the effect on the 1000g low-coverage pilot and a high-coverge
NA12878. As I remember (I have lost the data), 7 or even 4 quality values
worked.

Reduced quality resolution has a positive effect on the size of alignment.
Typically, for 35X human data, the size of the final BAM file is about 100GB.
The size of these 35X HiSeq X10 data is only 70GB. A 30% reduction. Could we
push further, say 1-bit quality?

I did an experiment. In the aligned BAM, I turned all quality below Q20 to Q10
all quality no less than Q20 to Q30 (based on the table above). I run GATK-HC on
the original BAM and the quality-reduced BAM. If I compared the exact variant
coordinates, HC called 7,253 unfiltered variants only present in the original
BAM and 15,435 variants only in the quality-reduced BAM. The difference is minor
if we notice that the difference between lane 1 and 7 is over 110,000. On GIAB,
the two call sets are largely indistinguishable. In all, 1-bit quality does not
obviously reduce the accuracy of variant calls.

One-bit quality further reduces the BAM size down to 40GB. In the CRAM format,
it is 16GB. This is a significant reduction from typical 100GB BAMs at 35X
coverage.

Although I have not tried, I firmly believe that we cannot discard base quality
at all. HiSeq produces recurrent errors. These errors are usually correctly
assigned to low base quality. If a variant caller ignores base quality, it is
likely to make calls at these systematic errors. One bit is the minimum.

While I was doing my small experiment, I learned from the GA4GH mailing list
that Illumina is also exploring the possibility of 1-bit quality. I believe this
strategy should be fine for normal samples. I actually think, cautiously, that
1-bit quality may even work for cancer data, but I am not experienced enough to
confirm this.



[NA12878]: https://basespace.illumina.com/datacentral
[ch2]: http://res.illumina.com/documents/products/techspotlights/techspotlight_two-channel_sbs.pdf
[pgen]: http://www.illumina.com/platinumgenomes/
[maq]: http://maq.sourceforge.net
[giab]: http://genomeinabottle.org
