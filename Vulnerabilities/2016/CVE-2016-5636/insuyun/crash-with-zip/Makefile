FILE = payload.txt
ZIP = import.zip

all: exploit
exploit: $(ZIP)
ifdef TARGET
	$(TARGET) run.py
else
	@echo 1>&2 "TARGET must be set"
endif

$(ZIP):
	@python generate.py

clean:
	rm -rf $(FILE) $(ZIP)

.PHONY: all exploit
