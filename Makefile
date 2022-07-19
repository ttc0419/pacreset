CXXFLAGS = -Wall -pipe -march=native
LDFLAGS = -larchive -lalpm

pacreset: main.cpp
ifeq ($(build_type), release)
	@echo "Builing release version..."
	$(eval CXXFLAGS += -O3 -flto -s)
else
	@echo "Building debug version..."
	$(eval CXXFLAGS += -Wextra -O0 -fsanitize=address)
endif
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f pacreset
