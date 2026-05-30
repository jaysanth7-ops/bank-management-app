CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET   = bank_app
SRCS     = main.cpp Account.cpp Bank.cpp
OBJS     = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) bank_data.txt

.PHONY: all clean