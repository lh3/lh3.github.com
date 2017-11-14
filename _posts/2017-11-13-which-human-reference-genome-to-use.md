---
layout: post
title: "Which human reference genome to use?"
description: ""
category: 
tags: []
---
{% include JB/setup %}

TL;DR: If you map reads to GRCh37 or hg19, use `hs37-1kg`:
```
ftp://ftp-trace.ncbi.nih.gov/1000genomes/ftp/technical/reference/human_g1k_v37.fasta.gz
```
If you map to GRCh37 and believe decoy sequences help with better variant calling, use `hs37d5`:
```
ftp://ftp-trace.ncbi.nih.gov/1000genomes/ftp/technical/reference/phase2_reference_assembly_sequence/hs37d5.fa.gz
```
If you map reads to GRCh38 or hg38, use the following:
```
ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCA/000/001/405/GCA_000001405.15_GRCh38/seqs_for_alignment_pipelines.ucsc_ids/GCA_000001405.15_GRCh38_no_alt_analysis_set.fna.gz
```

There are several other versions of GRCh37/GRCh38. What's wrong with them? Here
are a collection of potential issues:

1. Inclusion of ALT contigs. ALT contigs are large variations with very long
   flanking sequences nearly identical to the primary human assembly. Most read
   mappers will give mapping quality zero to reads mapped in the flanking
   sequences. This will reduce the sensitivity of variant calling and many
   other analyses. You can resolve this issue with an ALT-aware mapper, but
   no mainstream variant callers or other tools can take the advantage of
   ALT-aware mapping.

2. Padding ALT contigs with long "N"s. This has the same problem with 1 and
   also increases the size of genome unnecessarily. It is worse.

3. Inclusion of multi-placed sequences. In both GRCh37 and GRCh38, the
   pseudo-autosomal regions (PARs) of chrX are also placed on to chrY. If you
   use a reference genome that contains both copies, you will not be able to
   call any variants in PARs with a standard pipeline. In GRCh38, some
   alpha satellites are placed multiple times, too. The right solution is to
   hard mask PARs on chrY and those extra copies of alpha repeats.

4. Not using the [rCRS][2] mitochondrial sequence. rCRS is widely used in
   population genetics. However, the official GRCh37 comes with a mitochondrial
   sequence 2bp longer than rCRS. If you want to analyze mitochondrial
   phylogeny, this 2bp insertion will cause troubles. GRCh38 uses rCRS.

5. Converting semi-ambiguous [IUB codes][3] to "N". This is a very minor issue,
   though. Human chromosomal sequences contain few semi-ambiguous bases.

6. Using accession numbers instead of chromosome names. Do you know
   [CM000663.2][7] corresponds to chr1 in GRCh38?

7. Not including unplaced and unlocalized contigs. This will force reads
   originated from these contigs to be mapped to the chromosomal assembly and
   lead to false variant calls.

Now we can explain what is wrong with other versions of human reference genomes:

* hg19/chromFa.tar.gz [from UCSC][4]: 1, 3, 4 and 5.
* hg38/hg38.fa.gz [from UCSC][1]: 1, 3 and 5.
* GCA_000001405.15_GRCh38_genomic.fna.gz [from NCBI][5]: 1, 3, 5 and 6.
* Homo_sapiens.GRCh38.dna.primary_assembly.fa.gz [from EnsEMBL][6]: 3.
* Homo_sapiens.GRCh38.dna.toplevel.fa.gz [from EnsEMBL][6]: 1, 2 and 3.

Using an impropriate human reference genome is usually not a big deal unless
you study regions affected by the issues. However, 1) other researchers may be
studying in these biologically interesting regions and will need to redo
alignment; 2) aggregating data mapped to different versions of the genome will
amplify the problems. It is still preferable to choose the right genome version
if possible.

Well, welcome to bioinformatics!

[1]: http://hgdownload.soe.ucsc.edu/goldenPath/hg38/bigZips/
[2]: http://en.wikipedia.org/wiki/Cambridge_Reference_Sequence
[3]: http://biocorp.ca/IUB.php
[4]: http://hgdownload.soe.ucsc.edu/goldenPath/hg19/bigZips/
[5]: http://www.ncbi.nlm.nih.gov/projects/genome/guide/human/
[6]: http://ftp.ensembl.org/pub/current_fasta/homo_sapiens/dna/
[7]: https://www.ncbi.nlm.nih.gov/nuccore/568336023
