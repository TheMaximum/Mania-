---
title: Versioning and Branching | Mania++
layout: "page"
---
# Versioning #
The versioning system uses three numbers to define the version of the software:

* Major: system overhaul/big features added
* Minor: added feature(s)
* Patch: bugfix release

Which results in Major.Minor.Patch (f.e. 1.2.3).
A software version is represented by a tag on the Git repository, which is also linked to a commit and contains archive files.

# Branching #
The Git repository contains two main branches:

* Master: latest completely working version (which is tagged)
* Develop: continuous development is taking place

It is possible to create seperate branches for specific features if it's felt that is the correct way to go.
