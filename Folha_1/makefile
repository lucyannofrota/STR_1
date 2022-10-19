all: aux_objs problems #complete

FLAGS = -lpthread -lrt -lm

DBGCFLAGS = -g3 -O0 -DDEBUG

AUX_OBJS_NAMES = pthread_aux print_aux

PROBLEMS_NAMES = problema_1 problema_3 example

define generate_object
	echo "\nGenerating $(2).o"
	gcc -Wall $(DBGCFLAGS) -c $(1)$(2).c -o $(2).o
	echo gcc -Wall $(DBGCFLAGS) -c $(1)$(2).c -o $(2).o
	echo "$(2).o successfully generated\n"
endef

define generate_exec
	echo "\nGenerating exec $(1)"
	$(call generate_object,,$(1))
	gcc -Wall $(DBGCFLAGS) $(1).c func/func.o $(addsuffix .o,$(AUX_OBJS_NAMES)) -o $(1) ${FLAGS}
	echo "gcc -Wall $(DBGCFLAGS) $(1).c func/func.o $(addsuffix .o,$(AUX_OBJS_NAMES)) -o $(1) ${FLAGS}"
	echo "exec $(1) successfully generated\n"
endef

# gcc -Wall $(DBGCFLAGS) -c aux_libs/$(1).c -o $(1).o
# echo gcc -Wall $(DBGCFLAGS) -c aux_libs/$(1).c -o $(1).o


aux_objs:
	@$(foreach OBJ_NAME,$(AUX_OBJS_NAMES),\
		$(call generate_object,aux_libs/,$(OBJ_NAME));)


problems:
	@$(foreach p,$(PROBLEMS_NAMES),\
		$(call generate_exec,$(p)))
# $(call generate_exec,problema_1)
# 	gcc -Wall -g3 -c problema_1.c -o problema_1.o
# #	gcc -Wall -O2 -g3 problema_1.c func/func.o ${AUX_OBJS} -o problema_1 ${FLAGS}
# 	gcc -Wall -g3 problema_1.c func/func.o $(addsuffix .o,$(AUX_OBJS_NAMES)) -o problema_1 ${FLAGS}



# for OBJ in ${AUX_OBJS}; do \
# 	@echo $$OBJ \
# # $(call generate_object,${OBJ}) \
# done
#aux_libs hello #problema_1 #clean complete_msg

# aux_libs: 
# 	$(call generate_object,print_aux)

# hello:
# 	echo "Hello, World"
# 	echo "This line will always print, because the file hello does not exist."

# aux_libs: ${AUX_OBJS}.c
# 	$(call generate_object,${AUX_OBJS})










# aux_libs: ${AUX_OBJS}
# 	# @echo "FLAGS"
# 	# $(call generate_object,${AUX_OBJS})

# # pthread_aux.o print_aux.o

# # aux_libs

# pthread_aux.o: aux_libs/pthread_aux.c
# 	$(call generate_object,pthread_aux)
# #	gcc -Wall -g3 -c aux_libs/pthread_aux.c -o pthread_aux.o
# #	echo "pthread_aux.o generated"

# print_aux.o: aux_libs/print_aux.c
# 	gcc -Wall -g3 -c aux_libs/print_aux.c -o print_aux.o
# 	echo "print_aux.o generated"

# # problema_1

# problema_1: problema_1.o
# #	gcc -Wall -O2 -g3 problema_1.c func/func.o ${AUX_OBJS} -o problema_1 ${FLAGS}
# 	gcc -Wall -g3 problema_1.c func/func.o ${AUX_OBJS} -o problema_1 ${FLAGS}

# problema_1.o: problema_1.c
# 	gcc -Wall -g3 -c problema_1.c -o problema_1.o


# aux_objs:
# 	@$(foreach OBJ_NAME,$(AUX_OBJS_NAMES),\
# 		$(call generate_object,$(OBJ_NAME));)

CL_OBJS = $(AUX_OBJS_NAMES)
CL_OBJS += $(PROBLEMS_NAMES)

clean:
# @echo $(addsuffix .o,$(AUX_OBJS_NAMES))
# -rm $(addsuffix .o,$(obj))
# $(foreach problem, $(PROBLEMS_NAMES), rm $(problem))
# A = $(addsuffix .o,$(AUX_OBJS_NAMES))
# @-rm $(addsuffix .o,$(AUX_OBJS_NAMES))
# @-rm $(addsuffix .o,$(PROBLEMS_NAMES))
	@-rm $(addsuffix .o,$(CL_OBJS))
	@echo "Objects Deleted: $(addsuffix .o,$(CL_OBJS))\n"
# $(foreach OBJ, $(addsuffix .o,$(AUX_OBJS_NAMES)), rm $(OBJ))
# -rm *.o

# $(foreach obj, $(AUX_OBJS_NAMES), rm $(addsuffix .o,$(obj)))
# clear

complete:
# @echo "$(CL_OBJS)\n"
	@echo "------------------\nOperation Summary:\n------------------\n"
	@-rm $(addsuffix .o,$(CL_OBJS))
	@echo "Objects Deleted: $(addsuffix .o,$(CL_OBJS))\n"
	@echo "Binaries successfully generated: $(PROBLEMS_NAMES)\n"