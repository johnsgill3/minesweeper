# Object Vars
LIBDIR = objects
LIBSRCS = $(LIBDIR)/board.cpp $(LIBDIR)/square.cpp $(LIBDIR)/solver.cpp $(LIBDIR)/utils.cpp
LIBOBJS = $(patsubst $(LIBDIR)/%.cpp,$(BLDDIR)/%.o,$(LIBSRCS))

# Game Vars
GMDIR = game
GMSRCS = $(GMDIR)/mmain.cpp
GMOBJS = $(patsubst $(GMDIR)/%.cpp,$(BLDDIR)/%.o,$(GMSRCS))
GMTGT = $(BLDDIR)/minesweeper

# AI Vars
AIDIR = solver
AISRCS = $(AIDIR)/smain.cpp
AIOBJS = $(patsubst $(AIDIR)/%.cpp,$(BLDDIR)/%.o,$(AISRCS))
AITGT = $(BLDDIR)/solver

DEPDIR = .d
BLDDIR = build
OUTDIRS = $(DEPDIR) $(BLDDIR)

CPPFLAGS = -g --std=c++1y
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

all: $(OUTDIRS) $(GMTGT) $(AITGT)
clean:
	rm -rf $(LIBOBJS) $(OUTDIRS) $(GMOBJS) $(GMTGT) $(AIOBJS) $(AITGT)

$(OUTDIRS):
	mkdir -p $(OUTDIRS)

$(GMTGT): $(GMOBJS) $(LIBOBJS)
	$(CXX) $(CPPFLAGS) -o $@  -I$(LIBDIR) $^

$(AITGT): $(AIOBJS) $(LIBOBJS)
	$(CXX) $(CPPFLAGS) -o $@  -I$(LIBDIR) $^

$(LIBOBJS): $(BLDDIR)/%.o : $(LIBDIR)/%.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(GMOBJS): $(BLDDIR)/%.o : $(GMDIR)/%.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $< -I$(LIBDIR)
	$(POSTCOMPILE)

$(AIOBJS): $(BLDDIR)/%.o : $(AIDIR)/%.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $< -I$(LIBDIR)
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(LIBSRCS)))
-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(GMSRCS)))
-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(AISRCS)))
