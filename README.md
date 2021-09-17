# How to build?

1. Build all the packages.

   ```shell
   make world -j4
   ```

2. Only build one package.

   ```shell
   make packages/${package_name}/compile -j4
   ```

3. Clean one package and compile.

   ```shell
   make packages/${package_name}/{clean,compile} -j4	
   ```

# How to start with sys_call_demo

## How to add package in sys_call_demo?

1. Add you package name in packages.list.

   ```shell
   echo ${package_name} >> packages/sys_call_demo/packages.list
   ```

2. Add Makefile for you package  and implement run target

   ```shell
   vi packages/sys_call_demo/${package_name}/Makefile
   ```

## How to compile sys_call_demo

1. Now only support compile all packages in sys_call_demo at one time

   ```shell
   make packages/sys_call_demo/{clean,compile} -j4
   ```

## How to run sys_call_demo?

1. Run without parameters.

   ```shell
   make packages/sys_call_demo/${package_name}/run
   ```

2. Run with one parameter.

   ```shell
   make packages/sys_call_demo/${package_name}/run argv=${parameter}
   ```

3. Run with  Multi parameters.

   ```shell
   # parameters="par1 par2 par3"
   make packages/sys_call_demo/${package_name}/run "argv='${parameters}'"
   ```

