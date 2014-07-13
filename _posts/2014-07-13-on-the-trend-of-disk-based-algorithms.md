---
layout: post
title: "On the trend of disk-based algorithms"
description: ""
category: 
tags: []
---
{% include JB/setup %}

I am looking for a good k-mer counter to replace the slow k-mer counting phase
in fermi2. A quick survey reveals that except Jellyfish and BFCounter, most
k-mer counters are disk-based, in the sense that they heavily rely on temporary
files to reduce memory.

This is not a good trend in my view. In large institutions and universities
with centralized computing resources, the majority of file systems are sitting
over network. Network I/O is frequently slow. This is becoming even worse in
the production setting when multiple instances of a tool are running at the
same time and all putting heavy stress on I/O. In the end, the practical
performance of disk-based tools may be far from the optimal numbers shown in
the papers.

I would encourage the development of in-memory algorithms. Disks are not really
a good solution to high memory.
