# lbdg: 将LDraw模型文件生成HTML搭建图图纸的程序。

介绍：
    lbdg是将LDraw模型文件通过载入模型、拆分模型、渲染模型和输出图纸四个部分来生成搭建图图纸的程序。它使用了C++、OpenGL、GLFW、GLAD、GLM和PNGWriter。它包括：
        * 生成HTML搭建图图纸
        * 通过图形窗口选择搭建图中每个步骤照相机的角度
        * 打印模型树
用户指南：
    lbdg语法：
        lbdg -c LDraw模型文件 [-m]
    说明：
        LDraw模型文件为后缀名为.ldr、.dat和.mpd的可以被LDview打开的模型文件。
        -m选项为创建一个图形界面手动选择搭建图中每个步骤照相机的角度，图形界面中的操作：W键将照相机向面朝方向的前方移动，A键将照相机向面朝方向的左方移动，S键将照相机向面朝方向的后方移动，D键将照相机向面朝方向的右方移动，鼠标移动可以改变面朝方向，ESC键进入一下个步骤。
    备注：
        需要在代码中正确配置ldraw和thumbnail两个文件夹的路径以及shader的路径。
        可能需要改的文件src/Core/src/main.cpp和src/Tools/fileget.cpp。
        生成的搭建图图纸位于当前目录下LDraw模型文件的同名文件夹下面，使用浏览器打开index.html可以浏览搭建图图纸。
编译说明：
    1:需要安装OpenGL、GLFW、PNGWriter库，需要安装CMake、Clang、make
    2:在lbdg目录下使用[mkdir build && cd build && cmake .. && make]命令编译，编译后的程序位于lbdg/build/src/Core/目录下，名字为lbdg。
