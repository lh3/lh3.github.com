root=/tmp/www
zip download/jstree.zip jstree.html knhx.js menu.js canvastext.js excanvas.js
mkdir -p $root
rsync -Cavz * $root
(cd $root; \
	perl csi2ssi.pl *.html inc/*.html; \
	rm -f google*.shtml *.sh; \
	find . -name '*.html'|grep -v seq-nt|xargs rm; \
	chmod 644 *.shtml *.css *.js *.html; \
	sed s,CNVhgyOmkAEMEGS0FXzWs+t/85iDh7zUoggvg6NrHMQ=,6wo51xu6ARUrklKNbMeXjgoTCvR8I3C7/IESIFp0t10=, index.shtml > 1.shtml; \
	mv 1.shtml index.shtml; \
	)
rsync -Cavz --progress /tmp/www/* lh3lh3@frs.sourceforge.net:userweb/htdocs
