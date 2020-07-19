//
// Created by 10481 on 2020/7/17.
//
#include <stddef.h> // ptrdiff_t size_t
#include "stl_config.h"

#ifndef XSQ_STL_ITERATOR_H
#define XSQ_STL_ITERATOR_H
__STL_BEGIN_NAMESPACE
////////////////////////////////////////////////////////////////////////////////
// STL迭代器定义,迭代器实际上属于一种泛型指针 by value and by reference?
////////////////////////////////////////////////////////////////////////////////
// STL中有五种迭代器类型
// Input Iterator               read only
// Output Iterator              write only
// Forward Iterator             允许"写入型"算法在其指向区间进行操作
// Bidirectional Iterator       提供双向访问能力
// Random Access Iterator       支持原生指针具有的全部能力
////////////////////////////////////////////////////////////////////////////////
// 类型从属关系, 子类适用于接受父类类型的算法, 但是效率可能不佳
//
//              Input Iterator
//                    ↑
//             Forward Iterator
//                    ↑
//           Bidirectional Iterator
//                    ↑
//           Random Access Iterator
////////////////////////////////////////////////////////////////////////////////

struct input_iterator_tag {             //标记迭代器类型
};
struct output_iterator_tag {
};
struct forward_iterator_tag : public input_iterator_tag {
};
struct bidirectional_iterator_tag : public forward_iterator_tag {
};
struct random_access_iterator_tag : public bidirectional_iterator_tag {
};
template<class _Tp, class _Distance>
struct input_iterator {
    typedef input_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
};
struct output_iterator {
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;
};
template<class _Tp, class _Distance>
struct forward_iterator {
    typedef forward_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
};
template<class _Tp, class _Distance>
struct bidirectional_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
};
template<class _Tp, class _Distance>
struct random_access_iterator {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
};
template<class _Category, class _Tp, class _Distance = ptrdiff_t, class _Pointer = _Tp *, class _Reference = _Tp &>
struct stl_iterator {
    typedef _Category iterator_category;
    typedef _Tp value_type;
    typedef _Distance difference_type;
    typedef _Pointer pointer;
    typedef _Reference reference;
};

template<class _Iterator>
struct iterator_traits {
    typedef typename _Iterator::iterator_categroy iterator_category;
    typedef typename _Iterator::value_type value_type;
    typedef typename _Iterator::difference_type difference_type;
    typedef typename _Iterator::pointer pointer;
    typedef typename _Iterator::reference reference;
};
//指针类型的偏特化
template<class _Tp>
struct iterator_traits<_Tp *> {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp *pointer;
    typedef _Tp &reference;
};

template<class _Tp>
struct iterator_traits<const _Tp *> {
    typedef random_access_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef const _Tp *pointer;
    typedef const _Tp &reference;
};

//used to identify iterator_tag
template<class _Iter>
inline typename iterator_traits<_Iter>::iterator_category __iterator_category(const _Iter &) {
    typedef typename iterator_traits<_Iter>::iterator_category _Category;
    return _Category();
}

template<class _Iter>
inline typename iterator_traits<_Iter>::iterator_category iterator_category(const _Iter &__i) {
    return __iterator_category(__i);
}

//confused
template<class _Iter>
inline typename iterator_traits<_Iter>::difference_type *__distance_type(const _Iter &) {
    return static_cast<typename iterator_traits<_Iter>::difference_type *>(0);
}

template<class _Iter>
inline typename iterator_traits<_Iter>::difference_type *distance_type(const _Iter &__i) {
    return __distance_type(__i);
}

template<class _Iter>
inline typename iterator_traits<_Iter>::value_type *__value_type(const _Iter &) {
    return static_cast<typename iterator_traits<_Iter>::value_type *>(0);
}

template<class _Iter>
inline typename iterator_traits<_Iter>::value_type *value_type(const _Iter &__i) {
    return __value_type(__i);
}

#define __ITERATOR_CATEGORY(__i) __iterator_category(__i)
#define __DISTANCE_TYPE(__i)     __distance_type(__i)
#define __VALUE_TYPE(__i)        __value_type(__i)


template<class _InputIterator, class _Distance>
inline void __distance(_InputIterator __first, _InputIterator __last, _Distance &__n, input_iterator_tag) {
    while (__first != __last) {
        ++__first;
        ++__n;
    }
}

template<class _RandomAccessIterator, class _Distance>
inline void __distance(_RandomAccessIterator __first, _RandomAccessIterator __last, _Distance &__n,
                       random_access_iterator_tag) {
    __n += __last - __first;
}

template<class _InputIterator, class _Distance>
inline void distance(_InputIterator __first, _InputIterator __last, _Distance &__n) {
    __distance(__first, __last, __n, iterator_category(__first));
}
////////////////////////////////////////////////////////////////////////////////
// template <class InputIterator, class Distance>
// inline void distance(InputIterator first, InputIterator last, Distance& n)
////////////////////////////////////////////////////////////////////////////////
//                                distance
//                                   |
//                                   |---------------- 判断迭代器类型
//                 Input Iterator    ↓   Random Access Iterator
//               -------------------------------------------
//               |                                         |
//               |                                         |
//               ↓                                         |
// __distance(..., input_iterator_tag)                     |
// while (first != last) { ++first; ++n; }                 |
//                                                         ↓
//                                __distance(..., random_access_iterator_tag)
//                                n += last - first;
////////////////////////////////////////////////////////////////////////////////

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

//#ifdef __STL_USE_NAMESPACES
//template <class Category, class T, class Distance = ptrdiff_t,
//          class Pointer = T*, class Reference = T&>
//struct stl_iterator {
//  typedef Category  iterator_category;
//  typedef T         value_type;
//  typedef Distance  difference_type;
//  typedef Pointer   pointer;
//  typedef Reference reference;

template<class _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    __distance(_InputIterator __first, _InputIterator __last, input_iterator_tag) {
        typename iterator_traits<_InputIterator>::difference_type __n = 0;
        while (__first != __last) {
            ++__first;
            ++__n;
        }
        return __n;
    }

    template<class _RandomAccessIterator>
    inline typename iterator_traits<_RandomAccessIterator>::difference_type
    __distance(_RandomAccessIterator __first, _RandomAccessIterator __last, random_access_iterator_tag) {
        return __last - __first;
    }

    template<class _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    distance(_InputIterator __first, _InputIterator __last) {
        typedef typename iterator_traits<_InputIterator>::iterator_category _Category;
        __distance(__first, __last, _Category());
    }

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template<class _InputIterator, class _Distance>
inline void __advance(_InputIterator &__i, _Distance __n, input_iterator_tag) {
    while (__n--) {
        ++__i;
    }
}

template<class _BidirectionalIterator, class _Distance>
inline void __advance(_BidirectionalIterator &__i, _Distance __n, bidirectional_iterator_tag) {
    if (__n > 0) {
        while (__n--) {
            ++__i;
        }
    } else {
        while (__n++) {
            --__i;
        }
    }
}

template<class _RandomAccessIterator, class _Distance>
inline void __advance(_RandomAccessIterator &__i, _Distance __n, random_access_iterator_tag) {
    __i += __n;
}

template<class _InputAccessIterator, class _Distance>
inline void advance(_InputAccessIterator &__i, _Distance __n) {
    __advance(__i, __n, __iterator_category(__i));
}
__STL_END_NAMESPACE
#endif //XSQ_STL_ITERATOR_H
