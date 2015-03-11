CFLAGS += `pkg-config --cflags libxml-2.0`
LDLIBS += `pkg-config --libs libxml-2.0`
LDLIBS += -lreadline

test: heron udcsummary-skos.rdf
	./heron

udcsummary-skos.zip:
	wget -c http://udcdata.info/udcsummary-skos.zip

udcsummary-skos.rdf: udcsummary-skos.zip
	unzip udcsummary-skos.zip
	touch udcsummary-skos.rdf

heron:

.PHONY: teste
