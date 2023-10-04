It's hacked version of dtach made for chromeos to overcome restricted sudo

````
+/*
+    dtach - A simple program that emulates the detach feature of screen.
+    Copyright (C) 2004-2016 Ned T. Crigler
+
+    This program is free software; you can redistribute it and/or modify
+    it under the terms of the GNU General Public License as published by
+    the Free Software Foundation; either version 2 of the License, or
+    (at your option) any later version.
+
+    This program is distributed in the hope that it will be useful,
+    but WITHOUT ANY WARRANTY; without even the implied warranty of
+    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
+    GNU General Public License for more details.
+
+    You should have received a copy of the GNU General Public License
+    along with this program.  If not, see <http://www.gnu.org/licenses/>.
+*/
````

The goal is to run listener in priveledged environment and use fifo to spawn detached shell and then use it as a replacement for restricted crosh shell.

````
Usage: ./plugctl listen <fifo>|create <socket> <cmd> <args>|connect <socket>

To spawn a command in listener environment: 

  echo -e -n "cmd\0socket\0" > fifo # without arguments 
  cut -c1 fifo > /dev/null
  ./plugctl connect <socket>

````

run listener on vt2 or as upstart task...
