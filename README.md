# Notebook
A modern notebook control built for performance that does not flicker for wxWidgets

**Why not creating a new art provider for wxAuiNotebook ?**

The reasons are varying from

 - The wxAui Art provider does not allow tabs to overlap, this limits the capabilities of tabs drawings
 - Buffered drawing is not always used, this causes flicker (mainly noticeable on Linux) when changing or closing tabs
 - No matter what, the active tab is awlays visible


The current implementation offers two built-in styles:

 - Dark Tabs 
 - Light Tabs

Here is how the current `Notebook` looks like:

![Alt text](/resources/notebook.png?raw=true "Notebook")

# Usage

To start using the `Notebook` class, simply include the following files in your project:

 - `Notebook.cpp`
 - `Notebook.h`
 - `notebook_bitmaps.cpp`
 
**Prerequisites**

 - wxWidgets 3.0.X 
 - A decent compiler that supports `C++11`
 
 