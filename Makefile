RACK_DIR ?= ../..

# Add .cpp and .c files to the build
SOURCES += $(wildcard src/*.cpp)


# Add files to the ZIP package when running `make dist`
# The compiled plugin is automatically added.
DISTRIBUTABLES += $(wildcard LICENSE*) res



# Include the VCV Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk
