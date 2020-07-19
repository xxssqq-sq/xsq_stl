//
// Created by 10481 on 2020/7/15.
//
//😃❤💙❤💙❤💙❤💙❤💙❤💙❤💙❤💙❤💙❤
#ifndef XSQ_STL_STL_ALLOC_H
#define XSQ_STL_STL_ALLOC_H
#include <cstdlib> //malloc, free
#include <cstdio>
#include "stl_config.h"

// config文件所做的事情：
//  (1)  如果编译器没有定义 bool，true，false，就定义它们
//  (2)  如果编译器的标准程序库未支持 drand48() 函数，就定义 __STL_NO_DRAND48
//  (3)  如果编译器无法处理 static member of template classes（模板类的静态成员），就定义 __STL_STATIC_TEMPLATE_MEMBER_BUG
//  (4)  如果编译器未支持关键词 typename，就将‘typename’定义为一个 null macro（空的宏）.
//  (5)  如果编译器支持 partial specialization of class templates（模板类的部分特化），
//       就定义 __STL_CLASS_PARTIAL_SPECIALIZATION.
//  (6)  如果编译器支持 partial ordering of function templates (亦称为 partial specialization of function templates），
//       就定义 __STL_FUNCTION_TMPL_PARTIAL_ORDER
//  (7)  如果编译器允许我们在调用一个 function template 时可以明白指定其 template arguments，
//       就定义__STL_EXPLICIT_FUNCTION_TMPL_ARGS
//  (8)  如果编译器支持 template members of classes，就定义 __STL_MEMBER_TEMPLATES
//  (9)  如果编译器不支持关键词 explicit ，就定义 ‘explicit’为一个 null macro.
//  (10) 如果编译器无法根据前一个 template parameters 设定下一个 template parameters 的默认值，
//       就定义 __STL_LIMITED_DEFAULT_TEMPLATES
//  (11) 如果编译器针对 non-type template parameters 执行 function template 的参数推导（argument deduction）时有问题，
//       就定义 __STL_NON_TYPE_TMPL_PARAM_BUG
//  (12) 如果编译器无法支持迭代器的 operator->，就定义 __SGI_STL_NO_ARROW_OPERATOR
//  (13) 如果编译器（在你所选择的模式中）支持 exceptions，就定义 __STL_USE_EXCEPTIONS
//  (14) 如果我们将 STL 放入命名空间中，则定义 __STL_USE_NAMESPACES
//  (15) 如果本程序库由 SGI 编译器来编译，并且使用这并未选择 pthreads 或其他 threads，就定义 __STL_SGI_THREADS
//  (16) 如果本程序由一个 WIN32 编译器编译，并且在多线程模式下，就定义 __STL_WIN32THREADS
//  (17) 适当地定义与namespace 相关的 macros (如 __STD, __STL_BEGIN_NAMESPACE, 等.)
//  (18) 适当地定义与exception 相关的 macros (如 __STL_TRY, __STL_UNWIND, 等.)
//  (19) 根据 __STL_ASSERTIONS 是否定义，将 __stl_assert 定义为一个测试操作或一个 null macro
__STL_BEGIN_NAMESPACE
template<int __inst>
class __malloc_alloc_template{
public:
    // 分配指定大小的内存(size_t n)， 如果分配失败, 则进入循环分配阶段
    static void *allocate(size_t __size){
        void *__result = malloc(__size);
        if (0 == __result) {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        return __result;
    }
// 重新分配内存大小, 第二个参数是为了兼容operator new
    static void deallocate(void *__p, size_t /* __n */) {
        free(__p);
    }
    static void *reallocate(void *__p, size_t /*old_size */ , size_t __new_size) {
        void *__result = realloc(__p, __new_size);
        if (0 == __result) {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        return __result;
    }
};

// 设置错误处理函数, 返回原来的函数指针
 // 不属于C++标准规定的接口
typedef __malloc_alloc_template<0> malloc_alloc;

// 这个类中的接口其实就是STL标准中的allocator的接口
// 例如: stl_vector.h中
// template <class T, class Alloc = alloc>
// class vector
// {
//      ...
// protected:
//      typedef simple_alloc<value_type, Alloc> data_allocator;
//      ...

template<class _Tp, class _Alloc>
class simple_alloc {
public:
    static _Tp *allocate(size_t __n) {
        return 0 == __n ? 0 : (_Tp *) _Alloc::allocate(__n * sizeof(_Tp));
    }

    static _Tp *allocate(void) {
        return (_Tp *) _Alloc::allocate(sizeof(_Tp));
    }

    static void deallocate(_Tp *__p, size_t __n) {
        if (0 != __n) {
            _Alloc::deallocate(__p, __n * sizeof(_Tp));
        }
    }

    static void deallocate(_Tp *__p) {
        _Alloc::deallocate(__p, sizeof(_Tp));
    }
};

// Allocator adaptor to check size arguments for debugging.
// Reports errors using assert.  Checking can be disabled with
// NDEBUG, but it's far better to just use the underlying allocator
// instead when no checking is desired.
// There is some evidence that this can confuse Purify.

typedef malloc_alloc alloc;

__STL_END_NAMESPACE

#endif //XSQ_STL_STL_ALLOC_H
