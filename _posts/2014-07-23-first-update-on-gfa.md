---
layout: post
title: "First update on GFA"
description: ""
category: 
tags: []
---
{% include JB/setup %}

I was out of the town in the past few days, so have not been able to focus on
GFA. Now I am back to work to give the first update on the format based on the
comments from many people, which I appreciate a lot.

In comparison to my initial proposal, the first and the major change is to name
segments instead of the ends of segments. This seems the consensus so far.
Secondly, I am thinking to move the quality field on the "S" line to an
optional tag. Not many assemblers produce quality or per-base read depth.
Thirdly, more people prefer to explictly encode bubbles as multiple segments,
rather than inline them in the sequence. I will use explicit bubbles at least
in the initial iteration.

Here is the graph from the previous post in the updated format:

    H  VN:Z:1.0
    S  1  CGATGCAA
    L  1  +  2  +  5M
    S  2  TGCAAAGTAC
    L  3  +  2  +  0M
    S  3  TGCAACGTATAGACTTGTCAC  RC:i:4
    L  3  +  4  -  1M1D2M1S
    S  4  GCATATA
    L  4  -  5  +  0M
    S  5  CGATGATA
    S  6  ATGA
    C  5  +  6  +  2  4M

A little bit more formally, GFA consists of four types of lines indicated by
the first letter at each line. The format of each line is:

    Line  Fixed fields                             Comments
	-------------------------------------------------------------
	 H    N/A                                      Header
	 S    segName,segSeq                           Segment
	 L    segName1,segOri1,segName2,segOri2,CIGAR  Link
	 C    segName1,segOri1,segName2,segOri2,CIGAR  Contained

Here is a list of predefined tags:

    Line  Tag  Type  Comments
    -----------------------------------------------------------------------
      H   VN    Z    Version number
     L/S  RC    i    # reads that support the segment/link
     L/S  FC    i    # fragments that support the segment/link
     L/C  MQ    i    Mapping quality of the overlap/containment
     L/C  NM    i    # mismatches/gaps
      S   LN    i    Segment length

In the next step, I will write a standalone parser for GFA and clean up a few
dirty corners in this process. I will also try to write a few converters for
existing assembly formats by various assemblers and implement a few basic
tools.  If you have any suggestions, please let me know. After all, I am not so
experienced in de novo assemblies as most of the readers.

Finally, I should emphasize that the format has not been fixed at all. Please
keep the comments coming. The discussions so far are very helpful to me. Thank
you!

