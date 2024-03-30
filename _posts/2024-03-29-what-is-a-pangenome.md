---
layout: post
title: "On the definition of pangenome"
description: ""
category: 
tags: []
---
{% include JB/setup %}

Historically, the term "pan-genome" (or more often "pangenome" in recent
literature) was first coined by [by Sigaux][first] in
2000 for a purpose distinct from current uses. [Victor Tetz's
definition][origin0] is fairly close to the modern definition except that it is
applied to all living organisms. These definitions are rarely cited nowadays.

In microbiome research, a pangenome refers to the non-redundant set of genes in a bacterial species.
This definition is commonly attributed to two papers ([Medini et al,
2005][origin1]; [Tettelin et al, 2005][origin2]), both with [Rino Rappuoli][rr]
as the corresponding author. In practice, most widely used tools for
bacterial pangenome construction only consider protein-coding genes.
The [pan-genome wiki page][wiki] currently focuses on bacterial pangenome
analysis, too.

When the concept of pangenome was ported to eukaryotic genomes where
protein-coding genes are sparse, pangenome initially referred to the
non-redundant set of genomic sequences in a species. This is a good definition
in the narrow sense. However, the scope of pangenome has been broadened in
recent literature. In my talks, I often cite the definition in a [review paper
in 2018][comp-review] (with slight modifications): **a pangenome refers to a
collection of well assembled genomes in a clade to be analyzed together.** Determining the
non-redundant genes or genomic sequences is one type of pangenome analysis but other
types of joint analyses such as indexing and storage also count. Notably, we do
not consider short reads from the 1000 Genomes Project as pangenome data
because these short reads cannot be well assembled.

Like many concepts in biology, the definiton of pangenome somewhat differs
between fields and evolves with time. When you see "pangenome" next time, read
the context to understand what it really means.

[comp-review]: https://pubmed.ncbi.nlm.nih.gov/27769991/
[first]: https://pubmed.ncbi.nlm.nih.gov/11261250/
[origin1]: https://pubmed.ncbi.nlm.nih.gov/16185861/
[origin2]: https://pubmed.ncbi.nlm.nih.gov/16172379/
[rr]: https://en.wikipedia.org/wiki/Rino_Rappuoli
[pm-search]: https://pubmed.ncbi.nlm.nih.gov/?term=%22pangenome%22+OR+%22pan-genome%22%5Btiab%5D&filter=years.1982-2005
[origin0]: https://pubmed.ncbi.nlm.nih.gov/15990697/
[wiki]: https://en.wikipedia.org/wiki/Pan-genome
