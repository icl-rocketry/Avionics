#include <vector>
#include <tuple>
#include <string>

template <class C, class T>
class serialisableElement
{
    static constexpr size_t size = sizeof(T);
    T C::* ptr;

public:
    constexpr serialisableElement(T C::* elem) : ptr(elem) {}

    std::vector<std::byte> serialise(const C& owner) const
    {
        std::vector<std::byte> ret(size);
        std::memcpy(ret.data(), &owner.*ptr, size);
        return ret;
    }

    void serialise(const C& owner, std::vector<std::byte>& buffer) const
    {
        const size_t bufSize = buffer.size();
        buffer.resize(bufSize + size);
        std::memcpy(buffer.data() + bufSize, &(owner.*ptr), size);
    }

    size_t deserialise(C& owner, const std::vector<std::byte>& buffer, size_t offset) const
    {
        assert(offset + size <= buffer.size());
        std::memcpy(&(owner.*ptr), buffer.data() + offset, size);
        return size;
    }

};

template <class C, class... T>
class serialiser
{
    std::tuple<serialisableElement<C, T>...> elements;

    static constexpr size_t member_size()
    {
        return (0 + ... + sizeof(T));
    }

    template <size_t I>
    void deserialise_impl(C& owner, const std::vector<std::byte>& buffer, size_t pos) const
    {
        if constexpr (I < sizeof...(T))
        {
            deserialise_impl<I + 1>(owner, buffer, pos + std::get<I>(elements).deserialise(owner, buffer, pos));
        }
    }

public:
    constexpr serialiser(T C::* ...ptrs) : elements(std::make_tuple(serialisableElement(ptrs)...)) {}

    std::vector<std::byte> serialise(const C& owner) const
    {
        std::vector<std::byte> ret;
        ret.reserve(member_size());
        std::apply(
            [&](auto&&... args) 
            {
                (..., args.serialise(owner, ret));
            },
                elements);
        return ret;
    }

    void deserialise(C& owner, const std::vector<std::byte>& buffer) const
    {
        deserialise_impl<0>(owner, buffer, 0);
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


    std::vector<std::byte> serialise() const
    {
        return getSerialiser().serialise(*this);
    }

    static myclass deserialise(const std::vector<std::byte>& buffer)
    {
        myclass ret;
        getSerialiser().deserialise(ret, buffer);
        return ret;
    }
};*/