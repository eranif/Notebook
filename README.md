# Notebook
A modern notebook control built for performance that does not flicker for wxWidgets

**Why not creating a new art provider for wxAuiNotebook ?**

The reasons are variying from

 -  The wxAui Art provider does not allow tabs to overlap, this limits the capabilities of tabs drawings
 - Buffered drawing is not always used, this causes flicker (mainly noticeable on Linux) when changing or closing tabs

Here is how the current `Notebook` looks like:



![Alt text](/resources/notebook.png?raw=true "Notebook")
