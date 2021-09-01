# What to do for "make all"
.PHONY: all
# all: $(TARGET).bin $(TARGET).dis size.stdout
all: $(TARGET).hex $(TARGET).dis size.stdout

# Turn the elf file into a bin file.
$(TARGET).hex: $(TARGET).elf
	@echo Creating bin file $@
	$(HX) -j .text -j .data -O ihex $(TARGET).elf $@

.PHONY: size.stdout
size.stdout: $(TARGET).elf
	$(SIZE) $<

# Generate disassembly file.
$(TARGET).dis: $(TARGET).elf
	@echo Creating disassembly file $@
	$(OBJDMP) -h -S $< > $@

# Link all the object files and any local library code used by them into an elf file.
# Force recompile if *any* header has changed.
$(TARGET).elf: $(SRCS) $(LIB_SRCS) $(HDRS)
	@echo Compiling all sources to elf file $@
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKFLAGS) -o $(TARGET).elf $(SRCS) $(LIB_SRCS)

.PHONY: clean
# Delete all bin, map, object, and elf files, and other assorted crud
clean:
	$(RM) *.bin *.map *.o *.a *.elf *.hex *.dep *.dis log.* *.xml* *~

.PHONY: write
write: $(TARGET).hex $(TARGET).dis
	$(WRITE) -F -v -c arduino -p $(PROCESSOR) -P $(PORT) -b 57600 \
	-D -U flash:w:$(TARGET).hex
