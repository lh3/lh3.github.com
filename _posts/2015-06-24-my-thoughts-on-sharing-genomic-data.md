---
layout: post
title: "My thoughts on sharing genotype and phenotype data"
description: ""
category: 
tags: []
---
{% include JB/setup %}

Today, Google and Broad Institute (my employer) have [announced][news] that
they are teaming up to tackle genomic data. One sentence caught my attention:
"Broad Institute has ... either sequenced or genotyped the equivalent of more
than 1.4 million biological samples". Can we get the data?

## Current data sharing model

In my limited experience, the current data sharing model is largely
trust-and-distribute. Principle investigators (PIs) submit a form stating they
will use controlled data properly. Upon approval, they can download (usually
[de-identified][deiden]) individual genotypes and phenotypes and analyze
locally.

In my view, this model has two issues. Firstly, it is insecure. The model puts
all the trust on PIs and their PhDs/Postdocs/staffs. If one of them discloses
confidential data, there is a breach. Secondly, it hampers data sharing.  PIs
may need to access multiple data sets. Getting approval for all
of them is not trivial. In addition, many more researchers are not qualified to
access data at all. For example, some sensitive data are not allowed to be
transferred out of the border of a country.

## Data access patterns

To address the issues, we need to first understand how we use genotype and
phenotype data. It seems to me that we often, though not always, care about
the aggregate of genotypes instead of indivial genotypes. We download
genotype/phenotype data only to collect aggregate information with scripting.
For example, we may ask which gene in cases have high loads of rare mutations
or whether a particular SNP has high frequency in cases but low in controls or
some statistics stratified by a phenotype (e.g. BMI or populations). Having
aggregate data available is already very helpful.

## A different data sharing model

So, here is my thought: a better data sharing model is to let users publicly
query the aggregate statistics of their interest while hiding individual-level
data and keeping samples unidentifiable. The server sees all the genotypes and
phenotypes, processes the data and returns the aggregate to users.

How to make sure samples are unidentifiable is not easy. Here are a couple of
ideas. We may disallow query of frequency data among a small number of samples.
With one query, users would not know what sample has a particular allele.
However, depending on the structure of phenotypes, there is a small chance that
users may be able to infer the genotypes of a particular sample by performing
multiple queries. A stronger idea is to cluster samples based on phenotypes and
for each cluster, to use the median values to represent all samples in the
cluster. Users will not get individual-level details beyond a cluster.

## Implementing the new model

To make this new data sharing model something real, we need a highly flexible
and performant genotype server that computes user-defined aggregate on the fly.
It should at least match the expressiveness of SQL queries like:

	SELECT v.chrom,v.pos FROM Variant v, Sample s, Genotype g
	    WHERE v.gene="BRCA1" AND s.pop="CEU" AND g.vid=v.vid AND g.sid=s.sid
	    GROUPED BY g.vid HAVING maf(g.gt)>0.01

The key requirement is to have users flexibly defining their own ways to slice
and aggregate data. Precomputing a few summary statistics is a good start, but
is inadequate for complex use cases.

## Implementation is not enough

The idea of sharing aggregate data publicly will remain an idea unless
authorities such as IRB approves to share data this way. It is very hard, near
to impossible, but I do believe that this direction in general will make
data sharing more open, efficient, convenient and secure.

## A final comment

Many researchers are screaming for more samples, but in fact we already have
probably a couple of million genotyped/sequenced samples world wide. These
data are just locked up in attic. It is a pity. I have thoughts, but do not
have practical solutions.

[news]: http://googlecloudplatform.blogspot.com/2015/06/Google-Genomics-and-Broad-Institute-Team-Up-to-Tackle-Genomic-Data.html
[dbgap]: http://www.ncbi.nlm.nih.gov/gap
[deiden]: https://en.wikipedia.org/wiki/De-identification
[grabblur]: http://www.ncbi.nlm.nih.gov/pubmed/?term=25055742
