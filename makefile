CXX ?= g++

DEBUG ?= 1

ifeq ($(DEBUG),1)
	CXXFLAGS+=-g
else
	CXXFLAGS+=-O2
endif

DEPS= src/block.d src/command.d src/i18n.d src/input.d src/log.d src/scene.d src/utility.d

OBJS=$(patsubst %.d,%.o,$(DEPS))

sudoku:src/main.o $(OBJS)
	@echo "building ..."
	$(CXX) -o $@ $^ -pthread

%.d:%.cpp
	rm -f $@;\
	$(CXX) -MM $< >$@.tmp;\
	sed 's,\($*\)\.o[ :]*,\1.o $@:,g' <$@.tmp >$@;\
	rm -f $@.tmp

%.o:%.cpp
	$(CXX) -c -o $@ $<

clean:
	@echo "clean ..."
	rm -f sudoku src/*.o $(DEPS)

-include $(DEPS)











