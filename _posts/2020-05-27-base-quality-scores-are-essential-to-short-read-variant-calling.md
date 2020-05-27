---
layout: post
title: "Base quality scores are essential to short read variant calling"
description: ""
category: 
tags: []
---
{% include JB/setup %}

In [an earlier post][old-post] a few days ago, I said "discarding base quality
dramatically reduces variant calling accuracy". I didn't provide evidence. This
certainly doesn't sound persuasive. In this post, I will show an experiement to
support my claim.

I downloaded high-coverage short reads for sample HG002 [from GIAB
ftp][giab-data], converted to unsorted FASTQ with [samtools collate][st-collate], mapped
them to [hs37d5][hs37d5] (for compatibility with GIAB) with bwa-mem, called variants with
GATK v4 and compared the calls to the [GIAB truth v4.1][giab41]. I then
estimated the false negative rate (FNR=1-sensitivity) and false discovery rate
(FDR=1-precision) with [RTG's vcfeval][rtg]. I optionally applied the hard
filters proposed in [my earlier paper][hapdip]. For "no quality", I set all base quality
to Q25 which corresponds to the average empirical error rate of this dataset.

<style> .extable td,th { padding: 4px; } </style>
<table border="1" class="extable">
<tr><th># qual bins</th><th>Filtered</th><th>SNP FNR</th><th>SNP FDR</th><th>INDEL FNR</th><th>INDEL FDR</th></tr>
<tr><td>Lossless   </td><td>No      </td><td style="text-align:right">0.58%</td><td style="text-align:right">0.46%</td><td style="text-align:right">0.63%</td><td style="text-align:right">0.25%</td></tr>
<tr><td>8 bins     </td><td>No      </td><td style="text-align:right">0.58%</td><td style="text-align:right">0.45%</td><td style="text-align:right">0.66%</td><td style="text-align:right">0.26%</td></tr>
<tr><td>2 bins     </td><td>No      </td><td style="text-align:right">0.59%</td><td style="text-align:right">0.95%</td><td style="text-align:right">0.55%</td><td style="text-align:right">0.34%</td></tr>
<tr><td>No quality </td><td>No      </td><td style="text-align:right">0.60%</td><td style="text-align:right">6.38%</td><td style="text-align:right">0.64%</td><td style="text-align:right">0.44%</td></tr>
<tr><td>Lossless   </td><td>Yes     </td><td style="text-align:right">2.54%</td><td style="text-align:right">0.07%</td><td style="text-align:right">2.27%</td><td style="text-align:right">0.06%</td></tr>
<tr><td>8 bins     </td><td>Yes     </td><td style="text-align:right">2.52%</td><td style="text-align:right">0.07%</td><td style="text-align:right">2.30%</td><td style="text-align:right">0.06%</td></tr>
<tr><td>2 bins     </td><td>Yes     </td><td style="text-align:right">2.53%</td><td style="text-align:right">0.11%</td><td style="text-align:right">2.24%</td><td style="text-align:right">0.07%</td></tr>
<tr><td>No quality </td><td>Yes     </td><td style="text-align:right">2.71%</td><td style="text-align:right">0.20%</td><td style="text-align:right">2.51%</td><td style="text-align:right">0.08%</td></tr>
<tr><td>HiFi; no qual</td><td>No    </td><td style="text-align:right">0.80%</td><td style="text-align:right">0.10%</td><td style="text-align:right">1.46%</td><td style="text-align:right">1.29%</td></tr>
</table>

Several comments:

* If we completely drop base quality, the SNP FDR becomes 10 times higher.
  Most of additional false calls are due to low ALT allele fraction. Hard
  filtering can improve this metric but the resulting SNP FDR is still twice as
  high. **Base quality scores are essential to accurate variant calling. For
  somatic mutation calling, short reads without base quality are virtually
  useless.**

* Using 2 quality bins (i.e. good/bad) gives a dramtic improvement over
  no-quality, though the result is not as good as 8-binning.

* The accuracy of variants called with 8 quality bins is indistinguishable from
  the accuracy with the original quality. The file size of the sorted 8-binning
  alignment in CRAM is less than a quarter of the size of the orignal input
  in gzip'd FASTQ.

* I guess using 4 quality bins may achieve the best balance between storage and
  accuracy. The GATK team reached this conclusion years ago. I forgot what was
  the exact binning scheme in use, so I am not including an experiment here.

* The last line in the table evaluates [dipcall][dipcall] variants called
  from a HiFi trio binning assembly. [Hifiasm][hifiasm] is the only assembler
  to date that can achieve this accuracy.

[old-post]: http://lh3.github.io/2020/05/25/format-quality-binning-and-file-sizes
[giab-data]: https://ftp-trace.ncbi.nlm.nih.gov/giab/ftp/data/AshkenazimTrio/HG002_NA24385_son/NIST_HiSeq_HG002_Homogeneity-10953946/HG002Run01-11419412/
[rtg]: https://github.com/RealTimeGenomics/rtg-tools
[hapdip]: https://pubmed.ncbi.nlm.nih.gov/24974202/
[giab41]: https://ftp-trace.ncbi.nlm.nih.gov/giab/ftp/data/AshkenazimTrio/analysis/NIST_v4.1_SmallVariantDraftBenchmark_12182019/GRCh37/
[hifiasm]: https://github.com/chhylp123/hifiasm
[dipcall]: https://github.com/lh3/dipcall
[hs37d5]: https://ftp-trace.ncbi.nlm.nih.gov/1000genomes/ftp/technical/reference/phase2_reference_assembly_sequence/
[st-collate]: http://www.htslib.org/doc/samtools-collate.html
