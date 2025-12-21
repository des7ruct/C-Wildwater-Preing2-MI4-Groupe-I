CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99
TARGET  = projet_final

SRC = main.c \
      avl.c \
      usine.c \
      histogramme.c \
      leaks.c \
      outils.c

OBJ = $(SRC:.c=.o)



all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@



clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all


scripts:
	@echo "Scripts disponibles :"
	@echo " - wildwater.shell"
	@echo " - plotgrand.gp"
	@echo " - plotpetit.gp"
