CXX = g++

AR = ar

CXXFLAGS = -O2 -g -Wall -fmessage-length=0 -Iinclude -std=c++14 -fPIC

SRCS = $(shell find src/ -type f -name *.cc)

OBJS = $(SRCS:.cc=.o)

LIBS =

LDFLAGS =

LDLIBS = -lboost_system -lboost_log -lboost_program_options

TARGET = libys_util

TARGET_SO = $(TARGET).so

TARGET_A = $(TARGET).a

TEST_FLAGS = -O2 -g -Wall -fmessage-length=0 -Iinclude -std=c++14

TEST_SRCS = $(shell find test/ -type f -name *.cc)

TEST_LIBS =

TEST_LDFLAGS =

TEST_LDLIBS = $(LDLIBS)

TESTS = $(TEST_SRCS:.cc=.test)

CHECKS = $(TESTS:.test=.check)

EXAMPLE_FLAGS = -O2 -g -Wall -fmessage-length=0 -Iinclude -std=c++14

EXAMPLE_SRCS = $(shell find examples/ -type f -name *.cc)

EXAMPLE_LIBS =

EXAMPLE_LDFLAGS =

EXAMPLE_LDLIBS = $(LDLIBS)

EXAMPLES = $(EXAMPLE_SRCS:.cc=.example)

all: $(TARGET_SO) $(TARGET_A)

$(TARGET_SO): $(OBJS)
	$(CXX) -o $(TARGET_SO) -shared $(LDFLAGS) $(LDLIBS) $(OBJS) $(LIBS)

$(TARGET_A): $(OBJS)
	$(AR) rvs $(TARGET_A) $(OBJS)

install: all
	cd include && \
		find . -type f \( -name *.h -o -name *.tcc \) \
			-exec install -v -DT -m 0644 {} /usr/local/include/{} \;
	install -v -t /usr/local/lib/ $(TARGET_A) $(TARGET_SO)

uninstall:
	cd include && \
		find . -type f \( -name *.h -o -name *.tcc \) \
			-exec rm -fv /usr/local/include/{} \;
	cd /usr/local/lib && \
		rm -fv $(TARGET_A) $(TARGET_SO)

test: $(TESTS)

%.test: %.cc $(TARGET_A)
	    $(CXX) $< -o $@ $(TEST_FLAGS) $(TEST_LDFLAGS) $(TEST_LDLIBS) $(TEST_LIBS) $(TARGET_A)

%.check: %.test
	    $<

check: $(CHECKS)

examples: $(EXAMPLES)

%.example: %.cc $(TARGET_A)
	    $(CXX) $< -o $@ $(EXAMPLE_FLAGS) $(EXAMPLE_LDFLAGS) $(EXAMPLE_LDLIBS) $(EXAMPLE_LIBS) $(TARGET_A)

clean:
	rm -f $(OBJS) $(TARGET_SO) $(TARGET_A)
	rm -f $(TESTS)
	rm -f $(EXAMPLES)

.PHONY: clean all test check

