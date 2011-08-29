ifeq ($(CONFIG_DIR),)
    config_file=.config
else
    config_file=$(CONFIG_DIR)/.config
endif

-include $(config_file)

LCM_PATH:=./
TRANSPORT_PATH:=${LCM_PATH}/source/cnh1605204_ldr_transport_layer/
NETWORK_PATH:=${LCM_PATH}/source/cnh1605205_ldr_network_layer/
LCM_CNH_PATH:=${LCM_PATH}/source/cnh1606344_ldr_communication_module/
UTILITIES_PATH:=$(LOADER_DIR)/src/loader_code_base/cnh1605551_ldr_utilities
TIME_UTILITIES_PATH:=$(LOADER_DIR)/src/loader_code_base/cnh1605720_ldr_time_utilities
BUFFERS_PATH:=$(LOADER_DIR)/src/loader_code_base/cnh1605203_ldr_communication_buffer
SECURITY_ALGORITHMS_PATH:=$(LOADER_DIR)/src/loader_code_base/cnh1605721_ldr_security_algorithms
LCM_CONFIG:=$(LCM_CNH_PATH)/config
LCM_CUST_CONFIG:=$(LCM_PATH)/customer_config
SUPPORTED_CMDS_PATH:=$(LOADER_DIR)/src/platform_support/stn8500/config/supported_commands.xml

AUTOGEN_FILES := $(AUTO_DIR_LIB)/commands.h\
		$(AUTO_DIR_LIB)/command_ids.h\
		$(AUTO_DIR_LIB)/error_codes.h\
		$(AUTO_DIR_LIB)/command_marshal.c\
		$(AUTO_DIR_LOADER)/error_codes.h\
		$(AUTO_DIR_LOADER)/audit.h\
		$(AUTO_DIR_LOADER)/audit.c\
		$(AUTO_DIR_LOADER)/command_ids.h\
		$(AUTO_DIR_LOADER)/commands_impl.h\
		$(AUTO_DIR_LOADER)/commands.h\
		$(AUTO_DIR_LOADER)/command_marshal.c\
		$(AUTO_DIR_LOADER)/custom_command_ids.h\
		$(AUTO_DIR_LOADER)/custom_commands_impl.h\
		$(AUTO_DIR_LOADER)/custom_commands.h\
		$(AUTO_DIR_LOADER)/custom_command_marshal.c\
		$(AUTO_DIR_LOADER)/ces_commands.c\
		$(AUTO_DIR_LOADER)/ces_commands.h


LIBSRC :=${TRANSPORT_PATH}/source/bulk_protocol.c\
	${TRANSPORT_PATH}/source/command_protocol.c\
	${TRANSPORT_PATH}/source/protrom_protocol.c\
	${TRANSPORT_PATH}/source/protrom_transport.c\
	${TRANSPORT_PATH}/source/r15_transport_layer.c\
	${TRANSPORT_PATH}/source/a2_transport.c\
	${TRANSPORT_PATH}/source/a2_protocol.c\
	${TRANSPORT_PATH}/source/a2_speedflash.c\
	${TRANSPORT_PATH}/source/z_transport.c\
	${NETWORK_PATH}/source/a2_header.c\
	${NETWORK_PATH}/source/a2_network.c\
	${NETWORK_PATH}/source/r15_header.c\
	${NETWORK_PATH}/source/r15_network_layer.c\
	${NETWORK_PATH}/source/z_network.c\
	${NETWORK_PATH}/source/protrom_header.c\
	${NETWORK_PATH}/source/protrom_network.c\
	${LCM_CNH_PATH}/source/communication_service.c\
	${LCM_CNH_PATH}/source/protrom_family.c\
	${LCM_CNH_PATH}/source/z_family.c\
	${LCM_CNH_PATH}/source/r15_family.c\
	${LCM_CNH_PATH}/source/a2_family.c\
	${LCM_PATH}/source/cnh1605551_ldr_utilities/critical_section/critical_section_linux.c\
	${LCM_PATH}/source/serialization.c

LIBOBJ_x32 := $(addprefix $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/, $(notdir $(LIBSRC:.c=.o)))
LIBOBJ_x64 := $(addprefix $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/, $(notdir $(LIBSRC:.c=.o)))

	#-I ${AUTO_DIR)\
	#	-I ${LCM_PATH}/include/
#include directories
INCLUDES := -I. \
	-I $(AUTO_DIR_LIB)\
	-I $(AUTO_DIR_LOADER)\
	-I ${TRANSPORT_PATH}/include/\
	-I ${NETWORK_PATH}/include/\
	-I ${LCM_CNH_PATH}/include/\
	-I ${TIME_UTILITIES_PATH}/include/\
	-I ${UTILITIES_PATH}/include/\
	-I ${LCM_PATH}/source/cnh1605551_ldr_utilities/critical_section/\
	-I ${UTILITIES_PATH}/communication_abstraction/\
	-I ${BUFFERS_PATH}/include/\
	-I ${SECURITY_ALGORITHMS_PATH}/include/\
	-I ${LCM_PATH}/source/legacy_compatibility/

# C compiler flags (-g -O2 -Wall)
CFLAGS := -g -O2 -Wall -fPIC -fvisibility=hidden -DUINT64_SUPPORTED -DSINT64_SUPPORTED -DINT64_BASE_TYPE="long long" -DCFG_ENABLE_A2_FAMILY -DLCM_EXPORTS

LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
#
# do 64 bit stuff here, like set some CFLAGS
#
build: configfile setup_folders $(LIB_x32) $(LIB_x64)
else
#
# do 32 bit stuff here
#
build: configfile setup_folders $(LIB_x32)
endif

$(LIB_x32): $(LIBOBJ_x32)
	$(CC) -shared -m32 -Wl,-soname,$(LIB_x32) -o $(LIB_x32) -lc $(addprefix $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/, $(^F))

$(LIB_x64): $(LIBOBJ_x64)
	$(CC) -shared -Wl,-soname,$(LIB_x64) -o $(LIB_x64) -lc $(addprefix $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/, $(^F))

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: ${TRANSPORT_PATH}/source/%.c $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c -m32 $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: ${NETWORK_PATH}/source/%.c $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c -m32 $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: ${LCM_CNH_PATH}/source/%.c  $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c -m32 $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: ${LCM_PATH}/source/cnh1605551_ldr_utilities/critical_section/%.c  $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c -m32 $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/%.o: ${LCM_PATH}/source/%.c  $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c -m32 $< -o $(BUILDFOLDER)/$(LIB_x32_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: ${TRANSPORT_PATH}/source/%.c  $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: ${NETWORK_PATH}/source/%.c  $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: ${LCM_CNH_PATH}/source/%.c  $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: ${LCM_PATH}/source/cnh1605551_ldr_utilities/critical_section/%.c  $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

$(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/%.o: ${LCM_PATH}/source/%.c  $(AUTOGEN_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $(BUILDFOLDER)/$(LIB_x64_OBJ_DIR)/$(@F)

#Autogen files
$(AUTO_DIR_LIB)/commands.h: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/commands_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/commands_h.xsl -out $@ -PARAM target lcm

$(AUTO_DIR_LIB)/command_ids.h: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/command_ids_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/command_ids_h.xsl -out $@ -PARAM target lcm

$(AUTO_DIR_LIB)/error_codes.h: $(LCM_CONFIG)/error_codes.xml $(LCM_CONFIG)/errorcode_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/error_codes.xml -xsl $(LCM_CONFIG)/errorcode_h.xsl -out $@

$(AUTO_DIR_LIB)/command_marshal.c: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/command_marshal.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/command_marshal.xsl -out $@ -PARAM target lcm

$(AUTO_DIR_LOADER)/error_codes.h: $(LCM_CONFIG)/error_codes.xml $(LCM_CONFIG)/errorcode_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/error_codes.xml -xsl $(LCM_CONFIG)/errorcode_h.xsl -out $@ -param supportedCommands $(SUPPORTED_CMDS_PATH)

$(AUTO_DIR_LOADER)/audit.h: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/audit_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/audit_h.xsl -out $@ -param supportedCommands $(SUPPORTED_CMDS_PATH)

$(AUTO_DIR_LOADER)/audit.c: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/audit_c.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/audit_c.xsl -out $@ -param supportedCommands $(SUPPORTED_CMDS_PATH)

$(AUTO_DIR_LOADER)/command_ids.h: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/command_ids_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/command_ids_h.xsl -out $@ -param supportedCommands $(SUPPORTED_CMDS_PATH)

$(AUTO_DIR_LOADER)/commands_impl.h: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/commands_impl_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/commands_impl_h.xsl -out $@ -param supportedCommands $(SUPPORTED_CMDS_PATH)

$(AUTO_DIR_LOADER)/commands.h: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/commands_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/commands_h.xsl -out $@ -param supportedCommands $(SUPPORTED_CMDS_PATH)

$(AUTO_DIR_LOADER)/command_marshal.c: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/command_marshal.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/command_marshal.xsl -out $@ -param supportedCommands $(SUPPORTED_CMDS_PATH)

$(AUTO_DIR_LOADER)/ces_commands.c: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/ces_commands_c.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/ces_commands_c.xsl -out $@ -param supportedCommands $(SUPPORTED_CMDS_PATH)

$(AUTO_DIR_LOADER)/ces_commands.h: $(LCM_CONFIG)/commands.xml $(LCM_CONFIG)/ces_commands_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CONFIG)/commands.xml -xsl $(LCM_CONFIG)/ces_commands_h.xsl -out $@ -param supportedCommands $(SUPPORTED_CMDS_PATH)

$(AUTO_DIR_LOADER)/custom_command_ids.h: $(LCM_CUST_CONFIG)/custom_commands.xml $(LCM_CUST_CONFIG)/custom_command_ids_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CUST_CONFIG)/custom_commands.xml -xsl $(LCM_CUST_CONFIG)/custom_command_ids_h.xsl -out $@

$(AUTO_DIR_LOADER)/custom_commands_impl.h: $(LCM_CUST_CONFIG)/custom_commands.xml $(LCM_CUST_CONFIG)/custom_commands_impl_h.xsl  | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CUST_CONFIG)/custom_commands.xml -xsl $(LCM_CUST_CONFIG)/custom_commands_impl_h.xsl -out $@

$(AUTO_DIR_LOADER)/custom_commands.h: $(LCM_CUST_CONFIG)/custom_commands.xml $(LCM_CUST_CONFIG)/custom_commands_h.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CUST_CONFIG)/custom_commands.xml -xsl $(LCM_CUST_CONFIG)/custom_commands_h.xsl -out $@

$(AUTO_DIR_LOADER)/custom_command_marshal.c: $(LCM_CUST_CONFIG)/custom_commands.xml $(LCM_CUST_CONFIG)/custom_command_marshal.xsl | setup_folders
	java -classpath $(LCM_PATH)/tools/xalan-j_2_7_1/xalan.jar org.apache.xalan.xslt.Process -in $(LCM_CUST_CONFIG)/custom_commands.xml -xsl $(LCM_CUST_CONFIG)/custom_command_marshal.xsl -out $@

#setting up needed folders
$(BUILDFOLDER): | configfile
	@mkdir -p $@ 2> /dev/null

$(BUILDFOLDER)/.builddummy: | $(BUILDFOLDER)
	@touch $@

$(LCMLDR_INSTALLDIR): | configfile
	@mkdir -p $@ 2> /dev/null

$(LCMLDR_INSTALLDIR)/.ldrinstalldummy: | $(LCMLDR_INSTALLDIR)
	@touch $@

$(LCMLIB_INSTALLDIR): | configfile
	@mkdir -p $@ 2> /dev/null

$(LCMLIB_INSTALLDIR)/.libinstalldummy: | $(LCMLIB_INSTALLDIR)
	@touch $@

$(AUTO_DIR_LOADER): | configfile
	@mkdir -p $@ 2> /dev/null

$(AUTO_DIR_LOADER)/.autodirldrdummy: | $(AUTO_DIR_LOADER)
	@touch $@

$(AUTO_DIR_LIB): | configfile
	@mkdir -p $@ 2> /dev/null

$(AUTO_DIR_LIB)/.autodirlibdummy: | $(AUTO_DIR_LIB)
	@touch $@

$(LCM_AUTO_DIR): | configfile
	@mkdir -p $@ 2> /dev/null

$(LCM_AUTO_DIR)/.autodirdummy : | $(LCM_AUTO_DIR)
	@touch $@

.PHONY: setup_folders
setup_folders: validatevariables
setup_folders: $(BUILDFOLDER)/.builddummy $(LCMLDR_INSTALLDIR)/.ldrinstalldummy $(LCMLIB_INSTALLDIR)/.libinstalldummy
setup_folders: $(AUTO_DIR_LOADER)/.autodirldrdummy $(AUTO_DIR_LIB)/.autodirlibdummy $(LCM_AUTO_DIR)/.autodirdummy 
	@echo $< > /dev/null




.PHONY: validatevariables
validatevariables: configfile
	$(if $(CC),, $(error "CC not set"))
	$(if $(LOADER_DIR),, $(error "LOADER_DIR not set"))
	$(if $(BUILDFOLDER),, $(error "BUILDFOLDER not set"))
	$(if $(AUTO_DIR_LIB),, $(error "AUTO_DIR_LIB not set"))
	$(if $(LCM_AUTO_DIR),, $(error "LCM_AUTO_DIR not set"))
	$(if $(AUTO_DIR_LOADER),, $(error "AUTO_DIR_LOADER not set"))
	$(if $(LCMLIB_INSTALLDIR),, $(error "LCMLIB_INSTALLDIR not set"))
	$(if $(LCMLDR_INSTALLDIR),, $(error "LCMLDR_INSTALLDIR not set"))
	$(if $(LIB_x32),, $(error "LIB_x32 not set"))
	$(if $(LIB_x64),, $(error "LIB_x64 not set"))
	$(if $(LIB_x32_OBJ_DIR),, $(error "LIB_x32_OBJ_DIR not set"))
	$(if $(LIB_x64_OBJ_DIR),, $(error "LIB_x64_OBJ_DIR not set"))

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
config: BUILDFOLDER := $(if $(strip $(BUILDFOLDER)),$(BUILDFOLDER),$(shell bash -c "mktemp -d"))
config: LCM_AUTO_DIR:=$(BUILDFOLDER)/autogen
config: AUTO_DIR_LIB:=$(LCM_AUTO_DIR)/lib
config: AUTO_DIR_LOADER:=$(LCM_AUTO_DIR)/loader
config: LIB_x32 := $(BUILDFOLDER)/liblcm.so.1
config: LIB_x64 := $(BUILDFOLDER)/liblcm_x64.so.1
config: LCMLIB_INSTALLDIR := /tmp/
config: LCMLDR_INSTALLDIR := /tmp/
config: LIB_x32_OBJ_DIR := x32
config: LIB_x64_OBJ_DIR := x64
config: CC := gcc
config: LOADER_DIR := ./
config:
	@echo Generating config file...
	@echo config_file : $(config_file)
	@rm -f $(config_file)
	@touch $(config_file)
	@echo CC: $(CC)
	@echo "CC := $(CC)" >> $(config_file)
	@echo LOADER_DIR: $(LOADER_DIR)
	@echo "LOADER_DIR := $(LOADER_DIR)" >> $(config_file)
	@echo BUILDFOLDER: $(BUILDFOLDER)
	@echo "BUILDFOLDER := $(BUILDFOLDER)" >> $(config_file)
	@echo AUTO_DIR_LIB: $(AUTO_DIR_LIB)
	@echo "AUTO_DIR_LIB := $(AUTO_DIR_LIB)" >> $(config_file)
	@echo LCM_AUTO_DIR: $(LCM_AUTO_DIR)
	@echo "LCM_AUTO_DIR := $(LCM_AUTO_DIR)" >> $(config_file)
	@echo LCMLIB_INSTALLDIR: $(LCMLIB_INSTALLDIR)
	@echo "LCMLIB_INSTALLDIR := $(LCMLIB_INSTALLDIR)" >> $(config_file)
	@echo LCMLDR_INSTALLDIR: $(LCMLDR_INSTALLDIR)
	@echo "LCMLDR_INSTALLDIR := $(LCMLDR_INSTALLDIR)" >> $(config_file)
	@echo AUTO_DIR_LOADER: $(AUTO_DIR_LOADER)
	@echo "AUTO_DIR_LOADER := $(AUTO_DIR_LOADER)" >> $(config_file)
	@echo LIB_x32: $(LIB_x32)
	@echo "LIB_x32 := $(LIB_x32)" >> $(config_file)
	@echo LIB_x64: $(LIB_x64)
	@echo "LIB_x64 := $(LIB_x64)" >> $(config_file)
	@echo LIB_x32_OBJ_DIR: $(LIB_x32_OBJ_DIR)
	@echo "LIB_x32_OBJ_DIR := $(LIB_x32_OBJ_DIR)" >> $(config_file)
	@echo LIB_x64_OBJ_DIR: $(LIB_x64_OBJ_DIR)
	@echo "LIB_x64_OBJ_DIR := $(LIB_x64_OBJ_DIR)" >> $(config_file)
	@echo CONFIG_DIR : $(CONFIG_DIR)

install: build
	install -p -m 0755 -t ${LCMLIB_INSTALLDIR} ${BUILDFOLDER}/liblcm.so.*
ifeq ($(LBITS),64)
	install -p -m 0755 -t ${LCMLIB_INSTALLDIR} ${BUILDFOLDER}/liblcm_x64.so.*
endif
	install -p -m 0555 -t ${LCMLDR_INSTALLDIR} ${AUTO_DIR_LOADER}/*
	install -p -m 0555 -t ${LCMLDR_INSTALLDIR} ${TRANSPORT_PATH}/source/*.c
	install -p -m 0555 -t ${LCMLDR_INSTALLDIR} ${NETWORK_PATH}/source/*.c
	install -p -m 0555 -t ${LCMLDR_INSTALLDIR} ${LCM_CNH_PATH}/source/*.c
	install -p -m 0555 -t ${LCMLDR_INSTALLDIR} ${TRANSPORT_PATH}/include/*.h
	install -p -m 0555 -t ${LCMLDR_INSTALLDIR} ${NETWORK_PATH}/include/*.h
	install -p -m 0555 -t ${LCMLDR_INSTALLDIR} ${LCM_CNH_PATH}/include/*.h
	install -p -m 0555 -t ${LCMLDR_INSTALLDIR} ${LCM_PATH}/source/serialization.c

clean:
	$(if ${BUILDFOLDER}, \
		$(if ${LIB_x32_OBJ_DIR}, \
			@rm -f ${BUILDFOLDER}/${LIB_x32_OBJ_DIR}/*.o \
			@rm -rf ${BUILDFOLDER}/${LIB_x32_OBJ_DIR},),)
ifeq ($(LBITS),64)
	$(if ${BUILDFOLDER}, \
		$(if ${LIB_x64_OBJ_DIR}, \
			@rm -f ${BUILDFOLDER}/${LIB_x64_OBJ_DIR}/*.o \
			@rm -rf ${BUILDFOLDER}/${LIB_x64_OBJ_DIR},),)
endif
	$(if ${BUILDFOLDER}, \
		@rm -f ${BUILDFOLDER}/*.so*,)

distclean: clean
	$(if ${LCMLDR_INSTALLDIR}, \
		@rm -f ${LCMLDR_INSTALLDIR}/a2_family.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_a2_family.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_a2_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/a2_header.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_a2_header.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_a2_transport.h \
		@rm -f ${LCMLDR_INSTALLDIR}/a2_network.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_a2_network.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_bulk_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/a2_protocol.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_a2_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_command_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/a2_transport.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_a2_transport.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_communication_service.h \
		@rm -f ${LCMLDR_INSTALLDIR}/audit.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_bulk_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/ti_family.c \
		@rm -f ${LCMLDR_INSTALLDIR}/audit.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_command_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/ti_header.c \
		@rm -f ${LCMLDR_INSTALLDIR}/bulk_protocol.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_communication_service.h \
		@rm -f ${LCMLDR_INSTALLDIR}/ti_network.c \
		@rm -f ${LCMLDR_INSTALLDIR}/command_ids.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_measurement_tool.h \
		@rm -f ${LCMLDR_INSTALLDIR}/ti_protocol.c \
		@rm -f ${LCMLDR_INSTALLDIR}/command_marshal.c \
		@rm -f ${LCMLDR_INSTALLDIR}/ces_commands.c \
		@rm -f ${LCMLDR_INSTALLDIR}/ces_commands.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_protrom_family.h \
		@rm -f ${LCMLDR_INSTALLDIR}/ti_transport.c \
		@rm -f ${LCMLDR_INSTALLDIR}/command_protocol.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_protrom_header.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_measurement_tool.h \
		@rm -f ${LCMLDR_INSTALLDIR}/commands.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_protrom_network.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_protrom_header.h \
		@rm -f ${LCMLDR_INSTALLDIR}/commands_impl.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_protrom_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_protrom_network.h \
		@rm -f ${LCMLDR_INSTALLDIR}/communication_service.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_protrom_transport.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_protrom_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/custom_command_ids.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_r15_family.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_protrom_transport.h \
		@rm -f ${LCMLDR_INSTALLDIR}/custom_command_marshal.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_r15_header.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_r15_family.h \
		@rm -f ${LCMLDR_INSTALLDIR}/custom_commands.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_r15_network_layer.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_r15_header.h \
		@rm -f ${LCMLDR_INSTALLDIR}/custom_commands_impl.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_r15_transport_layer.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_r15_network_layer.h \
		@rm -f ${LCMLDR_INSTALLDIR}/error_codes.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_ti_family.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_r15_transport_layer.h \
		@rm -f ${LCMLDR_INSTALLDIR}/measurement_tool.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_ti_header.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_ti_header.h \
		@rm -f ${LCMLDR_INSTALLDIR}/memmap.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r_ti_network.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_ti_network.h \
		@rm -f ${LCMLDR_INSTALLDIR}/protrom_family.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_ti_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_ti_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/protrom_header.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_ti_transport.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_ti_transport.h \
		@rm -f ${LCMLDR_INSTALLDIR}/protrom_network.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_z_family.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_z_header.h \
		@rm -f ${LCMLDR_INSTALLDIR}/protrom_protocol.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_z_network.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_z_network.h \
		@rm -f ${LCMLDR_INSTALLDIR}/protrom_transport.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_z_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/t_z_protocol.h \
		@rm -f ${LCMLDR_INSTALLDIR}/r15_family.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r_z_transport.h \
		@rm -f ${LCMLDR_INSTALLDIR}/z_family.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r15_header.c \
		@rm -f ${LCMLDR_INSTALLDIR}/serialization.c \
		@rm -f ${LCMLDR_INSTALLDIR}/z_network.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r15_network_layer.c \
		@rm -f ${LCMLDR_INSTALLDIR}/t_a2_header.h \
		@rm -f ${LCMLDR_INSTALLDIR}/z_transport.c \
		@rm -f ${LCMLDR_INSTALLDIR}/r15_transport_layer.c \
		@rm -f ${LCMLDR_INSTALLDIR}/t_a2_network.h \
		@rm -f ${LCMLIB_INSTALLDIR}/liblcm.so.*,)
ifeq ($(LBITS),64)
	$(if ${LCMLDR_INSTALLDIR}, \
		@rm -f ${LCMLIB_INSTALLDIR}/liblcm_x64.so.*,)
endif
	$(if ${LCM_AUTO_DIR}, \
		@rm -rf ${LCM_AUTO_DIR},)
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
