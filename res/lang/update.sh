#!/bin/sh
TEMPLATE=Clevy.pot

echo Updating translation template $TEMPLATE ...
xgettext --omit-header --no-location --keyword=_ --language=C++ --output=$TEMPLATE ../../src/*.cpp ../../src/*.h

echo Found translations: *.po
for f in *.po; do
	echo Updating translation $f ...
	msgmerge -U --no-fuzzy-matching $f $TEMPLATE
done
