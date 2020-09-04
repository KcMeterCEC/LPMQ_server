# LPMQ_server

    ***             ******          *** ***           ***   
     *               *    *          ** **           *   *  
     *               *    *          ** **          *     * 
     *               *    *          ** **          *     * 
     *               *    *          ** **          *     * 
     *               *****           ** **          *     * 
     *               *               * * *          *     * 
     *               *               * * *          *     * 
     *               *               * * *          * **  * 
     *    *          *               * * *           *  **  
    *******         ***             ** * **           ***   
                                                         ** 
                                                         


This is a server for LPMQ(Linux Performance Monitor with Qt) project.

# How to build this project ? 
This project is built by **[cmake](https://cmake.org/)**,so you have to **[install](https://cmake.org/download/)** it first.

## Build for X86
> mkdir build && cd build
> 
> cmake ../
>
> make
## Build for ARM
For cross compiling,you should edit **cross_compile.cmake** file first.
Make sure the path of your compiler is right.
- dynamic link
> mkdir build && cd build
> 
> cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cross_compile.cmake
> 
> make

If your target didn't have c++ standard library,you should compile it with static link.
- static link
> mkdir build && cd build
> 
> cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cross_compile.cmake  -DSTATIC_LINK=yes
> 
> make