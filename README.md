heron
=====

## What is it?
A tool for reading the [UDC Summary](http://www.udcc.org/udcsummary) offline. It shall be useful for people organizing their libraries with the UDC System (http://en.wikipedia.org/wiki/Universal_Decimal_Classification).

## What is UDC?
UDC means Universal Decimal Classification and is a popular way of classifying books and other bibliographical material.
It is based on the Dewey Decimal Classification in the sense that categories are grouped by the first significant figures of the number.
Yet the UDC adds auxiliary signs and numbers that express relationships between classes and other general properties of the bibliography, such as the language it was written.
For more information, visit [wikipedia](http://en.wikipedia.org/wiki/Universal_Decimal_Classification).

## Who makes this classification?
The [UDC Consortium](http://udcc.org/) is the organization that maintains the UDC.
It publishes a summary with selected classes and also publishes [linked data for this summary](http://udcdata.info/) upon which this software is coded.

## How do I make this source work?
At first, you need compiler tools, e.g. gcc and gnu make.
Another prerequisite is the [libxml-2.0](http://xmlsoft.org/) developer tools.
Finally, you need the rdf file containing the information used by the program available zipped on http://udcdata.info/udcsummary-skos.zip .
The `Makefile` already downloads it as a convenience.

Clone this repository, type `make` on your terminal and if everything is successful, the program shall be compiled and executed.

## What are the next steps planned?
Some of my ideas are:
- Translate the source to English and internationalize the program.
- Interface with command-line arguments.
- Better interaction with the user, with line history.
- Interprete coordination specified by auxiliary signs.
- Add Cutter-Sanborn three figure table.

## How is this source licensed?
Since it is based on work from UDC Consortium, the license used here is under Creative Commons License Attribution-ShareAlike. The UDC Consortium made the rdf file. The program written by me reads and displays this rdf file and is licensed under the same license.

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
