
all:
	@echo make what?

vms:
	@./sh/build_vm.sh all

bao:
	@./sh/build_bao.sh

copy:
	@./sh/copy_files.sh
