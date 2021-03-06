CXXFLAGS   = -Wall -Wextra -g -fstack-protector-all
LDFLAGS    = -lssp
EXECUTABLE = engine
EXTENSION  = cc
SOURCES    = $(basename $(shell dir /s /b *.$(EXTENSION))) #$(basename $(shell dir /b *.$(EXTENSION))) # Does not work for subfolders

.PHONY: all
all: $(EXECUTABLE)
fresh: clean all

$(EXECUTABLE): $(addsuffix .o,$(SOURCES))
	$(CXX) $^ -o $@ $(LDFLAGS)

%.d: %.gcc
	$(CXX) $(CXXFLAGS) -MM $< -o $@
	echo sed -i 's/:/ $@:/' $@
	printf '\t$$(CC) $$(CXXFLAGS) -c $$< -o $$@\n' >>$@

-include $(addsuffix .d,$(SOURCES))

.PHONY: clean
clean:
	del /s *.o
	del /s *.d
	del /s *.~
