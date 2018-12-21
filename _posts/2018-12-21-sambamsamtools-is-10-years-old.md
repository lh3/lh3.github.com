---
layout: post
title: "SAM/BAM/samtools is 10 years old"
description: ""
category: 
tags: []
---
{% include JB/setup %}

I wrote a commentary on the SAM/BAM format a while ago. I now publish it as a
blog post, 10 years after I released the [first samtools][st1] to SourceForge.
It gives an overview about what has happened in the past 10 years. Here "we"
refers to the samtools dev team and the HTS file format commitee, as well as
those who have contributed to the specification or samtools/htslib. I am only
one of them.

<hr/>

When the [1000 Genomes Project][1000g] was launched in early 2008, there were already many
short-read aligners and variant callers. Each of them had its own input or
output format for limited use cases. They did not talk to each other. We had to
implement various format converters to bridge tools, which was awkward and even
sometimes impossible as formats may encode different information. The
fragmented ecosystem hampered the collaboration between the participants of the
project and delayed the development of advanced data analysis algorithms.

In a conference call on October 21, 2008, the 1000 Genome Project analysis
subgroup decided to take on the issue by unifying a variety of short-read
alignment formats to a [Sequence Alignment/Map format][sam] or SAM in short. Towards
the end of 2008, the subgroup announced the first SAM specification, detailing
a text-based SAM format and its binary representation, the BAM format. SAM/BAM
quickly replaced all the other short-read alignment formats and became the de
facto standard in the analysis of high-throughput sequence data. Nowadays,
SAM/BAM is not limited to a short-read format any more. It is able to represent
noisy read alignments of millions of bases in length (feature 6 and 9 in Table
1), and also finds its uses as the primary format to store signal data for
IonTorrent and PacBio (feature 3).

One of the most influential decisions on evolving the SAM ecosystem is the
separation of the application programming interface (API) from the command-line
tools. The SAM/BAM format originally came with a reference implementation,
[samtools][samtools]. While samtools provided primitive APIs to parse SAM/BAM files, it
mixed the APIs with applications and did not promise long-term stability, which
made it difficult to interface in other programs. To address this issue, we
created [htslib][htslib] in 2014, a dedicated programming library in C that processes
common data formats used in high-throughput sequencing. This library implements
stable and robust (e.g. feature 1 and 11) APIs that other programs can rely on.
It enables efficient access to SAM/BAM in other popular programming languages
such as Python and R and boosts the development of sequence analysis tools.

Htslib is not merely a separation; it also brought numerous improvements to
samtools and third-party programs depending on it. Htslib supports
multi-threading and uses faster compression libraries. In comparison to the
original samtools, it reduces the wall-clock processing time by several folds
on multiple cores, broadly matching the performance of [sambamba][sambamba]. Htslib can
directly access BAM files on remote HTTP/FTP servers or cloud storages such as
DropBox, Google Cloud and Amazon Web Services (feature 8). Users can extract
and visualize alignments in a small region without downloading the entire
dataset, which can be thousands of times smaller than the entire dataset.
Recently, we have pushed this feature further by implementing the [htsget][htsget]
protocol (feature 10). This protocol eliminates bulk data recoding and thus
takes tens of times less computing resource than remote BAM access. Htslib
seamlessly supports the [CRAMv3 format][cram] (feature 3), a more compact binary
representation of SAM. On high-coverage data, CRAM is typically twice as small
as BAM containing identical information. Htslib, samtools and popular libraries
that include a copy of the htslib source code (e.g. [Pysam][pysam] and [Rsamtools][rsamtools]) have
been downloaded over 3 million times in the past 10 years.

Although the SAM format has been revised multiple times in the past, nearly all
changes are backward compatible -- tools supporting more recent versions of
the specification seamlessly work with older versions. Meanwhile, the SAM/BAM
format is also largely forward compatible. The very [first version of samtools][st1]
still parses the vast majority of short-read SAM/BAM produced today. This
long-term stability prevents the fragmentation of the community and is one of
the most critical features of the format. Many may feel SAM is the same old
format announced 10 years ago. However, without the continuous enhancements to
the format and the ecosystem, many routine tasks, such as structural variation
calling, long read alignment, cloud computing and online genome browsing, would
be made more difficult or even impossible.

The SAM format, along with BAM, CRAM and several other formats that htslib
supports, is now maintained by the [Large Scale Genomics work stream][lsg] of the [Global Alliance
for Genomics and Health][ga4gh] initiative. These formats and htslib constantly receive
bug fixes, enhancements and new features, and will continue to empower the
analysis of high-throughput sequence data in the coming years.

|Feature |Year first available |Target |Feature descriptions|
|-------:|:-------------------:|:-----:|:-------------------|
|1 |2009 |BAM |End-of-file marker to detect truncated files|
|2 |2009 |SAM |"="/"X" CIGAR operators to encode sequence matches or mismatches|
|3 |2011 |SAM |"B" tag type to efficiently store binary arrays|
|4 |2012 |BAM |CSI index for chromosomes longer than 512Mbp|
|5 |2012 |htslib |BCFv2 as an efficient binary representation of VCF files|
|6 |2013 |SAM |Supplementary alignment to encode split alignment|
|7 |2014 |htslib |CRAMv3 as a more compact binary representation of SAM|
|8 |2015 |htslib |Direct access to remote files over internet|
|9 |2017 |BAM |BAM extension to support CIGARs with >65535 operators|
|10 |2017 |htslib |Htsget protocol for efficient database access|
|11 |2017 |htslib |CRC error checking to detect internally corrupted BAM files|

[1000g]: https://en.wikipedia.org/wiki/1000_Genomes_Project
[sam]: https://en.wikipedia.org/wiki/SAM_(file_format)
[samtools]: https://github.com/samtools/samtools
[htslib]: https://github.com/samtools/htslib
[sambamba]: http://lomereiter.github.io/sambamba/
[htsget]: https://www.ncbi.nlm.nih.gov/pubmed/29931085
[cram]: https://en.wikipedia.org/wiki/CRAM_(file_format)
[pysam]: https://github.com/pysam-developers/pysam
[rsamtools]: https://bioconductor.org/packages/release/bioc/html/Rsamtools.html
[st1]: https://sourceforge.net/projects/samtools/files/samtools/0.1.1/
[ga4gh]: https://www.ga4gh.org/
[lsg]: https://www.ga4gh.org/work_stream/large-scale-genomics/
