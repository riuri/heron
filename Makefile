CFLAGS += `pkg-config --cflags libxml-2.0`
LDLIBS += `pkg-config --libs libxml-2.0`

test: heron
	./heron

heron:

.PHONY: teste
