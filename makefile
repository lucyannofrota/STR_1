all: aux_objs problems complete

FLAGS = -lpthread -lrt -lm -D_REENTRANT

DBGCFLAGS = -O2 -Wall -Wextra #-DDEBUG -g3 -O0

AUX_OBJS_NAMES = pthread_aux aux_functions

PROBLEMS_NAMES = problema_1 problema_3 problema_4A problema_4B problema_5A problema_5B problema_6

define generate_object
	@echo "\n|Generating $(2).o"
	gcc $(DBGCFLAGS) -c $(1)$(2).c -o $(2).o
	@echo "|$(2).o successfully generated\n" 

endef

define generate_exec
	@echo "\n|Generating exec $(1)"

	$(call generate_object,,$(1))

	gcc $(DBGCFLAGS) $(1).c func/func.o $(addsuffix .o,$(AUX_OBJS_NAMES)) -o $(1) ${FLAGS}
	@echo "\n|exec $(1) successfully generated\n"

endef


aux_objs:
	$(foreach OBJ_NAME,$(AUX_OBJS_NAMES),\
		$(call generate_object,aux_libs/,$(OBJ_NAME)))


problems:
	$(foreach p,$(PROBLEMS_NAMES),\
		$(call generate_exec,$(p)))

CL_OBJS = $(AUX_OBJS_NAMES)
CL_OBJS += $(PROBLEMS_NAMES)

define clean_f

	-rm $(addsuffix .o,$($(1)))
	echo "Objects Deleted: $(addsuffix .o,$($(1)))\n"

endef

clean:
	@-rm $(addsuffix .o,$(CL_OBJS))
	@echo "Objects Deleted: $(addsuffix .o,$(CL_OBJS))\n"

complete:
	@echo "------------------\nOperation Summary:\n------------------\n"
	@-rm $(addsuffix .o,$(CL_OBJS))
	@echo "Objects Deleted: $(addsuffix .o,$(CL_OBJS))\n"
	@echo "Binaries successfully generated: $(PROBLEMS_NAMES)\n"