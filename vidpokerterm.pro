# VidPokerTerm
# Copyright (c) 2020 Daniel Brook (danb358 {at} gmail {dot} com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

TEMPLATE = subdirs

# Uncomment the lines below to build the proper GUI interface
#SUBDIRS  = poker ui test
#ui.depends = poker

# Uncomment the lines below to build the Graphic/Text Mode LCD interface
#SUBDIRS  = poker test lcdinterface lcdspi lcdui
#lcdui.depends = poker lcdui lcdinterface

# Uncomment the lines below to build everything
SUBDIRS  = poker ui test lcdinterface lcdspi lcdui
lcdui.depends = poker lcdui lcdinterface
ui.depends = poker
