BUILDDIR=build

all: $(BUILDDIR)/test

$(BUILDDIR)/test: src/test.c $(BUILDDIR)/libfoo.so libfoo.so | $(BUILDDIR)
	gcc -o $@ -O2 -Wl,-rpath=\$$ORIGIN $< -L$(BUILDDIR) -lfoo
$(BUILDDIR)/libfoo.so: src/foo_good.c | $(BUILDDIR)
	gcc -o $@ -O2 -shared $<
libfoo.so: src/foo_bad.c
	gcc -o $@ -O2 -shared $<
$(BUILDDIR):
	mkdir $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) libfoo.so
