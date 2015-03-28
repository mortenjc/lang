
BUILD=build

#CP = -classpath .:../jars/junit-4.10.jar:../jars/stdlib.jar

#SOURCES = Deque.java MinPQ.java ResizingArrayQueue.java
SOURCES = Histogram.java Sim2.java
CLASSES = $(SOURCES:%.java=$(BUILD)/%.class)

Q=@
ifdef V
Q=
endif

all: build $(CLASSES)
	@echo Done, run as java Sim2 x y

build:
	$(Q)mkdir -p $(BUILD)

$(BUILD)/%.class: %.java
	javac $< -d $(BUILD)

clean:
	rm -fr $(BUILD)

