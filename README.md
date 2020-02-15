## What's this

This is an oscillator written in Qt.

## Compile tools:

   * Qt 5.12.7
   * MinGW 730_64

## Demo:

![running](README_src/running.gif)



## Configuration:
Find this code in "mainwindow.cpp", modify it to your need. that's a little ugly...

```c
// add points to chartview
int i, j;
for (j = 0, i = 0; i < this->datlen; i += 1, j++) {
	series->append(j, dat[i]);
}
```
