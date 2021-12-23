# If build rack yourself, it will be two levels up from us.
# If you are using the rack SDK and put is somewhere different,
# point to it here.
RACK_DIR ?= ../..

# Specify extra directories to search for include files.
FLAGS += -I./dsp

# Add .cpp and .c files to the build
# This says "all cpp files are in the src folder. You can add more files
# to that folder and they will get compiled and linked also.
SOURCES += $(wildcard src/*.cpp)


# Add files to the ZIP package when running `make dist`
# The compiled plugin is automatically added.
DISTRIBUTABLES += $(wildcard LICENSE*) res

# Include the VCV Rack plugin Makefile framework
# This makefile from VCV has many compiler flags and command 
# line variables. You should/must use this file.
include $(RACK_DIR)/plugin.mk
