SDIR = src
ODIR = bin
CXXFILES = main time_series linear_regression price_simulation gbm_simulation sqrt_diffusion_simulation model_loader stock risk_measures simulator gbm_weighted
CXX = g++

ifneq "$(shell which g++-5)" ""
	CXX = g++-5
endif

CXXFLAGS = -std=c++11 -fcilkplus -I googletest/googletest/include  -I src/include --coverage -Wall -Werror
LDFLAGS = googletest/googlemock/gtest/libgtest.a googletest/googlemock/gtest/libgtest_main.a -lm -lconfig++
ifeq ($(TESTING), 1)
	CXXFLAGS += -DTESTING --coverage -O1
else
	CXXFLAGS += -O3
endif

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
