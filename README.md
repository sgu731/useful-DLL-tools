Made these around 2020. They're very useful if you know how to use them. 


MultiClient can technically be used in every game—unless they block it with a different method.

You can drop these files directly into your project. They have no dependencies except JpgParser.

JpgParser updates old games' DLLs (especially Intel® JPEG Library) to use the modern libjpeg library, which also lets you run them on Linux.

This is great because the newer library fixes the memory leak issues that happened in older DLLs. (TL;DR It makes your game smoother)

It can't replace the library automatically, so you'll have to trace the code yourself, and hook it using DecodeFromLibJpeg function. It's easy tho.
