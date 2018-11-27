CXXFLAGS = -Wall -O2 -std=c++11
CFLAGS = $(CXXFLAGS)

.PHONY: all
all: bin/print_psmb \
     bin/hap2ip \
     bin/hap2lp \
     test/analyze_Infeasible_HAP_set_of_Kashiwabara \
     bin/print_haps_sat_alpha


######################################################################
## bin
######################################################################
bin/print_psmb: src/print_psmb.c
	$(CC) $(CFLAGS) -o $@ $^

bin/print_haps_sat_alpha: src/print_haps_sat_alpha.cpp lib/home_away_pattern_sets.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/hap2ip: src/hap2ip.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/hap2lp: src/hap2lp.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^


######################################################################
## test
######################################################################
test/analyze_Infeasible_HAP_set_of_Kashiwabara: test/analyze_Infeasible_HAP_set_of_Kashiwabara.cpp lib/home_away_pattern_sets.o lib/playable_slots_set_table.o
	$(CXX) $(CXXFLAGS) -o $@ $^


######################################################################
## lib
######################################################################
lib/home_away_pattern_sets.o: lib/home_away_pattern_sets.cpp lib/home_away_pattern_sets.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lib/playable_slots_set_table.o: lib/playable_slots_set_table.cpp lib/playable_slots_set_table.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<
