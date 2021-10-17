# License Issues

When you first get involved with open source software some of the licensing issues can seem a bit abstract. But it can really pay to get this right as soon as you can. By the time some of this matters it can be too late to do anything about it.

These issues get even more complex if you are making VCV modules, for a couple of reasons. One is that you are going to be making modules that will probably “compete” directly with other VCV modules, and this may affect your thinking about who you want to use your “stuff”. Also, most VCV modules are distributed through VCV’s Library. This introduces some extra complexity and considerations.

Often it can be a good exercise to think through different scenarios, and whether you want to allow or forbid them.

1. Do you want people to use you code to make other VCV modules?
2. Do you want someone to be make a module that looks exactly like yours?
3. Do you want someone to take your free code and make a paid product from it?
4. Are you using any third party code that requires your to license your code in a certain way?
5. If you decide you want to stop making VCV modules, do you want someone else to update your module and submit them back to the VCV library?
6. Do you want to allow other developers to use your instruction manuals?

With all open source projects, you have to think of many of these issues. VCV adds their own “rules of ethics”, and their own conventions for doing business. It’s probably easiest to think of these issues separately.

## A few basics of open-source licenses

* The default license terms are that everything in your repo belongs to you, and no-one has any right to re-use it.
* Most licenses grant some rights to re-use.
* It is very common to license code under one set of terms, and graphics under another.
* It is almost impossible to make your license more restrictive once you have published something under permissive terms.
* There are quite a few libraries that require you to have a license as permissive as theirs. Libsndfile is a classic example.
* Code licenses are completely separate from trademarks and copyright.

## A few things about VCV

* They do have a code of ethics that says even if something may be permitted legally, there is a certain code of conduct you must follow or they will not distribute your modules in the library.
* They have indicated that if you say in your license, “please don’t do this”, that they would not let someone do that.
* The “rules” of VCV are not very clear and are not all written down in one place.

## Back to open source software

Above, 1) asks “do you want people to be able to use your code to make their modules”. Most people use a permissive license that does allow this. And they in turn use other people’s code in their modules, subject to the license. So it is normal to have a license that lets people use your code for any free products, including modules.

If you don’t want to allow that, you don’t have to. But it may limit what third party code you are allowed to use.

Question 2) is more about the look of your module. Often developers don’t want a third party to copy the look of their modules. So it is very common to have a different license for your graphics and panels than for your source code, or to assert a copyright over this visual design.

A good example is VCV Rack itself. Note that there is a copyright claimed on the visual design, and it is not permitted for anyone else to build on the design of the core modules. The same is true of the Fundamental modules.

It is difficult to imagine a scenario where you want to grant the right for anyone to re-use your panel designs, and possibly your control designs too. If this is the case, make sure you license does not inadvertently give someone that right. Although if you would like to allow this in the case that you abandon the plugins, you should consider spelling that out in your license.

Regarding 3), again, many developers don’t want someone to take their code and put it in a commercial product. Yet, this has happened in the past and it the main reason that the GNU GPL 3 license is commonly used in modules developed for VCV.

## VCV written codes and conventions

Most of the written “rules” are here: https://vcvrack.com/manual/PluginLicensing. The ethics guidelines say this:

>You may not clone the brand name, model name, logo, panel design, or layout of components (knobs, ports, switches, etc) of an existing hardware or software product without permission from its owner, regardless of whether these are covered under trademark/copyright law.

>It is recommended to follow these guidelines for all plugins, but you are not legally obligated to do so. However, it is a requirement for: distributing your plugin on the VCV Library.

However on the issue of “taking over” inactive plugins, Andrew has said:

>If the original developer hasn’t responded to questions/comments in a month, I’ll consider the developer inactive. Once inactive, another developer who has made significant improvements to the plugin can adopt the project and request build updates on its VCV Library thread with the same slug/name. Of course, all IP licenses have to be followed.

It is far from clear how someone could update a module from v1 to v2 and submit it as their own to the Library, but presumably it’s possible if the original module licensed the rights to all graphics and brands. Or if the module updater changes all the branding or visual elements.

It’s difficult to tell what these policies mean in reality, but it does suggest that if you care about these issues you should speak to them in your license.

For example, you can put in your license “These panels all say `Acme Widget Co`. While not a registered trademark, I am not giving anyone a license to re-use it, and do not what others to submit modules to the library labeled `Acme Widget Co.` In addition, the brand field in my plugin.json is `Acme Widget Co`. I do not want anyone to use that. And I also consider my module slug `acmewidgetco-slug` is a part of my brand, and do not wish for anyone to use that slug in their modules.

On the other hand, you can encourage someone to adopt your plugins in case you abandon them. Do it by granting some explicit rights in your own license file.

As I say, it’s difficult to determine what VCV might do by default when presented with these scenarios, but I take them at their word that they will honor requests when possible.

## Conclusion

Until you need to change it, or want to change it, you might consider having no license, and keeping all rights to yourself.

It’s better to be explicit about what permissions you are giving people than to leave things up to chance or interpretation.

If you don’t want someone to make a knock-off of your module, make sure that your license doesn’t give someone that right. If you **do** want to allow that, make sure you license allows it.

Think carefully about what you want people to do with your source code, graphics, and text. Then make sure your license is appropriate.
