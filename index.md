---
layout: page
title: Homepage
---
{% include JB/setup %}

### Posts [![](images/feed-icon-14x14.png)](rss.xml)

<ul class="posts">
  {% for post in site.posts %}
    <li><span>{{ post.date | date_to_string }}</span> &raquo; <a href="{{ BASE_PATH }}{{ post.url }}">{{ post.title }}</a></li>
  {% endfor %}
</ul>

### Links

* [My Homepage][home] at SourceForge.net
* [My GitHub page][github]
* [My Twitter page][twitter]
* [My Publications][pub]
* [My Google Scholar page][gsch]

[home]: http://lh3lh3.users.sourceforge.net
[github]: https://github.com/lh3
[twitter]: https://twitter.com/lh3lh3
[pub]: http://www.researcherid.com/rid/D-9344-2011
[gsch]: http://scholar.google.com/citations?user=HQv0p0kAAAAJ&hl=en
