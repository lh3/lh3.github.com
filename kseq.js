var ks_comp = {'A':'T','C':'G','G':'C','T':'A','M':'K','K':'M','Y':'R','R':'Y','V':'B','B':'V','H':'D','D':'H',
			   'a':'t','c':'g','g':'c','t':'a','m':'k','k':'m','y':'r','r':'y','v':'b','b':'v','h':'d','d':'h'};

function ks_parse(str)
{
	var i, k = 0;
	var seqs = new Array();
	str = str.replace(/\r/g, ''); // Opera uses "\r\n" while FireFox "\n". Nasty enough!
	while ((i = str.indexOf('>', k)) >= 0) {
		var o = { h:'', n:'', s:'' };
		// parse header
		k = i;
		if ((i = str.indexOf('\n', k)) < 0) break; // no seq
		o.h = str.substr(k, i - k);
		k = i + 1;
		// parse sequence
		do {
			if (str.charAt(k) == '>') break;
			if ((i = str.indexOf('\n', k)) < 0) {
				o.s += str.substr(k);
				k = str.length;
				break;
			} else o.s += str.substr(k, i - k);
			k = i + 1;
		} while (k < str.length);
		o.s = o.s.replace(/\s/g, ''); // squeeze out space, etc.
		o.n = o.h.match(/>(\S+)/).pop();
		seqs.push(o);
	}
	if (seqs.length == 0 && str.length) { // not FASTA
		var o = { h:null, n:null, s:'' };
		o.s = str.replace(/\s/g, '');
		seqs.push(o);
	}
	return seqs;
}

function ks_format(seqs, line)
{
	var i, j, out = '';
	line = line == null? 100 : parseInt(line);
	for (i = 0; i < seqs.length; ++i) {
		var o = seqs[i];
		if (o.h) out += o.h + '\n';
		for (j = 0; j < o.s.length; j += line)
			out += o.s.substr(j, line) + '\n';
	}
	return out;
}

function ks_revcomp(s)
{
	var i, t = '';
	for (i = 0; i < s.length; ++i) {
		var c = s.charAt(s.length - 1 - i);
		var d = ks_comp[c];
		t += d? d : c;
	}
	return t;
}

/************
 *
 ************/

function act_format(str)
{
	seqs = ks_parse(str);
	document.getElementById('out').value = ks_format(seqs, document.getElementById('width').value);
}

function act_reverse(str)
{
	seqs = ks_parse(str);
	for (var i = 0; i < seqs.length; ++i)
		seqs[i].s = ks_revcomp(seqs[i].s);
	document.getElementById('out').value = ks_format(seqs, document.getElementById('width').value);
}

function act_search(str, query)
{
	var x, out = '';
	if (query == null || query.length == 0) return;
	var uq = query.toUpperCase();
	seqs = ks_parse(str);
	for (var i = 0; i < seqs.length; ++i) {
		var o = seqs[i], y = '', k = 0, n = 0;
		var us = o.s.toUpperCase();
		while ((k = us.indexOf(uq, k)) >= 0) {
			y += (k+1) + ',';
			++n; ++k;
		}
		if (y.length > 0) out += o.n + "\t" + n + "\t" + y + "\n";
	}
	document.getElementById('out').value = out;
}

function act_length(str)
{
	var out = '';
	seqs = ks_parse(str);
	for (var i = 0; i < seqs.length; ++i)
		out += seqs[i].n + "\t" + seqs[i].s.length + "\n";
	document.getElementById('out').value = out;
}