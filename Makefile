EXEC := ./computeIdle

CFLAGS := -g -Og -Ofast -Wall -lX11 -lm -I src/include

OBJS := $(patsubst src/%.c,src/%.o,$(wildcard src/*.c))

build: $(OBJS)
		mkdir -p checkpoints/
		gcc $(OBJS) -o $(EXEC) $(CFLAGS)

# pull in dependency info for *existing* .o files
# -include $(OBJS:.o=.dep)
#
#  # compile and generate dependency info
%.o: %.c
		gcc -c $(CFLAGS) $*.c -o $*.o
		gcc -MM $(CFLAGS) $*.c > $*.dep

# remove compilation products
clean:
	rm -f $(EXEC) src/*.o src/*.dep

run: build
	$(EXEC)
