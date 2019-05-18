---
layout: post
title: "How much does developement time matter?"
description: ""
category: 
tags: []
---
{% include JB/setup %}

I often hear developers saying "I do XYZ because it saves my time". "XYZ" could
be the selection of programming language, the use of 3rd-party libraries or
other choices in programming. When I hear this, my immediate reaction is
always: where to put users' time into the equation? Here is how I think about
it.

In *my* view, the value of a feature is *roughly* measured by
```
value(feature) = #benefitedUsers * avgUserTimeSaved - devTime
```
It is a balance between users' time and developers' time. The feature is more
valuable if it benefits more users and takes less time to implement. This
simple "equation" can be applied to a variety of use cases:

* In one extreme, I am the only benefited user for a one-off task. The user
  time is usually much less than dev time. The approach taking the least dev
  time is preferred.

* In the other extreme, installation affects all users. For a tool with a large
  user base, it is worth every bit of effort to simplify installation, even at
  the cost of significant development time.

* Most practical scenarios are something in between. It can be difficult to
  measure how many users may be benefited from a new feature. Experience in
  user interaction and familiarity with practical data analysis will play an
  important role in making the right decision.

* A software project can be improved in multiple ways. Rationally, the most
  valuable feature should be prioritized.

* Similarly, a developer or a team of developers may be working on multiple
  projects, it is preferred to work on the most valuable feature first.

Of course, in practice, the choice to implement a feature is more often
opinionated than rational. There are also other factors into play. For example,
if a developer gets paid to implement a feature (e.g. in case of LuaJIT), that
feature is likely to get a higher priority.

The above measures the value of a feature from the developer point of view.
From the user point of view, development time doesn't matter. All users want
is a product that is more convient for them. Next time when you argue
development time is important, think about the potential users.
