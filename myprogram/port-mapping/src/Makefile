
APP_DIR 	= ./
OPTION =

PLATFORM=$(shell uname -p)

ifeq ("$(PLATFORM)", "i686")	
	LINUX   = -DLINUX
else
	LINUX   = -DLINUX
endif

TARGET   = port-mirroring

DEPS_FILE  	   = $(TARGET).dep 

all:$(TARGET)
CC     =gcc
WARN   = -w -W -Werror -Wall -Wchar-subscripts -Wcomment -Wformat -Wmissing-braces \
             -Wparentheses -Wswitch -Wunused -Wunused-parameter -Wuninitialized  \
             -Wfloat-equal -Wshadow -Wcast-align -Wsign-compare  \
             -Wunreachable-code  -Wpointer-arith \
             -Winline -Wno-deprecated




INC_DIR = -I./ -I./inc_pcap -I./inc_pcap/pcap
#LIBS    = -L./
CFLAGS  = $(WARN) $(LINUX) $(INC_DIR) -fPIC $(OPTION)

          
ACTION = release

ifeq ($(ACTION),release)
CFLAGS += -O2
else
CFLAGS += -g
endif

#CFLAGS += -DCHECK_DOG

#CC_DEF += 

C_RULE_FILE	   = c.rule
######################################################################################### 
#±àÒë¹æÔò

#c and cpp file dependence compute rule
%.dcpp: %.cpp
	$(CC) $(CFLAGS) -M $< >> $(DEPS_FILE)
	@cat $(C_RULE_FILE) >> $(DEPS_FILE)
%.dc: %.c
	$(CC) $(CFLAGS) -M $< >> $(DEPS_FILE)
	@cat $(C_RULE_FILE) >> $(DEPS_FILE)
	

#¼ÆËãËùÐè±àÒëµÄÇý¶¯ÎÄ¼þ
APP_SRCS 	:= $(foreach dir, $(APP_DIR), $(wildcard  $(dir)/*.c))
APP_SRCS 	+= $(foreach dir, $(APP_DIR), $(wildcard  $(dir)/*.cpp))
APP_SRCS  	:= $(filter %.c %.cpp, $(APP_SRCS))
APP_OBJS 	:= $(patsubst %.cpp,%.o, $(filter %.cpp, $(notdir $(APP_SRCS)))) 
APP_OBJS 	+= $(patsubst %.c,%.o, $(filter %.c, $(notdir $(APP_SRCS))))
 
APP_DEPS 	:= $(patsubst %.cpp,%.dcpp, $(filter %.cpp, $(APP_SRCS))) 
APP_DEPS 	+= $(patsubst %.c,%.dc, $(filter %.c, $(APP_SRCS)))

#########################################################################################
#Éú³ÉÒÀÀµ¹ØÏµ
dep: del_dep $(APP_DEPS)
	@echo building all files dependency done

del_dep:
	@$(RM) $(DEPS_FILE) -rf

#########################################################################################
#±àÒë¹¤³Ì
ifneq ($(wildcard $(DEPS_FILE)),)
include $(DEPS_FILE)
endif

$(TARGET):$(APP_OBJS)  
#	rm -f /usr/lib64/libpcap.so*
#	rm -f /lib64/libcapture.so
#	cp libpcap.so.1 /usr/lib64/
#	ln -s /usr/lib64/libpcap.so.1 /usr/lib64/libpcap.so
	
	
#ifeq ("$(PLATFORM)", "x86_64")	
#else
#endif
	$(CC) $(CFLAGS)  -LS -o $(TARGET) $(APP_OBJS) $(LDFLAGS) $(LIBS) -lpcap -lpthread -liconv -lhiredis


install:
	cd ../
	mkdir bin
	mv *.o ../bin -f              
clean:
	rm -f *.o $(TARGET)

