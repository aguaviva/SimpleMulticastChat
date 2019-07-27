# SimpleMulticastChat

A simple chat that uses multicast, the beauty of this is that it requires no configuration, just launch it and chat. Everybody in the same subnetwork will receive the messages.

This is a proof of concept, a nicer thing is coming up!

## Getting started
First create the slolution/makefile:
```
$ git clone --recursive https://github.com/aguaviva/SimpleMulticastChat.git
$ cd SimpleMulticastChat
$ mkdir build
$ cd build
$ cmake ..
$ make
```

then build it and run it:
- Windows: open and run the solution, the audio will play right away
- Linux: run make, copy the *SimleMidi* into its parent dir so it can find the mid file, then you can run it.
