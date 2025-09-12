---
layout: post
title: "A quick look at Roche's SBX"
description: ""
category: 
tags: []
---
{% include JB/setup %}

[Sequencing by Expansion (SBX)][sbx] is Roche's new short-read sequencing technology.
Roche released the first batch of public SBX data after a webinar yesterday.
This blog post provides my impression with the data.
If you are interested in SBX, I *highly* recommend watching the recording of the webinar,
as it contains a lot more information than this blog post.

![][fig]

Roche released duplex SBX (SBX-D) data produced in one hour by multiplexing seven human samples together.
These are single-end reads, averaging ~241bp in length.
The purple line in Fig. (a) shows the fraction of reads beyond a certain length.
We can see more than 95% of reads are longer than 150bp but less than 10% are longer than 350bp.

With SBX-D, the sequencing of the opposite strand begins at the 3'-end of the first strand.
As a result, most 3'-ends are in duplex but 5'-ends tend to be covered by one strand only.
The current SBX-D basecaller assigns a quality score of 39 to bases supported by both strands,
5 to bases covered by both strands but with disagreement between the two,
and 22 to bases covered by one strand only.
We can use this information to estimate the fraction of duplex reads (green curve).
At the 5'-end, less than 60% of read bases are in duplex
and the fraction increases to 85% at ~200bp.
The drop afterwards might be caused by the reduction of longer reads, though I haven't looked into details.
It might be clearer to draw the curve from the 3'-ends of all reads.

Sequencing errors in SBX-D reads are primarily due to 1bp insertions.
This results from an interesting decision to always choose bases from the longer strand when the two strands disagree.
In this case, the SBX-D basecaller assigns a quality score of 5 to the extra base(s).
In a homopolymer run, only the last base(s) are lowered to 5 but the rest of the bases still receive quality 39.
This quality assignment caused [my previous tool][mapchk] to report a low empirical quality.
I had to modify the source code for SBX-D.

Fig. (b) shows the empirical base quality (emQ) along the reads.
The overall SBX-D emQ stays almost constant at Q21 across base positions (red curve).
Bases confirmed by both strands reach a higher emQ at Q37, close to the estimated value of Q39 (dark blue curve).
This is comparable to the Illumina NovaSeq results from [Baid et al (2020)][ns-data] (purple and green curves).
The two technologies are also similar when we look at recurrent errors supported by two or more Q30+ bases (orange and light blue curves).
Roche further demonstrated similar SNP and indel calling accuracy to NovaSeq during the webinar.

**Overall, I am really impressed by the quality of SBX-D.**
Nevertheless, I believe the decision to always choose the longer strand is questionable.
This will leave a trap for many current analyses including all pileup-based ones.
While DeepVariant can learn the pattern and GATK can be tuned, they are only part of the ecosystem.
I have seen sequencing technologies that produced data of special structures or features,
but successful ones more often look like ordinary data.
In case of SBX-D, the basecaller doesn't distinguish good from bad bases based on raw signals.
I suspect it is possible to use raw signals to generate accurate duplex consensus
such that we can analyze SBX-D data the same way as typical short reads.
This would be ideal for everyone.

**The elephant in the room is pricing**, which Roche understandably has kept silent about.
If SBX were priced similarly to NovaSeq, many would not bother to adapt their tools and workflows just for SBX;
if SBX were much cheaper, we might see a shift in tool development towards SBX.
We will see.

[sbx]: https://sequencing.roche.com/us/en/article-listing/sequencing-platform-technologies.html
[mapchk]: https://github.com/lh3/htsbox/blob/lite/mapchk.c
[ns-data]: https://www.biorxiv.org/content/10.1101/2020.12.11.422022v1
[fig]: https://i.ibb.co/1f50pSyF/Screenshot-2025-09-11-at-20-25-15.png
