---
layout: post
title: "Fast high-level programming languages"
description: ""
category: 
tags: []
---
{% include JB/setup %}

### Background

Python and R are slow when they can't rely on functionality or libraries backed
by C/C++. They are inefficient not only for certain algorithm development but
also for common tasks such as FASTQ parsing. Using these languages limits the
reach of biologists. Sometimes you may have a brilliant idea but can't deliver
a fast implementation only because of the language in use. This can be
frustrating. I have always been searching for a [high-level language][hllang]
that is fast and easy to use by biologists. This blog post reports some of my
exploration. It is inconclusive but might still interest you.

### Design

Here I am implementing two tasks, FASTQ parsing and interval overlap query, in
several languages including C, Python, Javascript, [LuaJIT][luajit],
[Julia][julia], [Nim][nim], and [Crystal][crystal]. I am comparing their
performance. I am proficient in C and know Python a little. I have used LuaJIT
and Javascript for a few years. I am equally new to Julia, Nim and Crystal.  My
implementations in these languages may not be optimal. Please keep this
important note in mind when reading the results.

### Results

The source code and the full table are available at my [lh3/biofast][biofast]
github repo. You can also found the machine setup, versions of libraries in
use and some technical notes. I will only show part of the results here.

#### FASTQ parsing

The following table shows the CPU time in seconds for parsing a gzip'd FASTQ
(t<sub>gzip</sub>) or a plain FASTQ (t<sub>plain</sub>). We only count the
number of sequences and compute the sum of lengths. Except the two BioPython
implementations, all the others use an algorithm similar to my [kseq.h][kseq]
parser in C.

<style> .extable td,th { padding: 4px; } </style>
<table border="1" class="extable">
<tr><th>Language</th><th>Ext. Library</th><th>t<sub>gzip</sub> (s)</th><th>t<sub>plain</sub> (s)</th><th>Comments</th></tr>
<tr><td>C         </td><td>         </td><td style="text-align:right">  9.7</td><td style="text-align:right">  1.4</td><td></td> </tr>
<tr><td>Crystal   </td><td>         </td><td style="text-align:right">  9.7</td><td style="text-align:right">  1.5</td><td></td> </tr>
<tr><td>Nim       </td><td>         </td><td style="text-align:right"> 10.5</td><td style="text-align:right">  2.3</td><td></td> </tr>
<tr><td>Rust      </td><td>rust-bio </td><td style="text-align:right"> 10.6</td><td style="text-align:right">  2.9</td><td>4-line only</td> </tr>
<tr><td>Julia     </td><td>         </td><td style="text-align:right"> 11.2</td><td style="text-align:right">  2.9</td><td></td> </tr>
<tr><td>Python    </td><td>PyFastx  </td><td style="text-align:right"> 15.8</td><td style="text-align:right">  7.3</td><td>C binding</td> </tr>
<tr><td>Javascript</td><td>         </td><td style="text-align:right"> 17.5</td><td style="text-align:right">  9.4</td><td>k8 dialect</td> </tr>
<tr><td>Go        </td><td>         </td><td style="text-align:right"> 19.1</td><td style="text-align:right">  2.8</td><td>4-line only</td> </tr>
<tr><td>LuaJIT    </td><td>         </td><td style="text-align:right"> 28.6</td><td style="text-align:right"> 27.2</td><td></td> </tr>
<tr><td>PyPy      </td><td>         </td><td style="text-align:right"> 28.9</td><td style="text-align:right"> 14.6</td><td></td> </tr>
<tr><td>Python    </td><td>BioPython</td><td style="text-align:right"> 37.9</td><td style="text-align:right"> 18.1</td><td>FastqGeneralIterator</td> </tr>
<tr><td>Python    </td><td>         </td><td style="text-align:right"> 42.7</td><td style="text-align:right"> 19.1</td><td></td> </tr>
<tr><td>Python    </td><td>BioPython</td><td style="text-align:right">135.8</td><td style="text-align:right">107.1</td><td>SeqIO.parse</td> </tr>
</table>

This benchmark stresses on I/O and string processing. I replaced the low-level
I/O of several languages to achieve good performance. The code looks more like
C than a high-level language, but at least these langauges give me the power
without resorting to C.

It is worth mentioning the default BioPython FASTQ parser is over 70 times
slower on plain FASTQ and over 10 times slower on gzip'd FASTQ. Running the C
implementation on a human 30X gzip'd FASTQ takes 20 minutes. The default
BioPython parser will take four and half hours, comparable to bwa-mem2
multi-thread mapping. If you want to parse FASTQ but doesn't need other
BioPython functionality, choose [PyFastx][pyfx] or mappy.

#### Interval overlap query

The following table shows the CPU time in seconds for computing the breadth of
coverage of a list intervals compared against another interval list. There are
two columns for timing and memory footprint, depending on which list is loaded
into memory.

<table border="1" class="extable">
<tr><th>Language</th><th>t<sub>g2r</sub> (s)</th><th>M<sub>g2r</sub> (Mb)</th><th>t<sub>r2g</sub> (s)</th><th>M<sub>r2g</sub> (Mb)</th></tr>
<tr><td>C         </td><td style="text-align:right">  5.2</td><td style="text-align:right"> 138.4</td><td style="text-align:right"> 10.7</td><td style="text-align:right"> 19.1</td></tr>
<tr><td>Crystal   </td><td style="text-align:right">  9.0</td><td style="text-align:right"> 319.6</td><td style="text-align:right"> 17.4</td><td style="text-align:right"> 40.1</td></tr>
<tr><td>Nim       </td><td style="text-align:right"> 16.6</td><td style="text-align:right"> 248.4</td><td style="text-align:right"> 26.0</td><td style="text-align:right"> 34.1</td></tr>
<tr><td>Julia     </td><td style="text-align:right"> 25.9</td><td style="text-align:right"> 428.1</td><td style="text-align:right"> 63.0</td><td style="text-align:right">257.0</td></tr>
<tr><td>Go        </td><td style="text-align:right"> 34.0</td><td style="text-align:right"> 318.9</td><td style="text-align:right"> 21.8</td><td style="text-align:right"> 47.3</td></tr>
<tr><td>Javascript</td><td style="text-align:right"> 76.4</td><td style="text-align:right">2219.9</td><td style="text-align:right"> 80.0</td><td style="text-align:right">316.8</td></tr>
<tr><td>LuaJIT    </td><td style="text-align:right">174.1</td><td style="text-align:right">2668.0</td><td style="text-align:right">217.6</td><td style="text-align:right">364.6</td></tr>
<tr><td>PyPy      </td><td style="text-align:right">17332.9</td><td style="text-align:right">1594.3</td><td style="text-align:right">5481.2</td><td style="text-align:right">256.8</td></tr>
<tr><td>Python      </td><td style="text-align:right">&gt;33770.4</td><td style="text-align:right">2317.6</td><td style="text-align:right">&gt;20722.0</td><td style="text-align:right">313.7</td></tr>
</table>

The implementation of this algorithm is straightforward. It is mostly about
random access to large arrays. Javascript and LuaJIT are much slower here
because I can't put objects in an array; I can only put references to objects
in an array.

### My take on fast high-level languages

The following is subjective and can be controversial, but I need to speak it
out. Performance is not everything. Some subtle but important details are only
apparent to those who write these programs.

#### Javascript and LuaJIT

These are two similar languages. They are old and were not designed with
[Just-In-Time][jit] (JIT) compilation in mind. People later developed JIT
compilers and made them much faster. I like the two languages. They are easy to
use, have few performance pitfalls and are pretty fast. Nonetheless, they are
not the right languages for bioinformatics. If they were, they would have
prevailed years ago.

#### Julia

Among the three more modern languages Julia, Nim and Crystal, Julia reached 1.0
first. I think Julia could be a decent replacement of Matlab or R by the
language itself. If you like the experience of Matlab or R, you may like Julia.
It has builtin matrix support, 1-based coordinate system, friendly [REPL][repl]
and an emphasis on plotting as well. I heard its differential equation solver might be
the best across all languages.

I don't see Julia a good replacement of Python. Julia has a long startup time.
When you use a large package like Bio.jl, Julia may take 30 seconds to compile
the code, longer than the actual running time of your scripts. You may not feel
it is fast in practice. Actually in my benchmark, Julia is not really as fast
as other languages, either. Probably my Julia implementations here will get
most slaps. I have seen quite a few you-are-holding-the-phone-wrong type of
responses from Julia supporters. Also importantly, the Julia developers do not
value backward compatibility. There may be a python2-to-3 like transition in
several years if they still hold their views by then. I wouldn't take the risk.

#### Nim

Nim reached its maturity in September 2019. Its syntax is similar to python on
the surface, which is a plus. It is relatively easier to get descent
performance out of Nim. I have probably spent least time on learning Nim but I
can write programs faster than in Julia.

On the down side, writing Nim programs feels a little like writing Perl in that
I need to pay extra attention to reference vs value. For the second task, my
initial implementation was several times slower than the Javascript one, which
is unexpected. Even in the current program, I still don't understand why the
performance get much worse if I change by-reference to by-value in one instance.
Nim supporters advised me to run a profiler. I am not sure biologists would
enjoy that.

#### Crystal

Crystal is a pleasant surprise. On the second benchmark, I got a fast
implementation on my first try. I did take a detour on FASTQ parsing when I
initially tried to use Crystal's builtin buffered reader, but again I got
C-like performance immediately after I started to manage buffers by myself.

Crystal resembles Ruby a lot. It has very similar syntax, including a
class/[mixin][mixin] system familiar to modern programmers. Some elementary tutorials
on Ruby are even applicable to Crystal. I think building on top of successful
languages is the right way to design a new language. Julia on the other hand
feels different from most mainstream languages like C++ and Python. Some of its
key features haven't stood the test of time and may become frequent sources of
bugs and performance traps.

To implement fast programs, we need to care about reference vs value. Crystal
is no different. The good thing about Crystal is that reference and value are
explicit with its class system. Among Julia, Nim and Crystal, I feel most
comfortable with Crystal.

Crystal is not without problems. First, it is hard to install Crystal without
the root permission. I am providing a portable installation binary package in
[lh3/PortableCrystal][portcr]. It alleviates the issue for now. Second, Crystal is
unstable. Each release introduces multiple breaking changes. Your code written
today may not work later. Nonetheless, my program seems not affected by
breaking changes in the past two years. This has given me some confidence. The
Crystal devs also said 1.0 is coming "[in the near future][cr1.0]". I will look
forward to that.

### Conclusions

A good high-level high-performance programming language would be a blessing to
the field of bioinformatics. It could extend the reach of biologists, shorten
the development time for experienced programmers and save the running time of
numerous python scripts by many folds. However, no languages are good enough in
my opinion. I will see how Crystal turns out. It has potentials.

### Anecdote

Someone posted this blog post to [Hacker New][hn], [Crystal
subreddit][redditcr], and [Julia discourse][jldis]. The reaction from many
Julia supporters is just as I expected: they blame me for falling into traps
set by Julia and ignore that my first Nim/Crystal attempts are far more
successful. That said, I owe a debt of gratitude to [Kenta Sato][bicycle1885]
for improving my Julia implementation. I geniunely appreciate.

**Update on 2020-05-19:** Added contributed Go implementations. More accurate
timing for fast implementations, measured by [hyperfine][hyperfine].

**Update on 2020-05-20:** Added a contributed Rust implementation. Added PyPy.

**Update on 2020-05-21:** Faster Nim and Julia with [memchr][memchr]. Faster
Julia by adjusting [three additional lines][fastjl]. For gzip'd input,
Julia-1.4.1 is slow due to [a misconfiguration][jlzlib] on the Julia end. The
numbers shown in the table are acquired by forcing Julia to use the system
zlib on CentOS7. Added Python bedcov implementation. It is slow.

[julia]: https://en.wikipedia.org/wiki/Julia_(programming_language)
[nim]: https://en.wikipedia.org/wiki/Nim_(programming_language)
[crystal]: https://en.wikipedia.org/wiki/Crystal_(programming_language)
[luajit]: http://luajit.org/
[hllang]: https://en.wikipedia.org/wiki/High-level_programming_language
[biofast]: https://github.com/lh3/biofast
[kseq]: https://github.com/lh3/biofast/blob/master/lib/kseq.h
[pyfx]: https://github.com/lmdu/pyfastx
[jit]: https://en.wikipedia.org/wiki/Just-in-time_compilation
[repl]: https://en.wikipedia.org/wiki/Read-eval-print_loop
[md]: https://en.wikipedia.org/wiki/Multiple_dispatch
[portcr]: https://github.com/lh3/PortableCrystal
[cr1.0]: https://crystal-lang.org/2020/03/03/towards-crystal-1.0.html
[mixin]: https://en.wikipedia.org/wiki/Mixin
[hyperfine]: https://github.com/sharkdp/hyperfine
[memchr]: http://man7.org/linux/man-pages/man3/memchr.3.html
[fastjl]: https://github.com/lh3/biofast/pull/7
[jlzlib]: https://github.com/JuliaPackaging/Yggdrasil/pull/1051
[redditcr]: https://www.reddit.com/r/crystal_programming/comments/gm2dps/crystal_in_bioinformatics_comparison_fast/
[hn]: https://news.ycombinator.com/item?id=23229657
[jldis]: https://discourse.julialang.org/t/lhe-biofast-benchmark-fastq-parsing-julia-nim-crystal-python/39747
[bicycle1885]: https://github.com/bicycle1885
