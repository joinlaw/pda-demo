# Copyright (C) 2021  mr.fantastic

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

C_FLAGS = `pkg-config --cflags --libs gtk4` -lm
CXX_FLAGS = -O3 -Wall
DEBUG = -g3 -Wall -Wpadded
HEADERS = src/pda.h
SRC = src/demo.c src/pda.c
BIN = demo
CC ?= gcc


$(BIN): $(SRC) $(HEADERS)
	$(CC) $(C_FLAGS) $(CXX_FLAGS) $(SRC) $(HEADERS) -o $(BIN)

debug: $(SRC) $(HEADERS)
	$(CC) $(C_FLAGS) $(DEBUG) $(SRC) $(HEADERS) -o $(BIN)

clean: $(BIN)
	rm $(BIN) *~ src/*~

check-syntax:           # flymake support
	$(CC) -Wall -pedantic -Wextra --coverage -ftest-coverage -fanalyzer -std=gnu99 $(CFLAGS) $(CHK_SOURCES)

.PHONY: check-syntax clean debug
