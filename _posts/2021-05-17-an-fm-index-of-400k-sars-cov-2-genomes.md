---
layout: post
title: "An FM-index of 400k SARS-CoV-2 genomes"
description: ""
category: 
tags: []
---
{% include JB/setup %}

Leonardo Martins [tweeted][tweet] that xz can compress a 1.4 million SARS-CoV-2
genomes in a 39GB FASTA down to 74MB. That is a very impressive compression
ratio! This reminds me of my earlier work on [FM-index construction][rb2-pmid].

For an experiment, I downloaded ~400k SARS-CoV-2 genomes from EBI's [COVID-19
data portal][cdp] ([GISAID][gisaid] has ~1.5M genomes but imposes
restrictions) and generated an FM-index of these sequences in both strands
with [ropebwt2][rb2]
```
ropebwt2 -do sars-cov-2.fmd sequences_fasta_2021-05-15.fa.gz
```
The command line took ~30 minutes. The output file `sars-cov-2.fmd` is 33MB in
size.  It keeps the BWT and the necessary information for backward/forward
search. You can find this file [at Zenodo][dl].

Here are a few things you can do with this file, using
[fermi2][fm2]:
```
# uncompress the FM-index; forward and reverse strands are interleaved
fermi2 unpack sars-cov-2.fmd | less -S

# count 61-mers occurring 10 times or more using 4 threads (5 sec on my laptop)
fermi2 count -k61 -o10 -t4 sars-cov-2.fmd | less

# count how many times a sequence in FASTA occurs in the FM-index
fermi2 match sars-cov-2.fmd query.fa

# get the count of every 61-mer in query sequences
fermi2 kprof -k61 sars-cov-2.fmd query.fa

# find supermaximal exact matches (SMEMs)
fermi2 match -p sars-cov-2.fmd query.fa

# find SMEMs at least 200bp occurring 1000 times or more, using 8 threads
fermi2 match -pt8 -l200 -n1000 sars-cov-2.fmd

# generate sampled suffix array and output positions
fermi2 sa -t8 sars-cov-2.fmd > sars-cov-2.fmd.sa  # slooow
fermi2 match -pm1000 -s sars-cov-2.fmd.sa sars-cov-2.fmd query.fa # can be very sloow
```
Although the original input sequences are totaled 12GB in length (or 24GB if
we consider both strands), all but the last operations take ~33MB in RAM, the
size of the index. That is the advantage of FM-index or similar indices.

PS: I don't study SARS-CoV-2 genomes. I did the above for fun only. Let me know
if you feel some of these might be useful to your research and want to learn
more.

[tweet]: https://twitter.com/leomrtns/status/1393315682352250881
[rb2-pmid]: https://pubmed.ncbi.nlm.nih.gov/25107872/
[cdp]: https://www.covid19dataportal.org/
[gisaid]: https://www.gisaid.org/
[rb2]: https://github.com/lh3/ropebwt2
[fm2]: https://github.com/lh3/fermi2
[dl]: https://zenodo.org/record/4771285
