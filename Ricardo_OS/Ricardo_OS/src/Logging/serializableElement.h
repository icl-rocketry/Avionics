#ifndef SERIALIZABLEELEMENT_H
#define SERIALIZABLEELEMENT_H

#include <vector>
#include <tuple>
#include <string>
#include <Arduino.h>
#include <cassert>
#include <sstream>


template <class C, class T>
class serialisableElement
{
    static constexpr size_t size = sizeof(T);
    T C::* ptr; // member variable pointer to provide instance to pointer - https://riptutorial.com/cplusplus/example/6997/pointers-to-member-variables
    // allows classes to be static and allows pointers to class members to be defined at compile time

public:
    constexpr serialisableElement(T C::* elem) : ptr(elem) {} // constructor

    void stringify(const C& owner, std::stringstream &buffer) const 
    {
        buffer << owner.*ptr << ',';
    }

    void serialise(const C& owner, std::vector<uint8_t>& buffer) const
    {
        const size_t bufSize = buffer.size();
        buffer.resize(bufSize + size);
        memcpy(buffer.data() + bufSize, &(owner.*ptr), size);
    }

    size_t deserialise(C& owner, const std::vector<uint8_t>& buffer, size_t offset) const
    {
        assert(offset + size <= buffer.size()); //dump packet instead?
        memcpy(&(owner.*ptr), buffer.data() + offset, size);
        return size;
    }

};

template <class C, class... T> // variadic template
class serialiser
{
    std::tuple<serialisableElement<C, T>...> elements;

    static constexpr size_t member_size()
    {
        return (0 + ... + sizeof(T)); // unary fold expression, applies sizeof to all members in t and calculated total size
    }

    template <size_t I>
    void deserialise_impl(C& owner, const std::vector<uint8_t>& buffer, size_t pos) const
    {
        if constexpr (I < sizeof...(T)) // sizeof...(T) gives the number of elements in the pack expression
        {
            auto element_size = std::get<I>(elements).deserialise(owner, buffer, pos); // gets the I'th element from elements tuple and calls deserialise
            deserialise_impl<I + 1>(owner, buffer, pos + element_size); // recursion, increment I to process next element
        }
    }

public:
    // Constructor, creates serialisable element for all the member variable pointers
    constexpr serialiser(T C::* ...ptrs) : elements(std::make_tuple(serialisableElement(ptrs)...)) {} 

    std::vector<uint8_t> serialise(const C& owner) const
    {
        std::vector<uint8_t> ret; // buffer of serialized objects
        ret.reserve(member_size()); // allocate memory but dont change size so we can easily get the end of buffer in serialize

        std::apply(         // applies serialize to all members in elements
                            // to capture all used variables by value, use a capture value of =.
                            // to capture all used variables by reference, use a capture value of &.
            [&](auto&&... args)   
            {
                (..., args.serialise(owner, ret)); //apply serialise on all elements
            },
                elements);

        return ret; // return serialized bytes
    }

    std::string stringify(const C& owner) const
    {
        std::stringstream ss; // output string stream

        std::apply(        
            [&](auto&&... args)   
            {
                (..., args.stringify(owner, ss)); //apply stringify on all elements
            },
                elements);

        return ss.str(); // return string bytes
    }

    void deserialise(C& owner, const std::vector<uint8_t>& buffer) const
    {
        deserialise_impl<0>(owner, buffer, 0); // public implmentation of deserializer
    }

};

/*


class myclass {

    static constexpr auto getSerialiser()
    {
        auto ret = serialiser(&myclass::a, &myclass::b, &myclass::c);
        return ret;
    }

public:
    float a;
    int b;
    long c;


    std::vector<uint8_t> serialise() const
    {
        return getSerialiser().serialise(*this);
    }

    static myclass deserialise(const std::vector<uint8_t>& buffer)
    {
        myclass ret;
        getSerialiser().deserialise(ret, buffer);
        return ret;
    }
};

*/

#endif