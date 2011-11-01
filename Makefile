ifeq ($(CONFIG_DIR),)
    config_file=./.config
else
    config_file=$(CONFIG_DIR)/.config
endif
-include $(config_file)

XALAN_PATH:=./lcmodule/tools/xalan-j_2_7_1/
LCD_CONFIG:=./source/config/
LCD_DIR:=./
WIN_BINARIES=./win_binaries/

LIBSRC := \
	source/utilities/Serialization.cpp\
	source/utilities/Logger.cpp\
	source/utilities/MemMappedFile.cpp\
	source/utilities/CaptiveThreadObject.cpp\
	source/utilities/BulkHandler.cpp\
	source/CEH/ProtromRpcInterface.cpp\
	source/CEH/commands_impl.cpp\
	source/CEH/a2_commands_impl.cpp\
	source/CEH/ZRpcInterface.cpp\
	source/CEH/CmdResult.cpp\
	source/LcmInterface.cpp\
	source/LCDriverThread.cpp\
	source/LCDriverMethods.cpp\
	source/LCDriverEntry.cpp\
	source/LCDriver.cpp\
	source/LCM/Hash.cpp\
	source/LCM/Buffers.cpp\
	source/LCM/Queue.cpp\
	source/LCM/Timer.cpp\
	source/api_wrappers/linux/CThreadWrapper.cpp\
	source/api_wrappers/linux/CWaitableObject.cpp\
	source/api_wrappers/linux/CSemaphore.cpp\
	source/api_wrappers/linux/CSemaphoreQueue.cpp\
	source/api_wrappers/linux/CEventObject.cpp\
	source/api_wrappers/linux/CWaitableObjectCollection.cpp\
	source/api_wrappers/linux/OS.cpp\
	source/LCDriverInterface.cpp\
	source/security_algorithms/SecurityAlgorithms.cpp\
	source/security_algorithms/sha/sha2.cpp\
	$(AUTO_DIR_LIB)/commands_marshal.cpp\
	$(AUTO_DIR_LIB)/a2_commands_marshal.cpp\

LIBOBJ_x32 := $(addprefix $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/, $(notdir $(LIBSRC:.cpp=.o)))
LIBOBJ_x64 := $(addprefix $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/, $(notdir $(LIBSRC:.cpp=.o)))

AUTOGEN_FILES := $(AUTO_DIR_LIB)/command_ids.h\
		$(AUTO_DIR_LIB)/commands.h\
		$(AUTO_DIR_LIB)/commands_impl.h\
		$(AUTO_DIR_LIB)/commands_marshal.cpp\
		$(AUTO_DIR_LIB)/lcdriver_error_codes.h\
		$(AUTO_DIR_LIB)/a2_command_ids.h\
		$(AUTO_DIR_LIB)/a2_commands.h\
		$(AUTO_DIR_LIB)/a2_commands_impl.h\
		$(AUTO_DIR_LIB)/a2_commands_marshal.cpp


#include directories
INCLUDES := \
	-Isource\
	-Isource/api_wrappers/linux\
	-Isource/utilities\
	-Isource/LCM\
	-Isource/LCM/include\
	-Isource/security_algorithms\
	-Isource/CEH\
	-Isource/security_algorithms/sha\
	-I$(AUTO_DIR_LIB)

# C++ compiler flags (-g -O2 -Wall)
CXXFLAGS := -c -O2 -Wall -fPIC -fvisibility=hidden -fno-strict-aliasing -DLCDRIVER_EXPORTS -D_FILE_OFFSET_BITS=64

LDFLAGS := -fPIC -fvisibility=hidden -lpthread -lrt -ldl -shared -Wl,-soname,liblcdriver.so

LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
#
# do 64 bit stuff here, like set some CFLAGS
#
CXXFLAGS += -DLINUX_64
build: configfile setup_folders $(LIB_x32) $(LIB_x64)
else
#
# do 32 bit stuff here
#
CXXFLAGS += -DLINUX_32
build: configfile setup_folders $(LIB_x32)
endif

$(LIB_x32): $(LIBOBJ_x32)
	$(CXX) $(LDFLAGS) -m32 -o $(LIB_x32) $(addprefix $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/, $(^F))

$(LIB_x64): $(LIBOBJ_x64)
	$(CXX) $(LDFLAGS) -o $(LIB_x64) $(addprefix $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/, $(^F))

#
# Source files build
#
$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: source/utilities/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -m32 -c $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: source/CEH/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -m32 -c $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: source/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -m32 -c $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: source/LCM/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -m32 -c $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: source/api_wrappers/linux/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -m32 -c $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: $(AUTO_DIR_LIB)/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -m32 -c $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: source/security_algorithms/sha/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -m32 -c $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: source/security_algorithms/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -m32 -c $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

###
$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: source/utilities/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: source/CEH/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: source/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: source/LCM/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: source/api_wrappers/linux/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: source/security_algorithms/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: source/security_algorithms/sha/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: $(AUTO_DIR_LIB)/%.cpp $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

#Autogen files
$(AUTO_DIR_LIB)/command_ids.h: $(LCD_CONFIG)commands.xml $(LCD_CONFIG)command_ids_h.xsl | setup_folders
	@echo "Generating autogen $(AUTO_DIR_LIB)/command_ids.h..."
	@java -classpath $(XALAN_PATH)xalan.jar org.apache.xalan.xslt.Process -in $(LCD_CONFIG)commands.xml -xsl $(LCD_CONFIG)command_ids_h.xsl -out $(AUTO_DIR_LIB)/command_ids.h

$(AUTO_DIR_LIB)/commands.h: $(LCD_CONFIG)commands.xml $(LCD_CONFIG)commands_h.xsl | setup_folders
	@echo "Generating autogen $(AUTO_DIR_LIB)/commands.h..."
	@java -classpath $(XALAN_PATH)xalan.jar org.apache.xalan.xslt.Process -in $(LCD_CONFIG)commands.xml -xsl $(LCD_CONFIG)commands_h.xsl -out $(AUTO_DIR_LIB)/commands.h

$(AUTO_DIR_LIB)/commands_impl.h: $(LCD_CONFIG)commands.xml $(LCD_CONFIG)commands_impl_h.xsl | setup_folders
	@echo "Generating autogen $(AUTO_DIR_LIB)/commands_impl.h..."
	@java -classpath $(XALAN_PATH)xalan.jar org.apache.xalan.xslt.Process -in $(LCD_CONFIG)commands.xml -xsl $(LCD_CONFIG)commands_impl_h.xsl -out $(AUTO_DIR_LIB)/commands_impl.h

$(AUTO_DIR_LIB)/commands_marshal.cpp: $(LCD_CONFIG)commands.xml $(LCD_CONFIG)commands_marshal_cpp.xsl | setup_folders
	@echo "Generating autogen $(AUTO_DIR_LIB)/commands_marshal.cpp..."
	@java -classpath $(XALAN_PATH)xalan.jar org.apache.xalan.xslt.Process -in $(LCD_CONFIG)commands.xml -xsl $(LCD_CONFIG)commands_marshal_cpp.xsl -out $(AUTO_DIR_LIB)/commands_marshal.cpp

$(AUTO_DIR_LIB)/lcdriver_error_codes.h: $(LCD_CONFIG)lcdriver_error_codes.xml $(LCD_CONFIG)lcdriver_error_codes_h.xsl | setup_folders
	@echo "Generating autogen $(AUTO_DIR_LIB)/lcdriver_error_codes.h..."
	@java -classpath $(XALAN_PATH)xalan.jar org.apache.xalan.xslt.Process -in $(LCD_CONFIG)lcdriver_error_codes.xml -xsl $(LCD_CONFIG)lcdriver_error_codes_h.xsl -out $(AUTO_DIR_LIB)/lcdriver_error_codes.h

$(AUTO_DIR_LIB)/a2_command_ids.h: $(LCD_CONFIG)a2_commands.xml $(LCD_CONFIG)a2_command_ids_h.xsl | setup_folders
	@echo "Generating autogen $(AUTO_DIR_LIB)/a2_command_ids.h..."
	@java -classpath $(XALAN_PATH)xalan.jar org.apache.xalan.xslt.Process -in $(LCD_CONFIG)a2_commands.xml -xsl $(LCD_CONFIG)a2_command_ids_h.xsl -out $(AUTO_DIR_LIB)/a2_command_ids.h

$(AUTO_DIR_LIB)/a2_commands.h: $(LCD_CONFIG)a2_commands.xml $(LCD_CONFIG)a2_commands_h.xsl | setup_folders
	@echo "Generating autogen $(AUTO_DIR_LIB)/a2_commands.h..."
	@java -classpath $(XALAN_PATH)xalan.jar org.apache.xalan.xslt.Process -in $(LCD_CONFIG)a2_commands.xml -xsl $(LCD_CONFIG)a2_commands_h.xsl -out $(AUTO_DIR_LIB)/a2_commands.h

$(AUTO_DIR_LIB)/a2_commands_impl.h: $(LCD_CONFIG)a2_commands.xml $(LCD_CONFIG)a2_commands_impl_h.xsl | setup_folders
	@echo "Generating autogen $(AUTO_DIR_LIB)/a2_commands_impl.h..."
	@java -classpath $(XALAN_PATH)xalan.jar org.apache.xalan.xslt.Process -in $(LCD_CONFIG)a2_commands.xml -xsl $(LCD_CONFIG)a2_commands_impl_h.xsl -out $(AUTO_DIR_LIB)/a2_commands_impl.h

$(AUTO_DIR_LIB)/a2_commands_marshal.cpp: $(LCD_CONFIG)a2_commands.xml $(LCD_CONFIG)a2_commands_marshal_cpp.xsl | setup_folders
	@echo "Generating autogen $(AUTO_DIR_LIB)/a2_commands_marshal.cpp..."
	@java -classpath $(XALAN_PATH)xalan.jar org.apache.xalan.xslt.Process -in $(LCD_CONFIG)a2_commands.xml -xsl $(LCD_CONFIG)a2_commands_marshal_cpp.xsl -out $(AUTO_DIR_LIB)/a2_commands_marshal.cpp

#setting up needed folders
$(BUILDFOLDER): | configfile
	@mkdir -p $@ 2> /dev/null

$(BUILDFOLDER)/.builddummy: | $(BUILDFOLDER)
	@touch $@

$(AUTO_DIR_LIB): | configfile
	@mkdir -p $@ 2> /dev/null

$(AUTO_DIR_LIB)/.autodirdummy: | $(AUTO_DIR_LIB)
	@touch $@

$(LCD_INSTALLDIR): | configfile
	@mkdir -p $@ 2> /dev/null

$(LCD_INSTALLDIR)/.installdummy: | $(LCD_INSTALLDIR)
	@touch $@

.PHONY: setup_folders
setup_folders: validatevariables
setup_folders: $(BUILDFOLDER)/.builddummy $(AUTO_DIR_LIB)/.autodirdummy $(LCD_INSTALLDIR)/.installdummy
	@echo $< > /dev/null

.PHONY: validatevariables
validatevariables: configfile
	$(if $(CXX),, $(error "CXX not set"))
	$(if $(BUILDFOLDER),, $(error "BUILDFOLDER not set"))
	$(if $(AUTO_DIR_LIB),, $(error "AUTO_DIR_LIB not set"))
	$(if $(LIB_x32),, $(error "LIB_x32 not set"))
	$(if $(LIB_x64),, $(error "LIB_x64 not set"))
	$(if $(LIB_x32_OBJ_DIR),, $(error "LIB_x32_OBJ_DIR not set"))
	$(if $(LIB_x64_OBJ_DIR),, $(error "LIB_x64_OBJ_DIR not set"))
	$(if $(LCD_INSTALLDIR),, $(error "LCD_INSTALLDIR not set"))

#if the wildcard function finds any file matching the name
#of the configfile then this will result in
#"configfile: " so the config target will not be unnecessarily
#called. If however there is no file matching the configfile
#this will become "configfile: config" and the config target
#will be called and the configfile created.
#(calling config directly will always recreate the configfile
#so any values that has been changed will be overwritten with
#the new value)
.PHONY: configfile
configfile: $(if $(wildcard $(config_file)),,config)
	@echo $< > /dev/null

.PHONY: config
config: BUILDFOLDER := $(LCD_DIR)/out
config: AUTO_DIR_LIB :=$(BUILDFOLDER)/autogen/
config: LIB_x32 := $(BUILDFOLDER)/liblcdriver.so
config: LIB_x64 := $(BUILDFOLDER)/liblcdriver_x64.so
config: LIB_x32_OBJ_DIR := x32
config: LIB_x64_OBJ_DIR := x64
config: CXX := $(CROSS_PREFIX)g++
config: LCD_INSTALLDIR := /tmp/
config:
	@echo Generating config file...
	@rm -f $(config_file)
	@touch $(config_file)
	@echo CXX: $(CXX)
	@echo "CXX := $(CXX)" >> $(config_file)
	@echo BUILDFOLDER: $(BUILDFOLDER)
	@echo "BUILDFOLDER := $(BUILDFOLDER)" >> $(config_file)
	@echo AUTO_DIR_LIB: $(AUTO_DIR_LIB)
	@echo "AUTO_DIR_LIB := $(AUTO_DIR_LIB)" >> $(config_file)
	@echo LIB_x32: $(LIB_x32)
	@echo "LIB_x32 := $(LIB_x32)" >> $(config_file)
	@echo LIB_x64: $(LIB_x64)
	@echo "LIB_x64 := $(LIB_x64)" >> $(config_file)
	@echo LIB_x32_OBJ_DIR: $(LIB_x32_OBJ_DIR)
	@echo "LIB_x32_OBJ_DIR := $(LIB_x32_OBJ_DIR)" >> $(config_file)
	@echo LIB_x64_OBJ_DIR: $(LIB_x64_OBJ_DIR)
	@echo "LIB_x64_OBJ_DIR := $(LIB_x64_OBJ_DIR)" >> $(config_file)
	@echo LCD_INSTALLDIR: $(LCD_INSTALLDIR)
	@echo "LCD_INSTALLDIR := $(LCD_INSTALLDIR)" >> $(config_file)

install: build
	install -m 0755 -t $(LCD_INSTALLDIR) $(BUILDFOLDER)/liblcdriver.so
ifeq ($(LBITS),64)
	install -m 0755 -t $(LCD_INSTALLDIR) $(BUILDFOLDER)/liblcdriver_x64.so
endif
	install -m 0755 -t $(LCD_INSTALLDIR) $(WIN_BINARIES)/*.dll

clean:
	$(if $(BUILDFOLDER), \
		$(if $(LIB_x32_OBJ_DIR), \
			@rm -f $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/*.o \
			@rm -rf $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR) \
			@rm -rf $(BUILDFOLDER),),)
ifeq ($(LBITS),64)
	$(if $(BUILDFOLDER), \
		$(if $(LIB_x64_OBJ_DIR), \
			@rm -f $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/*.o \
			@rm -rf $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR) \
			@rm -rf $(BUILDFOLDER),),)
endif
	$(if $(BUILDFOLDER), \
		@rm -f $(BUILDFOLDER)/*.so* \
		@rm -rf $(BUILDFOLDER),)

distclean: clean
	$(if $(AUTO_DIR_LIB), \
		@rm -f $(AUTO_DIR_LIB)/*.cpp \
		@rm -f $(AUTO_DIR_LIB)/*.h,)
	$(if $(LCD_INSTALLDIR), \
		@rm -f $(LCD_INSTALLDIR)/liblcdriver.so,)
ifeq ($(LBITS),64)
	$(if $(LCD_INSTALLDIR), \
		@rm -f $(LCD_INSTALLDIR)/liblcdriver_x64.so,)
endif
	$(if $(config_file), \
		@rm -f $(config_file),)

COV_DATA_DIR=cov_data
COV_INTER_DATA_DIR=cov_inter

coverity:
	@if [ -d "$(COV_DATA_DIR)" ]; then \
	@cov-stop-gui --datadir $(COV_DATA_DIR); \
	fi
	@cov-configure --compiler $(CC) --comptype gcc
	@if [ -d "$(COV_DATA_DIR)" ]; then \
		echo coverity gui already installed; \
	else \
		cov-install-gui --password admin --datadir $(COV_DATA_DIR) --product lcd --domain "C/C++"; \
	fi

	@cov-build --dir $(COV_INTER_DATA_DIR) $(MAKE) build
	@cov-analyze --dir $(COV_INTER_DATA_DIR) --aggressiveness-level medium --all
	@cov-commit-defects --datadir $(COV_DATA_DIR) --product lcd --user admin --dir $(COV_INTER_DATA_DIR)
	@cov-start-gui --datadir $(COV_DATA_DIR) --port 1122
	echo Go to localhost port 1122 in webbrowser and login with username admin and password admin to review result
