---
layout: post
title: "On the MPEG-G alignment format"
description: ""
category: 
tags: []
---
{% include JB/setup %}

SAM is a text format that is typically used to store the alignment of
high-throughput sequence reads against a reference genome. BAM is the first
binary representation of SAM designed at the same time. BAM is smaller, faster
to process and has additional features like random access.

BAM is not optimal in terms of compression ratio. By reorganizing binary data
and using more advanced compression techniques, we can make alignments much
more compressible. There have been many attempts to replace BAM with a better
binary format, such as [DeeZ][deez], [Quip][quip], [cSRA][csra] (PPT),
[GenComp][gencomp], [cSAM][csam], [Goby][goby] and [samcomp][samcomp] (see
[Hosseini et al (2016)][aln-review] for a more thorough review). The team
maintaining the SAM spec finally adopted [CRAM][cram] as the future of
alignment format. CRAM is much smaller than BAM and has a similar feature set.
With the new codec implemented in [scramble][scramble], it is as fast as BAM in
routine data processing.

[MPEG-G][mpeg-g] is a new binary format that aims to replace BAM. [Its
preprint][mpeg-g-bior] claims "10x improvement over the BAM format" in the
abstract. However, in the only compression ratio comparison, Figure 3, MPEG-G
is only 6.54x as small, not 10x. In addition, Figure 3 suggests sequences and
qualities are of different sizes in SAM (green vs orange). This could happen
(some reads don't have qualities), but is very rarely the case in real-world
BAMs. I am also surprised by Figure 3a, where MPEG-G can compress qualities
much more than sequences (green vs orange). On real data produced today,
qualities are harder to compress because they don't follow a clear pattern. I
suspect the authors are employing lossy compression, possibly with one of the
algorithms developed by [a key contributor][voges] to MPEG-G.  Furthermore, the
usability of a format is more than just compression ratio.  Encoding/decoding
has to be performant. The preprint shows no evaluation. James Bonfield, the key
developer behind the latest CRAM, [has similar concerns][jkb1] with their
previous results.

Much of the above is my speculation. I could be wrong. And it is easy to prove
me wrong: make the data and software available and let the world reproduce
Figure 3. Unfortunately, although the [MPEG-G specification][mgspec] is
available, the implementation and the [benchmark data][data] are not. This
leads to my following point:

MPEG-G is an open standard endorsed by [ISO][iso]. However, open doesn't mean
free. Remember the [royalties][royalty] imposed by [H.264/MPEG-4 AVC][h264]?
MPEG-G may be going down the same route. Key contributors [are applying for
patents][jkb2] and may have financial interest in the format. Before the MPEG-G
authors 1) open source the reference implementation and 2) make the format
[royal-free][rf] like [AV1][av1], I recommend everyone to use BAM or CRAM.

***Disclaimer***: I was the key contributor to BAM, the format that CRAM and
MPEG-G aim to replace, and I am still a contributor to the SAM/BAM spec and its
reference implementation. I have no competing financial interests in
SAM/BAM/CRAM or its reference implementation htslib.

[aln-review]: https://www.mdpi.com/2078-2489/7/4/56
[deez]: https://www.ncbi.nlm.nih.gov/pubmed/25357237
[csra]: https://www.ncbi.nlm.nih.gov/core/assets/sra/files/csra-fileformat.ppsx
[cram]: https://www.ncbi.nlm.nih.gov/pubmed/21245279
[gencomp]: https://www.ncbi.nlm.nih.gov/pubmed/29046896
[csam]: https://www.ncbi.nlm.nih.gov/pubmed/27540265
[samcomp]: https://www.ncbi.nlm.nih.gov/pubmed/23533605
[goby]: https://www.ncbi.nlm.nih.gov/pubmed/24260313
[scramble]: https://www.ncbi.nlm.nih.gov/pubmed/24930138
[quip]: https://www.ncbi.nlm.nih.gov/pubmed/22904078
[white]: https://mpeg.chiariglione.org/sites/default/files/files/standards/docs/w17468%20MPEG-G%20White%20paper.docx
[mpeg-g-bior]: https://www.biorxiv.org/content/early/2018/09/27/426353
[mpeg-g]: https://mpeg-g.org
[iso]: https://en.wikipedia.org/wiki/International_Organization_for_Standardization
[voges]: https://github.com/voges
[mgspec]: https://mpeg.chiariglione.org/standards/mpeg-g/genomic-information-representation/study-isoiec-cd-23092-2-coding-genomic
[data]: https://github.com/voges/mpeg-g-gidb
[jkb1]: https://datageekdom.blogspot.com/2018/09/mpeg-g-bad.html
[jkb2]: https://datageekdom.blogspot.com/2018/09/mpeg-g-ugly.html
[h264]: https://en.wikipedia.org/wiki/H.264/MPEG-4_AVC#Licensing
[royalty]: https://en.wikipedia.org/wiki/Royalty_payment
[rf]: https://en.wikipedia.org/wiki/Royalty-free
[av1]: https://en.wikipedia.org/wiki/AV1
