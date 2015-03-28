
BUILD=build


SOURCES = Hashes.java Mac.java
CLASSES = $(SOURCES:%.java=$(BUILD)/%.class)

Q=@
ifdef V
Q=
endif

all: build $(CLASSES)
	@echo Done

build:
	$(Q)mkdir -p $(BUILD)

$(BUILD)/%.class: %.java
	javac $< -d $(BUILD)

clean:
	rm -fr $(BUILD)

