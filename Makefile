SDIR = src
ODIR = bin
CXXFILES = time_series
CXX = g++
CXXFLAGS = -std=c++11 -O3 -fcilkplus
LDFLAGSS = -lm

TARGET = $(ODIR)/main

OFILES = $(patsubst %, $(ODIR)/%.o, $(CXXFILES))


.PHONY: all clean

tmp := $(shell mkdir -p $(ODIR))

all: $(TARGET)

$(ODIR)/%.o: $(SDIR)/%.cc
	@echo [CXX] $< "-->" $@
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OFILES)
	@echo [LD} $< "-->" $@
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	@echo cleaning up
	@rm -rf $(ODIR)
