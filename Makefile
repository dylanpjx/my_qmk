USER = nalyd

KEYBOARDS = sweep
NAME_sweep = ferris/sweep

all: $(KEYBOARDS)

.PHONY: $(KEYBOARDS)
$(KEYBOARDS):
	# init submodule
	git submodule update --init --recursive
	git submodule update --remote

	# cleanup old symlinks
	rm -rf qmk_firmware/keyboards/ferris/keymaps/$(USER)
	rm -rf qmk_firmware/users/$(USER)

	# add new symlinks
	ln -s $(shell pwd)/user qmk_firmware/users/$(USER)
	ln -s $(shell pwd)/$@ qmk_firmware/keyboards/ferris/keymaps/$(USER)

	# run lint check
	# cd qmk_firmware; qmk lint -km $(USER) -kb $(PATH_$@) --strict

	# run build
	# make BUILD_DIR=$(shell pwd) -j1 -C qmk_firmware $(NAME_$@):$(USER)
	qmk flash -kb ferris/sweep -km nalyd

	# cleanup symlinks
	rm -rf qmk_firmware/keyboards/ferris/keymaps/$(USER)
	rm -rf qmk_firmware/users/$(USER)

clean:
	rm -rf obj_*
	rm -f *.elf
	rm -f *.map
	rm -f *.hex
