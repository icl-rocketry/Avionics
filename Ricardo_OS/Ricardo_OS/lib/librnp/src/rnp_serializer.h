#pragma once

#include <vector>
#include <tuple>
#include <string>
#include <cassert>
#include <sstream>
#include <cstring>


template <class C, class T>
class RnpSerializableElement
{
    static constexpr size_t size = sizeof(T);
    T C::* ptr; // member variable pointer to provide instance to pointer - https://riptutorial.com/cplusplus/example/6997/pointers-to-member-variables
    // allows classes to be static and allows pointers to class members to be defined at compile time

public:
    constexpr RnpSerializableElement(T C::* elem) : ptr(elem) {} // constructor

    void serialize(const C& owner, std::vector<uint8_t>& buffer) const
    {
        const size_t bufSize = buffer.size();
        buffer.resize(bufSize + size);
        std::memcpy(buffer.data() + bufSize, &(owner.*ptr), size);
    }

    size_t deserialize(C& owner, const std::vector<uint8_t>& buffer, size_t offset) const
    {
        assert(offset + size <= buffer.size()); //dump packet instead?
        std::memcpy(&(owner.*ptr), buffer.data() + offset, size);
        return size;
    }

};

template <class C, class... T> // variadic template
class RnpSerializer
{
    std::tuple<RnpSerializableElement<C, T>...> elements;

    template <size_t I>
    void deserialize_impl(C& owner, const std::vector<uint8_t>& buffer,[[maybe_unused]] size_t pos) const
    {
        
        if constexpr (I < sizeof...(T)) // sizeof...(T) gives the number of elements in the pack expression
        {
            auto element_size = std::get<I>(elements).deserialize(owner, buffer, pos); // gets the I'th element from elements tuple and calls deserialize
            deserialize_impl<I + 1>(owner, buffer, pos + element_size); // recursion, increment I to process next element
        }
    }

public:
    // Constructor, creates serializable element for all the member variable pointers
    constexpr RnpSerializer(T C::* ...ptrs) : elements(std::make_tuple(RnpSerializableElement(ptrs)...)) {} 

    static constexpr size_t member_size()
    {
        return (0 + ... + sizeof(T)); // unary fold expression, applies sizeof to all members in t and calculated total size
    }

    std::vector<uint8_t> serialize(const C& owner) const
    {
        std::vector<uint8_t> ret; // buffer of serialized objects
        ret.reserve(member_size()); // allocate memory but dont change size so we can easily get the end of buffer in serialize

        std::apply(         // applies serialize to all members in elements
                            // to capture all used variables by value, use a capture value of =.
                            // to capture all used variables by reference, use a capture value of &.
            [&](auto&&... args)   
            {
                (..., args.serialize(owner, ret)); //apply serialize on all elements
            },
                elements);

        return ret; // return serialized bytes
    }


    void deserialize(C& owner, const std::vector<uint8_t>& buffer) const
    {
        deserialize_impl<0>(owner, buffer, 0); // public implmentation of deserializer
    }

};


