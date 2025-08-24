
all:
	@echo make what?

vms:
	@./sh/util/build_vm.sh all

bao:
	@./sh/util/build_bao.sh

copy:
	@./sh/util/copy_files.sh

clean:
	@./sh/util/clean.sh
