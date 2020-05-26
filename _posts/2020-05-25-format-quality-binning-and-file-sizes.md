---
layout: post
title: "Format, quality binning and file size"
description: ""
category: 
tags: []
---
{% include JB/setup %}

This short post evaluates the effect of format and quality binning on file
sizes. I am taking [SRR2052362][SRR2052362] as an example. It gives 4.3-fold
coverage on the human genome. For 2-binning, I turned original quality 20 or
above to 30 and turned original quality below 20 to 10. For 8-binning, I took
the scheme from a [white paper][il-white] (PDF) published by Illumina.
Illumina has been using quality binning for more than seven years. In this
experiement, I only retained the original read names. To produce CRAM files, I
mapped the short reads to the GRCh38 primary assembly. The following table
shows the file sizes:

<style> .extable td,th { padding: 4px; } </style>
<table border="1" class="extable">
<tr><th>Format       </th><th># qual bins</th><th>                     Size (GB)</th><th>       Change relative to SRA</th></tr>
<tr><td>Sorted CRAM  </td><td>2 bins     </td><td style="text-align:right">1.187</td><td style="text-align:right">-85%</td></tr>
<tr><td>Unsorted CRAM</td><td>2 bins     </td><td style="text-align:right">1.279</td><td style="text-align:right">-84%</td></tr>
<tr><td>Unsorted CRAM</td><td>8 bins     </td><td style="text-align:right">2.115</td><td style="text-align:right">-73%</td></tr>
<tr><td>Gzip'd FASTA </td><td>No quality </td><td style="text-align:right">4.172</td><td style="text-align:right">-47%</td></tr>
<tr><td>Unsorted CRAM</td><td>Lossless   </td><td style="text-align:right">4.536</td><td style="text-align:right">-43%</td></tr>
<tr><td>Gzip'd FASTQ </td><td>2 bins     </td><td style="text-align:right">4.784</td><td style="text-align:right">-40%</td></tr>
<tr><td>SRA          </td><td>Lossless   </td><td style="text-align:right">7.917</td><td style="text-align:right">  0%</td></tr>
<tr><td>Gzip'd FASTQ </td><td>Lossless   </td><td style="text-align:right">9.210</td><td style="text-align:right">+16%</td></tr>
</table>

It is clear that the CRAM format is the winner here and the advantage of CRAM
is more prominent given lower quality resolution. A key question is how much
quality binning affects variant calling. Brad Chapman [concluded][bcb-eval]
8-binning had little effect on variant calling accuracy. With Crumble, James
Bonfield [could get][crumble] a little higher accuracy with lossy compression.
[FermiKit][fermikit] effectively uses 2-binning and can achieve descent
results. I applied 2-binning to GATK many years ago and observed 2-binning
barely reduced accuracy. The GATK team at Broad Institute also evaluated
2-binning and 4-binning. They found 4-binning was better than 2-binning and was
as good as original quality. The overall message is that we don't need full
quality resolution to make accurate variant calls for germline samples.
The effect on tumor samples is more of an open question, though.

It is worth noting that completely discarding base quality dramatically reduces 
variant calling accuracy. I have observed this both with FermiKit and with
GATK (I didn't keep the results unfortunately). This is because low-quality
Illumina sequencing errors are correlated, in that if one low-quality base is
wrong, other low-quality bases tend to be wrong in the same way. Without base
quality, variant callers wouldn't be able to identify such recurrent errors.

[SRR2052362]: https://trace.ncbi.nlm.nih.gov/Traces/sra/?run=SRR2052362
[bcb-eval]: https://bcbio.wordpress.com/2013/02/13/the-influence-of-reduced-resolution-quality-scores-on-alignment-and-variant-calling/
[il-white]: https://www.illumina.com/Documents/products/whitepapers/whitepaper_datacompression.pdf
[crumble]: https://academic.oup.com/bioinformatics/article/35/2/337/5051198
[fermikit]: https://github.com/lh3/fermikit
