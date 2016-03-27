SDIR = src
ODIR = bin
CXXFILES = main time_series linear_regression
CXX = g++
ifeq ($(TRAVIS), 1)
	CXX = g++-5
endif
CXXFLAGS = -DTESTING -std=c++11 -O3 -fcilkplus -I googletest/googletest/include --coverage
LDFLAGS = googletest/googlemock/gtest/libgtest.a googletest/googlemock/gtest/libgtest_main.a -lm -lgcov

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
