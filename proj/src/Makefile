# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = main.c utils.c timer.c keyboard.c KBC.c video.c game.c sprite.c breakout.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CPPFLAGS += -D PROJ

# include search paths for source files in subdirectories
CFLAGS += -I . -I drivers -I drivers/timer -I drivers/keyboard -I drivers/video -I game -I sprite -I breakout

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# Specify the search path for source files
.PATH: . drivers drivers/timer drivers/keyboard drivers/video game sprite breakout

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>