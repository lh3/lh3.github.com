---
layout: post
title: "Let's talk about AI review"
description: ""
category: 
tags: []
---
{% include JB/setup %}

In November 2025, the company behind the [Pangram][pangram] AI detector published a [report][pg-report] claiming 21% of ICLR 2026 reviews are *fully* AI-generated. Shen and Wang published an independent [investigation][shen-wang] and concluded that 20% of ICLR reviews and 12% of Nature Communications reviews are mostly AI-written. I skimmed the [Nature summary][pg-nature] of the Pangram report a while ago but I didn't *feel* much because I hadn't seen AI-generated reviews, until now.

In the past few months, two manuscripts from my group were likely reviewed by AI; one paper I reviewed likely received full AI comments from another reviewer. I use word "likely" for scientific accuracy because AI detectors could have false positives and my experience could be wrong. Nonetheless, I am willing to bet my money on these cases if there is a way to confirm. I am pretty sure.

### AI-generated vs AI-assisted review

I want to distinguish the two types for clarity. If the reviewer drafts the initial review on his/her own thought and then gets AI involved to improve the draft, I regard this review as AI-assisted because the reviewer has paid the due diligence. On the contrary, if the reviewer generates review with AI before careful reading as a typical reviewer, the resulting review is AI-generated even if he/she makes edits later. This is because AI has already influenced his/her decision process. The distinction is where the original review comes from, human alone or AI. I will focus on AI-generated review below. Note that although I do not use AI for paper review,  I *personally* think AI-assisted review is acceptable.

### My concerns with AI-generated review

**AI often misses big pictures**. This is not because AI lacks knowledge but because it lacks taste. In scientific research, I am often faced with many plausible and inconsistent choices that are not factually wrong. I use my experience and instinct to narrow down to a few candidates. It seems to me that AI is swamped by conflicting messages in the literature. It is often reluctant to make choices and when it makes a choice, it tends to follow the popular opinion that is easier to defend, even if it is suboptimal or even wrong.

For a test case, I uploaded three published papers on the same topic, none from me, to an AI review website. The agent gave the first paper the highest score without pointing out half of the main conclusion is wrong. The second paper got the lowest score perhaps because it is less polished in details. I, however, much prefer this one because it attacks the problem in the right angle and the conclusion is right. The third paper got a score in-between but its result is practically useless and the review agent didn't mention the key problem. I am evaluating the three papers based on the big picture. AI miserably failed here.

Partly as a result, **AI review is picky**. I feel AI always tries to say something. As it doesn't comment on the big picture, it likes to nitpick unimportant details that do not affect the main conclusion. Funnily enough, AI picked on a negligible weakness I disclosed but it probably would not have raised it if I had said nothing. Al continued to argue why it is correct in the second round. Human reviewers sometimes do that but not as bad. In the end, AI adds unnecessary work that barely improves manuscripts in many of my cases. One may argue improving small details is still beneficial but I would rather use my time more wisely.

AI is picky also because **AI lacks compassion**. When I review a paper, I often ask myself "what would I think about this comment if I were the author?". AI doesn't care. It doesn't *feel* what the authors think. It has no sense about the authors' time. In one case, AI wrote the longest review I have seen in my career but it missed the weakest link in the manuscript. That is also a big-picture problem.

[A recent article][harvard-ai-review] argues that "[on social science papers] AI-generated article reviews are now better, by every measure we could construct, than reviews written by well-compensated human experts and judged to be of unusually high quality by journal editors and authors". Well, I am not among these "authors". I think fully AI-generated review often does not reveal key issues in a manuscript but instead asks the authors to follow common clichés which may lower the quality of the manuscript. Even when AI gives technically correct comments, it at times argues over side details which unnecessarily costs significant time for both the authors and the editor. 

### Journals are banning fully AI-generated review

In fact, many journals in life sciences disallow AI-generated review:
* Springer Nature has recently (on 2026-10-15 [according to web archive][archive-nature]) put up a [new page][nature-policy] on AI review policies. It disallows "generating full review reports" with AI.
* Science detailed its AI policy in a [PDF file][science-pdf] where it states "Entering all or part of the manuscript into an LLM or other AI tool to generate or augment a review" is not allowed.
*  [Elsevier][elsevier-policy]: "AI tools may therefore only be used in a supportive capacity, for example to improve the language and structure of a review report, or to assist with a background literature search".
* Cell (under Elsevier): "Private generative AI tools may be used only in a supportive context, for example to improve language and structure of a review report".
* [Bioinformatics][bfx-policy] (a journal I often submit to): "It is not acceptable to submit manuscripts received for review to an LLM, nor to rely on an LLM or related technology for generating the review".
* [ISCB][iscb-policy]: "It is not acceptable to use LLMs or related technologies to carry out reviewing activities, such as scientific peer reviews and promotion and tenure reviews".

I am pleased with these policies but I am still concerned that **journals do not have an effective way to enforce their AI policies**. If a reviewer denies AI usage, the editorial office has no way to confirm given that all AI detectors have false positives. The latest AI also seems to mimic human writing style better. It will be more challenging to tell what text is AI generated just based on its tone and style. I predict some researchers will continue using AI for review despite of journal policies, just as some students will continue using AI for homework despite of class rules. AI won't be stopped by policies, or by humanity. I don't have a solution. I can only promise that if you see an AI-generated review, it won't come from me.

[pangram]: https://www.pangram.com/
[pg-report]: https://www.pangram.com/blog/pangram-predicts-21-of-iclr-reviews-are-ai-generated
[shen-wang]: https://arxiv.org/abs/2602.00319v2
[pg-nature]: https://www.nature.com/articles/d41586-025-03506-6
[nature-policy]: https://www.springernature.com/gp/policies/editorial-policies/using-ai-in-peer-review
[archive-nature]: https://web.archive.org/web/20260000000000*/https://www.springernature.com/gp/policies/editorial-policies/using-ai-in-peer-review
[bfx-policy]: https://academic.oup.com/bioinformatics/pages/author-guidelines
[elsevier-policy]: https://www.elsevier.com/about/policies-and-standards/generative-ai-policies-for-journals#3-for-reviewers
[iscb-policy]: https://transition.iscb.org/iscb-policy-statements/iscb-policy-for-acceptable-use-of-large-language-models
[cell-policy]: https://www.cell.com/reviewers
[science-pdf]: https://www.science.org/cms/asset/cea1081d-3aa4-42a2-96b7-72290067116a/sept26_science_journals_guidelines_ai_use.pdf
[harvard-ai-review]: https://gking.harvard.edu/peerless/
