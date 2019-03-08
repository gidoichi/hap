CXXFLAGS = -Wall -O2 -std=c++11
CFLAGS = $(CXXFLAGS)
objs = bin/print_psmb \
       bin/hap2ip \
       bin/hap2lp \
       bin/haprect2ip \
       bin/sat-alpha \
       test/analyze_Infeasible_HAP_set_of_Kashiwabara \
       test/num_of_haps \
       bin/print_haps_sat_alpha \
       bin/print_haps_sat_alpha_perm_row \
       bin/print_haps_sat_alpha_perm_row_perm_col_using_feature \
       bin/print_haps_sat_alpha_perm_row_col \
       bin/print_haps_sat_alpha_perm_row_perm_1st_col

.PHONY: all
all: $(objs)

######################################################################
## bin
######################################################################
bin/print_psmb: src/print_psmb.c
	$(CC) $(CFLAGS) -o $@ $^

bin/print_haps_sat_alpha: src/print_haps_sat_alpha.cpp lib/home_away_pattern_sets.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/print_haps_sat_alpha_perm_row: src/print_haps_sat_alpha_perm_row.cpp lib/home_away_pattern_sets.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/print_haps_sat_alpha_perm_row_perm_col_using_feature: src/print_haps_sat_alpha_perm_row_perm_col_using_feature.cpp lib/home_away_pattern_sets.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/print_haps_sat_alpha_perm_row_perm_1st_col: src/print_haps_sat_alpha_perm_row_perm_1st_col.cpp lib/home_away_pattern_sets.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/print_haps_sat_alpha_perm_row_col: src/print_haps_sat_alpha_perm_row_col.cpp lib/home_away_pattern_sets.o
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/hap2ip: src/hap2ip.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/hap2lp: src/hap2lp.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/haprect2ip: src/haprect2ip.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/sat-alpha: src/sat-alpha.cpp lib/home_away_pattern_sets.o
	$(CXX) $(CXXFLAGS) -o $@ $^


######################################################################
## test
######################################################################
test/analyze_Infeasible_HAP_set_of_Kashiwabara: test/analyze_Infeasible_HAP_set_of_Kashiwabara.cpp lib/home_away_pattern_sets.o lib/playable_slots_set_table.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test/num_of_haps: test/num_of_haps.cpp lib/home_away_pattern_sets.o
	$(CXX) $(CXXFLAGS) -o $@ $^


######################################################################
## lib
######################################################################
lib/home_away_pattern_sets.o: lib/home_away_pattern_sets.cpp lib/home_away_pattern_sets.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lib/playable_slots_set_table.o: lib/playable_slots_set_table.cpp lib/playable_slots_set_table.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<


######################################################################
## clean
######################################################################
.PHONY: clean
clean:
	$(RM) $(objs)
