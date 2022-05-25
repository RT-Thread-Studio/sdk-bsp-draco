# RT-AK MicroPython扩展模块

## 1.简介

该目录为针对[RT-AK](https://github.com/RT-Thread/RT-AK)的MicroPython适配. 建议用户可先对C语言接口使用方法有所了解.

- RT-AK MicroPython的代码实现说明: [Module_Design_Description](Module_Design_Description.md)

## 2.编译配置说明

该文件夹加入具有RT-AK C库的工程目录中, 参考文件夹下的`Sconscript`脚本, `SConscript`脚本部分展示如下:

```python
if GetDepend('PRJ_USING_RT_AK_EXMODS'):
    CPPPATH = [cwd]
    src = Glob('*.c')

group = DefineGroup('RT-AK-LIB', src, depend = ['PRJ_USING_RT_AK_EXMODS'], CPPPATH = CPPPATH)
```

代码加入编译需要开启`PRJ_USING_RT_AK_EXMODS`宏.

具体配置方法示例:
在Kconfig中加入`PRJ_USING_RT_AK_EXMODS`, 配置语句如下(以K210平台为例):

```python
menu "mpy-extmods"
config PRJ_USING_EXTMODS_MISC
    bool "Enable MPY extmods"
    select PKG_USING_MICROPYTHON
    default y

    if PRJ_USING_EXTMODS_MISC
        menuconfig EXTMODS_MISC_USING_K210
            bool "Enable K210 extmods"
            default y

            if EXTMODS_MISC_USING_K210
                config EXTMODS_K210_LCDCTL
                    bool "Enable LCD Control"
                    default y
                config EXTMODS_K210_DVP
                    bool "Enable DVP Camera"
                    default n
            endif

        config PRJ_USING_RT_AK_EXMODS
            bool "Enable RT-AK MPY module"
            default y
    endif
endmenu
```

## 3.RT-AK MicroPython函数使用说明

### rt_ak模块

RT-AK的所有python函数包含在`rt_ak`模块中, 使用时应`import rt_ak`. 在python环境下使用`dir(rt_ak)`语句可查看模块所包含的函数名词.

#### Model对象

神经网络模型对象, 保存注册到RT-AK中的模型信息, 以及运行环境等.

#### ai_find(name)

该函数通过一个字符串类型的名字, 用于查找一个已注册到RT-AK中的模型. 若查找成功将返回一个`Model`对象实例, 否则返回`None`

使用示例:

```python
model=rt_ak.ai_find("cifar10_mb")
```

该方法对应RT-AK C接口`rt_ai_find()`

#### ai_init(model, size=0)

该函数初始化一个模型对象. `model`参数必须传入, `size`为运行时需要的内存, 在K210上指示模型输入的buffer大小. 可选参数. 无返回值

使用示例:

```python
model=rt_ak.ai_find("cifar10_mb")
rt_ak.ai_init(model)
```

该函数对应RT-AK C接口`rt_ai_init()`.

#### ai_run(model, input)

该函数执行模型一次前向推理, 参数`model`为要进行推理的模型, `input`为模型的输入数据, 无返回值.

使用示例:

```python
model=rt_ak.ai_find("cifar10_mb")
rt_ak.ai_init(model)
rt_ak.ai_run(model,img_888_chw)
```

该函数对于RT-AK C接口`rt_ai_run()`.

#### ai_output(model, index=0)

该函数返回最近一次执行模型推理的输出结果. 参数`model`为模型, `index`为要获取的输出索引. 返回值为结果的`list`对象.

使用示例:

```python
model=rt_ak.ai_find("cifar10_mb")
rt_ak.ai_init(model)
rt_ak.ai_run(model,img_888_chw)
predicts=rt_ak.ai_output(model,0)
predicts

# out[]: [0.0, 0.9, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0]
```

该函数对于RT-AK C接口`rt_ai_output()`.
