# 模块设计说明

## 1.简介

该文档用于简要说明RT-AK MicroPython扩展模块的设计及实现.

关于MicroPython添加自定义模块及相关接口说明请参考[micropython-usermod](https://micropython-usermod.readthedocs.io/en/latest/usermods_01.html)

## 2.C-->MicroPython开发基础

### Python相关概念

熟悉python的小伙伴会知道, python是面向对象的. 了解的更深一点的人或许知道, 与C语言不同, python中的变量本质上是个引用, 类似C语言的指针. 而指针可以理解为是一种若类型的变量, 其指向一个内存地址. 因此在python中，类型属于对象，对象有不同类型的区分，变量是没有类型的, 对象是在内存中的一个实体, 而变量是对其的一个引用(指针). 带着这个概念, 来理解用C开发Pyhthon模块会更加清晰.

一个基础的python函数其底层C函数的基本形式如下:

```c
mp_obj_t some_function(mp_obj_t some_variable, ...) {
    // some_variable is converted to fundamental C types (bytes, ints, floats, pointers, structures, etc.)
    ...
}

// 其中 typedef (void *) mp_obj_t 
```

某个类型的变量被传递给函数，函数将结果作为类型的对象返回。基本上，该变量只不过是一个8字节(地址位宽)的内存段，所有具体对象都需要进一步通过该变量进行进步解析获得。对于Python对象到C层面的解析, 有一系列宏可做判断.

如果我们想知道该变量是否是一个整数，我们可以检查布尔值

```c
MP_OBJ_IS_SMALL_INT(some_variable)
```

存储在其中的整数值可以通过调用下面宏获得:

```c
int value_of_some_variable = MP_OBJ_SMALL_INT_VALUE(some_variable);
```

在C层面, 想要返回一个整数给python, 同样需要C-->Python的编码:

```c
return mp_obj_new_int(value_of_some_variable);
```

更泛型的类型可以用宏处理，宏将对象作为第一个参数，并将指向该类型的指针作为第二个参数。现在，如果你想知道，是否是一个元组，你可以应用宏:

```c
mp_obj_is_type(some_variable, &mp_type_tuple);
```

判断变量所指的对象:

```c
mp_obj_is_type(myobject, &my_type);
```

还有关于用C添加MicroPython模块以及对象的相关方法参考[micropython-usermod](https://micropython-usermod.readthedocs.io/en/latest/usermods_01.html)

## 3.源文件及代码介绍

源文件列表:

| 源文件 | 说明 |
| ------ | ---- |
| rt_ai_mpython.c/.h | 该文件为`rt_ak`模块及相关函数主要适配代码 |
| moddefs_rt_ai.h | 该头文件为模块声明文件, 该头文件被`moddefs.user.extmods.h`包含, 用于导入MicroPython |
| qstrdefs_rt_ai.h | 标识字符串, 如python中所输入的如变量名、函数名等用字符串进行标识 |

### rt_ak Module

C层面使用键值对的方式来描述python变量. 键为python层显示的变量名, 值为变量的值. 

```c
STATIC const mp_rom_map_elem_t rt_ak_module_globals_table[] = {
        // 键 对应python层面rt_ak.__name__(变量)            值(字符串)
        { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_RT_AK) },
        // 键 rt_ak.model(变量)                    值(对象)
        { MP_OBJ_NEW_QSTR(MP_QSTR_model), MP_ROM_PTR(&py_model_type)},
        // 键 对应rt_ak.ai_find()                      值(C函数实现)
        { MP_OBJ_NEW_QSTR(MP_QSTR_ai_find), MP_ROM_PTR(&py_rt_ai_find_obj) },
        { MP_OBJ_NEW_QSTR(MP_QSTR_ai_init), MP_ROM_PTR(&py_rt_ai_init_obj) },
        { MP_OBJ_NEW_QSTR(MP_QSTR_ai_run), MP_ROM_PTR(&py_rt_ai_run_obj) },
        { MP_OBJ_NEW_QSTR(MP_QSTR_ai_output), MP_ROM_PTR(&py_rt_ai_output_obj) },
};
    // 理解为rt_ak_module_globals 注册到rt_ak_module_globals_table
STATIC MP_DEFINE_CONST_DICT(rt_ak_module_globals, rt_ak_module_globals_table);
// 定义一个模块
const mp_obj_module_t rt_ak_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&rt_ak_module_globals,
};
```

关于MP_QSTR_xxx,  MicroPython使用字符串池的方式来保存RAM和ROM内容。这避免了必须存储相同字符串的重复副本。这主要适用于代码中的标识符，因为像函数或变量名这样的东西很可能出现在代码中的多个位置。在MicroPython中，嵌入字符串称为QSTR(唯一字符串)。例如在python中输入函数, 并且使用关键字参数时如`add(a=1, b=2)`其中`add` `a` `b`都是作为字符串输入到python解析器中, 在C层面中使用`MP_QSTR_add` `MP_QSTR_a` `MP_QSTR_a`来进行匹配函数名和变量名, 使用字符串标识的方式避免了在C层面多处使用`"xxxx"`的文字池.

QSTR值(类型为QSTR)是QSTR池的索引。qstr存储它们的长度和内容的哈希值，以便在重复数据删除过程中进行快速比较。所有与字符串相关的字节码操作都使用QSTR参数. 例如在MicroPython C代码中，任何应该被嵌入到最终固件中的字符串都被写成MP_QSTR_Foo。在编译时，它将求值为qstr值，该值指向qstr池中“Foo”的索引.

QSTR值可使用工程跟目录下的genqstr.py来进行生成.

### Model

```c
// 类型对应的C实现
typedef struct
{
    mp_obj_base_t base; //每个python对应的c类型的具体实现需要包含该成员, 该成员指向const mp_obj_type_t py_model_type;
    rt_ai_t handle;
    void *buf;
} py_model_obj_t;
// 类型标识
const mp_obj_type_t py_model_type =
{
    {&mp_type_type},
    .name = MP_QSTR_model,
    // .make_new = make_new,
    .locals_dict = (mp_obj_t)&locals_dict
};
```
