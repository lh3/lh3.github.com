---
layout: post
title: "A few hours with docker"
description: ""
category: 
tags: []
---
{% include JB/setup %}

## Installing docker on Mac

With all the buzz around [docker][docker], I finally decided to give it try.
I first asked Broad sysadmins if there are machines set up for testing docker
applications. They declined my request for security concerns and suggested
[Kitematic][kitematic] for my MacBook. This means I can hardly run sequence
analyses for human. Anyway, I followed their suggestion. Kitematic turns out
to be easy to install. It found my pre-installed [VirtualBox][vb], put
a new Linux VM in it, launched a docker server inside the VM and provided a
`/usr/local/bin/docker` on my laptop that talks to the server. When I opened a
terminal from Kitematic (hot key: command-shift-T), I have a fully functional
`docker` command. You can in principle launch `docker` from other terminals,
but you need to export the right environmental variables.

## Trying prebuilt images

I ran the [busybox image][busybox] successfully. I then tried ngseasy as it is
supposed to be easily installed with `make all`. When I did that, it started to
download a 600MB image. I frowned - my laptop does not have much disk space -
but decided to wait. After this one, it started to download another 500MB
image. I killed `make all` and deleted temporary files and the virtual machine.
A 1.1GB pipeline seems too much for my small experiment (and I don't know if it
keeps downloading more).

## Building my own image

Can I build a small image if I only want to install BWA in it? I asked myself.
I then googled around and found [this post][tinyimage]. It is still too complex
for my purpose, but does give the answer: I can. With more google searches, I
learned how to build a tiny image: to use statically linked binaries. I have put
up relevant files in [lh3/bwa-docker][bd] at github. Briefly, to build and use
it locally:

	git clone https://github.com/lh3/bwa-docker.git
	cd bwa-docker
	docker build -t mybwa .
	docker run -v `pwd`:/tmp -w /tmp mybwa index MT.fa
	cat test.fq | docker run -iv `pwd`:/tmp -w /tmp mybwa mem MT.fa - > test.sam

This creates test.sam in the `bwa-docker` directory. Yes, docker naturally
reads from stdin and writes to stdout, though perhaps there are more efficient
ways to pipe between docker containers.

With files on github, I can also add [my image][bwa-dh] to [Docker Hub][dh] by
allowing Docker Hub to access my github account. You can access the image with:

	docker pull lh3lh3/bwa
	docker run -v `pwd`:/tmp -w /tmp lh3lh3/bwa index MT.fa

Is the above the typical approach to creating images? Definitely not. This way,
docker is no better than statically linked binaries. If you look at other
Dockerfiles (the file used to automatically build a docker image), you will see
the typical approach is to compile executables inside the docker VM. Images
created this way depend on "fat" base images. You have to download a base image
of hundreds of MB in size in the first place. If you have two tools built upon
different fat base images, you probably need to have both bases (is that
correct?).

## Preliminary thoughts

Docker is a bless to complex systems such as the old Apache+MySQL+PHP combo,
but is a curse to simple command line tools. For simple tools, it adds multiple
complications (security, kernel version, Dockerfile, large package,
inter-process communication, etc) with little benefit.

Bioinformatics tools are not rocket science. They are supposed to be simple. If
they are not simple, we should encourage better practices rather than live with
the problems and resort to docker. I am particularly against dockerizing
easy-to-compile tools such as velvet and bwa or well packaged tools such as
spades. Another large fraction of tools in C/C++ can be compiled to statically
linked binaries or shipped with necessary dynamic libraries (see salifish).
While not ideal, these are still better solutions than docker. Docker will be
needed for some tools with complex dependencies, but I predict most of such
tools will be abandoned by users unless they are substantially better than
other competitors, which rarely happens in practice.

PS: the only benefit of dockerizing simple tools is that we can acquire a tool
with `docker pull user/tool`, but that is really the benefit of a centralized
repository which we are lacking in our field.


[docker]: https://www.docker.com
[kitematic]: https://kitematic.com
[vb]: https://www.virtualbox.org
[busybox]: https://registry.hub.docker.com/_/busybox/
[tinyimage]: http://blog.xebia.com/2014/07/04/create-the-smallest-possible-docker-container/
[bd]: https://github.com/lh3/bwa-docker
[dh]: https://hub.docker.com
[bwa-dh]: https://registry.hub.docker.com/u/lh3lh3/bwa/
