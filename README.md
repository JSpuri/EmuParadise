# EmuParadise
NES Emulator Project for MC861/MC871.

## Phase 1: Give the (NES) game away
<img align = "right" src="misc/unrelated_cartridge.png" alt="What an awesome cartridge!" height="250" width="">

**Requirements:**

* [asm6f](https://github.com/freem/asm6f) to assemble the game
* [mednafen](https://mednafen.github.io/) to run it

Once inside the UNRELATED folder, run these commands to assemble and play the game:

```
$ asm6f main.asm game.nes
$ mednafen game.nes
```

## Phase 2 to 4: Assembling the NES
In the root folder of the project, run:
* `$ make` to compile the current state of the emulator;
* `$ make test` to run current CPU tests;
* `$ make clean` to remove files created on testing and compiling cases;
> Note: We used C++ 14 

### Our emulator currently have:
* [X] CPU
* [X] PPU
* [ ] APU


## Criadores:
* 157642 - Alex Wei
* 155943 - João Víctor Brazileu Spuri
* 166779 - Eduardo Moreira Freitas de Souza
* 176522 - Jonathas Rocha Limirio Sardinha
